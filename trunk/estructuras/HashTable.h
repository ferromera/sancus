/*
 * Los desbordes se resuelven usando direccionamiento abierto y doble dispersión.
 * Se usa una funcion para obtener la direccion base y otra para dar los saltos
 * en caso de desborde
 *
 * @author Alfredo Scoppa
 * @since Sep 24, 2011
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <iostream>
#include "MathUtils.h"
#include "File.h"
#include "Bucket.h"
#include <list>
#include "../records/Uint16Key.h"
#include "../records/Record.h"
#include "HashTableExceptions.h"
#include "Function.h"
#include <sys/stat.h>

#define PACKAGE_DENSITY 0.7
#define BUCKET_LOAD_FACTOR 0.75

template<class T, unsigned int bucketSize>
class HashTable {

private:
	File * file;

	unsigned int size;
	unsigned int maxNumberOfRehashes;

	Function<T> * hashFunction;
	Function<T> * rehashFunction;

	int insertRecord(T & record, Function<T> * function, unsigned int jump);

public:

	/**
	 * Primitiva de creación
	 *
	 * @file
	 * @recordsPerBucket numero de registros por cubeta
	 * @maxNumberOfRecords numero maximo de registros en el archivo (cuando se llegue a este numero se debera
	 * enfrentar una reorganización de la tabla).
	 */
	HashTable(std::string & path, const unsigned int recordsPerBucket,
			const unsigned int maxNumberOfRecords);

	/**
	 * Primitiva de carga
	 *
	 * @path el path a un archivo directo existente (se supone que es un archivo valido)
	 */
	HashTable(std::string & path);

	~HashTable();

	/**
	 * Primitiva de insercion
	 *
	 * @record el registro que se quiere insertar
	 * @throws RehashCountException cuando se supera la cantidad maxima de rehashes
	 */
	void insert(T & record);

	/**
	 * Primitiva de recuperacion
	 *
	 * @key la clave del registro que se quiere recuperar
	 * @returns una copia del registro con la clava dada
	 * @throws RecordNotFoundException cuando no se puede recuperar el registro
	 */
	T get(const typename T::Key & key);

	/**
	 * Primitiva de borrado
	 *
	 * @record el registro que se quiere borrar
	 * @throws RecordNotFoundException cuando no se puede recuperar el registro
	 */
	void remove(T & record);

	/**
	 * @returns la cantidad de "filas" de la tabla
	 */
	unsigned int getSize();

};

template<class T, unsigned int bucketSize>
HashTable<T, bucketSize>::HashTable(std::string & path,
		const unsigned int recordsPerBucket,
		const unsigned int maxNumberOfRecords) {

	this->file = new File(path, File::NEW | File::IO | File::BIN);
	this->size = maxNumberOfRecords / (PACKAGE_DENSITY * recordsPerBucket);

	if (!MathUtils::isPrime(size)) {
		this->size = MathUtils::nextPrime(size);
	}

	this->hashFunction = new HashFunction<T> (this->size);
	this->rehashFunction = new ReHashFunction<T> (this->size);

	//por propiedad de los numeros primos luego de size rehashes se vuelve a repetir el primer resultado.
	this->maxNumberOfRehashes = this->size;

	Bucket<bucketSize> * bucket = new Bucket<bucketSize> ();
	bucket->freeSpace = sizeof(bucket->bytes);
	bucket->count = 0;
	bucket->overflow = false;

	for (unsigned int i = 0; i < this->size; i++) {
		file->write((char *) bucket, bucketSize);
	}

	file->flush();

	delete (bucket);
}

template<class T, unsigned int bucketSize>
HashTable<T, bucketSize>::HashTable(std::string & path) {
	this->file = new File(path, File::IO | File::BIN);
	this->file->seek(0, File::END);
	this->size = file->tell()/bucketSize;
	this->maxNumberOfRehashes = this->size;
	this->hashFunction = new HashFunction<T> (this->size);
	this->rehashFunction = new ReHashFunction<T> (this->size);
}

template<class T, unsigned int bucketSize>
void HashTable<T, bucketSize>::insert(T & record) {
	unsigned int rehashingCount = 0;

	int jump = insertRecord(record, this->hashFunction, 0);

	if (jump != -1) {
		if (rehashingCount < maxNumberOfRehashes) {
			rehashingCount++;
			insertRecord(record, this->rehashFunction, jump);
		} else {
			throw RehashCountException();
		}
	}
}

template<class T, unsigned int bucketSize>
int HashTable<T, bucketSize>::insertRecord(T & record, Function<T> * function,
		unsigned int jump) {
	Bucket<bucketSize> * bucket = new Bucket<bucketSize> ();
	unsigned int position = (function->hash(record.getKey(), jump));
	unsigned int insertOffset = (position * bucketSize);

	file->seek(insertOffset, File::BEG);
	file->read((char *) bucket, bucketSize);
	char * buffer = bucket->bytes;

	if (!bucket->overflow) {
		//Hay un bucket disponible sin overflow y debo validar que se cumpla la unicidad

		//Salto los bytes de control
		file->seek(5, File::CUR);

		typename std::list<T> bufferedRecords;

		for (int i = 0; i < bucket->count; i++) {
			T * recordFromBucket = new T(&buffer);

			bufferedRecords.push_back(*recordFromBucket);

			if (*recordFromBucket > record) { //NO ESTA! Porque los registros estan almacenados en orden
				break;
			}

			if (*recordFromBucket == record) {
				delete (recordFromBucket);
				throw UniqueViolationException();
			}
		}

		//VERIFICAR EL FACTOR DE CARGA DEL BUCKET
		unsigned int effectiveSizeForRecords = bucketSize - 5;

		if ((bucket->freeSpace - record.size()) > effectiveSizeForRecords * (1
				- BUCKET_LOAD_FACTOR)) {

			//No existe en el bucket debemos insertar (respetando el orden);
			bufferedRecords.push_front(record);

			// MENSAJE DE LOG
			std::cout << "SE inserta el record con id: "
					<< record.getKey().getKey() << ". En el bucket: "
					<< position << std::endl;

			bufferedRecords.sort();
			typename std::list<T>::iterator recordsIterator;

			//free bucket bytes
			for (unsigned int i = 0; i < sizeof(bucket->bytes); i++) {
				bucket->bytes[i] = 0;
			}

			buffer = bucket->bytes;

			bucket->freeSpace -= record.size();
			bucket->count++;

			recordsIterator = bufferedRecords.begin();

			while (recordsIterator != bufferedRecords.end()) {
				T r = (*recordsIterator);
				r.write(&buffer);
				recordsIterator++;
			}

			file->seek(insertOffset, File::BEG);
			file->write((char *) bucket, bucketSize);
			file->flush();

			delete (bucket);

			return -1;
		} else {
			bucket->overflow = true;

			file->seek(insertOffset, File::BEG);
			file->write((char *) bucket, bucketSize);
			file->flush();

			return position;
		}

	} else { //hubo overflow, retornara con el ultimo resultado de la funcion de hash
		delete (bucket);
		return position;
	}
}

template<class T, unsigned int bucketSize>
T HashTable<T, bucketSize>::get(const typename T::Key & key) {
	Bucket<bucketSize> * bucket = new Bucket<bucketSize> ();
	unsigned int offset;
	unsigned int rehashingCount = 0;
	unsigned int position = hashFunction->hash(key, 0);
	char * buffer;

	while (rehashingCount < maxNumberOfRehashes) {
		if (rehashingCount > 0) {
			position = rehashFunction->hash(key, position);
		}

		offset = (position * bucketSize);

		file->seek(offset, File::BEG);
		file->read((char *) bucket, bucketSize);
		buffer = bucket->bytes;

		//Salto los bytes de control
		file->seek(5, File::CUR);

		if (bucket->count == 0) {
			if (!bucket->overflow) {
				delete (bucket);
				throw RecordNotFoundException();
			} else {
				rehashingCount++;
			}
		}

		for (int i = 0; i < bucket->count; i++) {
			T * recordFromBucket = new T(&buffer);

			if (recordFromBucket->getKey() > key) {
				if (!bucket->overflow) {
					delete (recordFromBucket);
					throw RecordNotFoundException();
				} else {
					rehashingCount++;
					break;
				}
			}

			if (recordFromBucket->getKey() == key) {
				T recordCopy = *recordFromBucket;
				delete (bucket);
				return recordCopy;
			}
		}

		delete (bucket);
	}

	throw RecordNotFoundException();
}

template<class T, unsigned int bucketSize>
void HashTable<T, bucketSize>::remove(T & record) {
	Bucket<bucketSize> * bucket = new Bucket<bucketSize> ();
	unsigned int offset;
	unsigned int rehashingCount = 0;
	unsigned int position = hashFunction->hash(record.getKey(), 0);
	char * buffer;
	bool found;
	typename std::list<T> bufferedRecords;

	while (rehashingCount < maxNumberOfRehashes) {
		if (rehashingCount > 0) {
			position = rehashFunction->hash(record.getKey(), position);
		}

		offset = (position * bucketSize);

		file->seek(offset, File::BEG);
		file->read((char *) bucket, bucketSize);
		buffer = bucket->bytes;

		//Salto los bytes de control
		file->seek(5, File::CUR);

		if (bucket->count == 0) {
			if (!bucket->overflow) {
				delete (bucket);
				throw RecordNotFoundException();
			} else {
				found = false;
				rehashingCount++;
			}
		}

		for (int i = 0; i < bucket->count; i++) {

			T * recordFromBucket = new T(&buffer);

			if (*recordFromBucket > record) {
				if (!bucket->overflow) {
					delete (recordFromBucket);
					throw RecordNotFoundException();
				} else {
					delete (recordFromBucket);
					rehashingCount++;
					found = false;
					break;
				}
			}

			if (*recordFromBucket != record) {
				bufferedRecords.push_front(record);
			} else {
				found = true;
			}
		}

		if (found) {
			//free bucket bytes
			for (unsigned int i = 0; i < sizeof(bucket->bytes); i++) {
				bucket->bytes[i] = 0;
			}

			buffer = bucket->bytes;

			bucket->freeSpace += record.size();
			bucket->count--;

			typename std::list<T>::iterator recordsIterator;

			recordsIterator = bufferedRecords.begin();

			while (recordsIterator != bufferedRecords.end()) {
				T r = (*recordsIterator);
				r.write(&buffer);
				recordsIterator++;
			}

			file->seek(offset, File::BEG);
			file->write((char *) bucket, bucketSize);
			file->flush();

			delete (bucket);
			break;
		}
	}
}

template<class T, unsigned int bucketSize>
unsigned int HashTable<T, bucketSize>::getSize() {
	return this->size;
}

template<class T, unsigned int bucketSize>
HashTable<T,bucketSize>::~HashTable(){
	delete(hashFunction);
	delete(rehashFunction);
	delete(file);
}

#endif /* HASHTABLE_H_ */

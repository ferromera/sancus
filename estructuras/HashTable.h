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
#include <algorithm>

#define PACKAGE_DENSITY 0.7
#define BUCKET_LOAD_FACTOR 0.75

template<class T, unsigned int bucketSize>
class HashTable {

private:
	File * file;

	/* El numero de filas de la tabla*/
	unsigned int size;
	unsigned int maxNumberOfRecords;
	unsigned int maxNumberOfRehashes;

	Function<T> * hashFunction;
	Function<T> * rehashFunction;

	unsigned int insertRecord(T & record, Function<T> * function,
			unsigned int jump);

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

	void load(File & file);

	void insert(T & record);

	T * get(const typename T::Key & key);

	void remove(T & record);

	unsigned int getSize();

};

template<class T, unsigned int bucketSize>
HashTable<T, bucketSize>::HashTable(std::string & path,
		const unsigned int recordsPerBucket,
		const unsigned int maxNumberOfRecords) {

	this->file = new File(path, File::NEW | File::IO | File::BIN);
	this->maxNumberOfRecords = maxNumberOfRecords;
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
void HashTable<T, bucketSize>::load(File & file) {

}

template<class T, unsigned int bucketSize>
void HashTable<T, bucketSize>::insert(T & record) {
	unsigned int rehashingCount = 0;

	unsigned int jump = insertRecord(record, this->hashFunction, 0);

	if (jump != 0) {
		if (rehashingCount < maxNumberOfRehashes) {
			rehashingCount++;
			insertRecord(record, this->rehashFunction, jump);
		} else {
			throw new RehashCountException();
		}
	}
}

template<class T, unsigned int bucketSize>
unsigned int HashTable<T, bucketSize>::insertRecord(T & record,
		Function<T> * function, unsigned int jump) {
	Bucket<bucketSize> * bucket = new Bucket<bucketSize> ();
	unsigned int position = (function->hash(record.getKey()) + jump)-1;
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
			delete (recordFromBucket);

			if (recordFromBucket->getKey() > record.getKey()) //NO ESTA! Porque los registros estan almacenados en orden
				break;

			if (recordFromBucket->getKey() == record.getKey())
				throw new UniqueViolationException();
		}

		//VERIFICAR EL FACTOR DE CARGA DEL BUCKET
		unsigned int effectiveSizeForRecords = bucketSize - 5;

		if ((bucket->freeSpace - record.size()) > effectiveSizeForRecords * (1
				- BUCKET_LOAD_FACTOR)) {
			//No existe en el bucket debemos insertar (respetando el orden);
			bufferedRecords.push_front(record);

			bufferedRecords.sort();
			typename std::list<T>::iterator recordsIterator;

			//delete [] bucket->bytes; //REVISAR ESTO, TENGO Q VACIAR ANTES DE VOLVER A ESCRIBIR ?
			//FREE ARRAY

			bucket->freeSpace -= record.size();
			bucket->count++;

			for(; recordsIterator != bufferedRecords.end();recordsIterator++){
				record.write(&buffer);
			}

			file->seek(insertOffset, File::BEG);
			file->write((char *) bucket, sizeof(bucket));
			file->flush();

			delete (bucket);

			return 0;
		} else {
			return position;
		}

	} else { //hubo overflow, retornara con el ultimo resultado de la funcion de hash
		delete (bucket);
		return position;
	}
}

template<class T, unsigned int bucketSize>
T * HashTable<T, bucketSize>::get(const typename T::Key & key) {
	Bucket<bucketSize> * bucket = new Bucket<bucketSize> ();
	unsigned int offset;
	unsigned int rehashingCount = 0;
	unsigned int position = hashFunction->hash(key);
	char * buffer;

	while (rehashingCount < maxNumberOfRehashes) {
		if (rehashingCount > 0) {
			position = rehashFunction->hash(key);
		}

		offset = (position * bucketSize);

		file->seek(offset, File::BEG);
		file->read((char *) bucket, bucketSize);
		buffer = bucket->bytes;

		//Salto los bytes de control
		file->seek(5, File::CUR);

		for (int i = 0; i < bucket->count; i++) {
			T * recordFromBucket = new T(&buffer);

			if (recordFromBucket->getKey() > key) {
				if (!bucket->overflow) {
					throw new RecordNotFoundException();
				} else {
					rehashingCount++;
					break;
				}
			}

			if (recordFromBucket->getKey() == key)
				return recordFromBucket;
		}

		delete (bucket);
		delete (buffer);
	}

	throw new RecordNotFoundException();
}

template<class T, unsigned int bucketSize>
void HashTable<T, bucketSize>::remove(T & record) {
	Bucket<bucketSize> * bucket = new Bucket<bucketSize> ();
	unsigned int offset;
	unsigned int rehashingCount = 0;
	unsigned int position = hashFunction->hash(record.getKey());
	char * buffer;
	typename std::list<T> bufferedRecords;

	while (rehashingCount < maxNumberOfRehashes) {
		if (rehashingCount > 0) {
			position = rehashFunction->hash(record.getKey());
		}

		offset = (position * bucketSize);

		file->seek(offset, File::BEG);
		file->read((char *) bucket, bucketSize);
		buffer = bucket->bytes;

		//Salto los bytes de control
		file->seek(5, File::CUR);

		for (int i = 0; i < bucket->count; i++) {
			T * recordFromBucket = new T(&buffer);

			if (recordFromBucket->getKey() > record.getKey()) {
				if (!bucket->overflow) {
					throw new RecordNotFoundException();
				} else {
					rehashingCount++;
					break;
				}
			}

			if (recordFromBucket->getKey() != record.getKey())
				bufferedRecords.push_front(record);
		}

		bucket->freeSpace += record->size();
		bucket->count--;

		delete[] buffer; //REVISAR ESTO, TENGO Q VACIAR ANTES DE VOLVER A ESCRIBIR ?

		typename std::list<T>::iterator recordsIterator;

		while (recordsIterator != bufferedRecords.end()) {
			record.write(&buffer);
		}

		file->seek(offset, File::BEG);
		file->write((char *) bucket, sizeof(bucket));
		file->flush();

		delete (bucket);
		delete (buffer);
	}
}

template<class T, unsigned int bucketSize>
unsigned int HashTable<T, bucketSize>::getSize() {
	return this->size;
}

#endif /* HASHTABLE_H_ */

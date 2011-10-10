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
#include "Uint16Key.h"
#include "Record.h"
#include "HashTableExceptions.h"

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

	int insertRecord(T record,
					unsigned int* (*hashFunction)(typename T::Key * key),
					unsigned int jump);

	unsigned int hash(typename T::Key * key);

	unsigned int rehash(typename T::Key * key);

public:

	/**
	 * Primitiva de creación
	 *
	 * @file
	 * @recordsPerBucket numero de registros por cubeta
	 * @maxNumberOfRecords numero maximo de registros en el archivo (cuando se llegue a este numero se debera
	 * enfrentar una reorganización de la tabla).
	 */
	HashTable(File & file, const unsigned int recordsPerBucket,const unsigned int maxNumberOfRecords);

	void load(File & file);

	void insert(T & record);

	T get(typename T::Key * key);

	void remove(T & record);

	unsigned int getSize();

};

template<class T, unsigned int bucketSize>
HashTable<T, bucketSize>::HashTable(File & file,
		const unsigned int recordsPerBucket,
		const unsigned int maxNumberOfRecords) {

	this->file = &file;
	this->maxNumberOfRecords = maxNumberOfRecords;
	this->size = maxNumberOfRecords / (PACKAGE_DENSITY * recordsPerBucket);

	if (!MathUtils::isPrime(size)) {
		this->size = MathUtils::nextPrime(size);
	}

	//por propiedad de los numeros primos luego de size rehashes se vuelve a repetir el primer resultado.
	this->maxNumberOfRehashes = size;

	Bucket<bucketSize> * bucket = new Bucket<bucketSize>();
	bucket->freeSpace = sizeof(bucket->bytes);
	bucket->count = 0;
	bucket->overflow = false;

	for (unsigned int i = 0; i < size; i++) {
		file.write(bucket, bucketSize);
	}

	file.flush();

	delete (bucket);
	//delete[] buckets;
}

template<class T, unsigned int bucketSize>
void HashTable<T, bucketSize>::load(File & file) {

}

template<class T, unsigned int bucketSize>
void HashTable<T, bucketSize>::insert(T & record) {
	unsigned int rehashingCount = 0;

	int jump = insertRecord(record, hash, 0);

	if (jump != 0) {
		if (rehashingCount < maxNumberOfRehashes) {
			rehashingCount++;
			insertRecord(record, rehash, jump);
		} else {
			throw new RehashCountException();
		}
	}
}

template<class T, unsigned int bucketSize>
int HashTable<T, bucketSize>::insertRecord(T record,
		unsigned int * (*hashFunction)(typename T::Key * key), unsigned int jump) {
	Bucket<bucketSize> * bucket;
	unsigned int position = hashFunction(record.getKey()) + jump;
	unsigned int insertOffset = (position * bucketSize);

	file->seek(insertOffset, File::BEG);
	file->read(bucket, bucketSize);
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

			if (recordFromBucket > record) //NO ESTA! Porque los registros estan almacenados en orden
				break;

			if (recordFromBucket == record)
				throw new UniqueViolationException();
		}

		//VERIFICAR EL FACTOR DE CARGA DEL BUCKET
		unsigned int effectiveSizeForRecords = bucketSize - 5;

		if ((bucket->freeSpace - record.size()) > effectiveSizeForRecords
				* BUCKET_LOAD_FACTOR) {
			//No existe en el bucket debemos insertar (respetando el orden);
			bufferedRecords.push_front(record);

			//Los records tienen los operadores sobrecargados deberia andar bien el sort nativo
			bufferedRecords.sort();
			typename std::list<T>::iterator recordsIterator;

			delete[] buffer; //REVISAR ESTO, TENGO Q VACIAR ANTES DE VOLVER A ESCRIBIR ?

			while (recordsIterator != bufferedRecords.end()) {
				bucket->freeSpace -= record->size();
				bucket->count++;
				record.write(&buffer);
			}

			file->seek(insertOffset, File::BEG);
			file->write(bucket, sizeof(bucket));
			file->flush();

			delete (bucket);

			return 0;
		}

	} else { //hubo overflow, retornara con el ultimo resultado de la funcion de hash
		delete (bucket);
		return position;
	}
}

template<class T, unsigned int bucketSize>
T HashTable<T, bucketSize>::get(typename T::Key * key) {
	Bucket<bucketSize> * bucket;
	unsigned int offset;
	unsigned int rehashingCount = 0;
	unsigned int position = hash(key);
	char * buffer;

	while (rehashingCount < maxNumberOfRehashes) {
		if (rehashingCount > 0) {
			position = rehash(key);
		}

		offset = (position * bucketSize);

		file->seek(offset, File::BEG);
		file->read(bucket, bucketSize);
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
}

template<class T, unsigned int bucketSize>
void HashTable<T, bucketSize>::remove(T & record) {
	Bucket<bucketSize> * bucket;
	unsigned int offset;
	unsigned int rehashingCount = 0;
	unsigned int position = hash(record.getKey());
	char * buffer;
	typename std::list<T> bufferedRecords;

	while (rehashingCount < maxNumberOfRehashes) {
		if (rehashingCount > 0) {
			position = rehash(record.getKey());
		}

		offset = (position * bucketSize);

		file->seek(offset, File::BEG);
		file->read(bucket, bucketSize);
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

		delete[] buffer;//REVISAR ESTO, TENGO Q VACIAR ANTES DE VOLVER A ESCRIBIR ?

		typename std::list<T>::iterator recordsIterator;

		while (recordsIterator != bufferedRecords.end()) {
			record.write(&buffer);
		}

		file->seek(offset, File::BEG);
		file->write(bucket, sizeof(bucket));
		file->flush();

		delete (bucket);
		delete (buffer);
	}
}

template<class T, unsigned int bucketSize>
unsigned int HashTable<T, bucketSize>::hash(typename T::Key * key) {
	Uint16Key * integerKey = dynamic_cast<Uint16Key *> (&key);
	return integerKey->getKey() % size;
}

template<class T, unsigned int bucketSize>
unsigned int HashTable<T, bucketSize>::rehash(typename T::Key * key) {
	Uint16Key * integerKey = dynamic_cast<Uint16Key *> (&key);
	return (integerKey->getKey() + 1) % size;;
}

template<class T, unsigned int bucketSize>
unsigned int HashTable<T, bucketSize>::getSize() {
	return this->size;
}

#endif /* HASHTABLE_H_ */

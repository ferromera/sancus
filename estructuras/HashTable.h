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
#include "../utils/Logger.h"
#include "../utils/StringUtils.h"

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

	int insertRecord(const T & record, Function<T> * function,
			unsigned int jump);

	std::string keyToString(typename T::Key key);

	T* recordFound;

	unsigned int searchBucketIndex;

	unsigned int searchRecordIndex;

public:

	/**
	 * Primitiva de creación
	 *
	 * @file
	 * @recordsPerBucket numero de registros por cubeta
	 * @maxNumberOfRecords numero maximo de registros en el archivo (cuando se llegue a este numero se debera
	 * enfrentar una reorganización de la tabla).
	 */
	HashTable(const std::string & path, const unsigned int recordsPerBucket,
			const unsigned int maxNumberOfRecords);

	/**
	 * Primitiva de carga
	 *
	 * @path el path a un archivo directo existente (se supone que es un archivo valido)
	 */
	HashTable(const std::string & path);

	~HashTable();

	/**
	 * Primitiva de insercion
	 *
	 * @record el registro que se quiere insertar
	 * @throws RehashCountException cuando se supera la cantidad maxima de rehashes
	 */
	void insert(const T & record);

	/**
	 * Primitiva de recuperacion
	 *
	 * @key la clave del registro que se quiere recuperar
	 * @returns una copia del registro con la clava dada
	 * @throws RecordNotFoundException cuando no se puede recuperar el registro
	 */
	const T & get(const typename T::Key & key);

	/**
	 * Primitiva de actualizacion
	 *
	 * @key El registro con las modificaciones que se quieren persistir
	 * @throws RecordNotFoundException cuando no existe el registro a actualizar
	 */
	void update(const T & record);

	/**
	 * Primitiva de borrado
	 *
	 * @record el registro que se quiere borrar
	 * @throws RecordNotFoundException cuando no se puede recuperar el registro
	 */
	void remove(const T & record);

	/**
	 * @returns la cantidad de "filas" de la tabla
	 */
	unsigned int getSize();

	/**
	 * @returns
	 */
	void positionateAtBegining();

	/**
	 * @returns devuelve el proximo registro
	 */
	const T& next();
};

template<class T, unsigned int bucketSize>
HashTable<T, bucketSize>::HashTable(const std::string & path,
		const unsigned int recordsPerBucket,
		const unsigned int maxNumberOfRecords) {

	this->file = new File(path, File::NEW | File::IO | File::BIN);
	this->size = maxNumberOfRecords / (PACKAGE_DENSITY * recordsPerBucket);

	this->recordFound = NULL;
	this->searchBucketIndex = 0;
	this->searchRecordIndex = 0;

	if (!MathUtils::isPrime(size)) {
		this->size = MathUtils::nextPrime(size);
	}

	this->hashFunction = new HashFunction<T>(this->size);
	this->rehashFunction = new ReHashFunction<T>(this->size);

	//por propiedad de los numeros primos luego de size rehashes se vuelve a repetir el primer resultado.
	this->maxNumberOfRehashes = this->size;

	Bucket<bucketSize> * bucket = new Bucket<bucketSize>();
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
HashTable<T, bucketSize>::HashTable(const std::string & path) {
	this->file = new File(path, File::IO | File::BIN);
	this->file->seek(0, File::END);
	this->recordFound = NULL;
	this->searchBucketIndex = 0;
	this->searchRecordIndex = 0;
	this->size = file->tell() / bucketSize;
	this->maxNumberOfRehashes = this->size;
	this->hashFunction = new HashFunction<T>(this->size);
	this->rehashFunction = new ReHashFunction<T>(this->size);
}

template<class T, unsigned int bucketSize>
void HashTable<T, bucketSize>::insert(const T & record) {
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
int HashTable<T, bucketSize>::insertRecord(const T & record,
		Function<T> * function, unsigned int jump) {
	Bucket<bucketSize> * bucket = new Bucket<bucketSize>();
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

				Logger::getInstance()->error(
						"No se puede insertar el record "
								+ keyToString(record.getKey()) + " ya existe");

				throw UniqueViolationException();
			}
		}

		//VERIFICAR EL FACTOR DE CARGA DEL BUCKET
		unsigned int effectiveSizeForRecords = bucketSize - 5;

		if ((bucket->freeSpace - record.size())
				> effectiveSizeForRecords * (1 - BUCKET_LOAD_FACTOR)) {

			//No existe en el bucket debemos insertar (respetando el orden);
			bufferedRecords.push_front(record);

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

			Logger::getInstance()->info(
					"Se inserto el record con clave "
							+ keyToString(record.getKey()) + " en el bucket "
							+ StringUtils::intToString(position));

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
const T & HashTable<T, bucketSize>::get(const typename T::Key & key) {
	Bucket<bucketSize> * bucket = new Bucket<bucketSize>();
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

				Logger::getInstance()->error(
						"El record con clave " + keyToString(key)
								+ " no existe.");

				throw RecordNotFoundException();
			} else {
				rehashingCount++;
			}
		}

		for (int i = 0; i < bucket->count; i++) {
			delete (recordFound);
			recordFound = new T(&buffer);

			if (recordFound->getKey() > key) {
				if (!bucket->overflow) {
					Logger::getInstance()->error(
							"El record con clave " + keyToString(key)
									+ " no existe.");
					throw RecordNotFoundException();
				} else {
					rehashingCount++;
					break;
				}
			}

			if (recordFound->getKey() == key) {
				//REVISAR!!
				//T recordCopy = *recordFromBucket;
				delete (bucket);
				return *recordFound;
			}
		}

		delete (bucket);
	}

	Logger::getInstance()->error(
			"El record con clave " + keyToString(key) + " no existe.");

	throw RecordNotFoundException();
}

template<class T, unsigned int bucketSize>
void HashTable<T, bucketSize>::remove(const T & record) {
	Bucket<bucketSize> * bucket = new Bucket<bucketSize>();
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
				Logger::getInstance()->error(
						"El record con clave " + keyToString(record.getKey())
								+ " no existe.");
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
					Logger::getInstance()->error(
							"El record con clave "
									+ keyToString(record.getKey())
									+ " no existe.");
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

			Logger::getInstance()->info(
					"Se removio el record con clave "
							+ keyToString(record.getKey()) + " en el bucket "
							+ StringUtils::intToString(position));

			delete (bucket);
			break;
		}
	}
}

template<class T, unsigned int bucketSize>
void HashTable<T, bucketSize>::update(const T & record) {
	Bucket<bucketSize> * bucket = new Bucket<bucketSize>();
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
				bucket->freeSpace += recordFromBucket->size();
				found = true;
			}
		}

		if (found) {
			//VERIFICAR EL FACTOR DE CARGA DEL BUCKET
			unsigned int effectiveSizeForRecords = bucketSize - 5;

			if ((bucket->freeSpace - record.size())
					> effectiveSizeForRecords * (1 - BUCKET_LOAD_FACTOR)) {

				//insertarmos el bucket con las actualizaciones (respetando el orden);
				bufferedRecords.push_front(record);

				bufferedRecords.sort();
				typename std::list<T>::iterator recordsIterator;

				//free bucket bytes
				for (unsigned int i = 0; i < sizeof(bucket->bytes); i++) {
					bucket->bytes[i] = 0;
				}

				buffer = bucket->bytes;
				bucket->freeSpace -= record.size();

				recordsIterator = bufferedRecords.begin();

				while (recordsIterator != bufferedRecords.end()) {
					T r = (*recordsIterator);
					r.write(&buffer);
					recordsIterator++;
				}

				file->seek(offset, File::BEG);
				file->write((char *) bucket, bucketSize);
				file->flush();

				Logger::getInstance()->info(
						"Se actualizo el record con clave "
								+ keyToString(record.getKey()) + " no existe.");

				delete (bucket);
				break;
			} else {
				bucket->overflow = true;

				file->seek(offset, File::BEG);
				file->write((char *) bucket, bucketSize);
				file->flush();
			}
		}
	}
}

template<class T, unsigned int bucketSize>
unsigned int HashTable<T, bucketSize>::getSize() {
	return this->size;
}
template<class T, unsigned int bucketSize>
void HashTable<T, bucketSize>::positionateAtBegining() {
	searchBucketIndex = 0;
	searchRecordIndex = 0;
}
template<class T, unsigned int bucketSize>
const T& HashTable<T, bucketSize>::next() {
	if (searchBucketIndex >= size)
		throw RecordNotFoundException();

	Bucket<bucketSize> * bucket = new Bucket<bucketSize>();
	file->seek(searchBucketIndex * bucketSize, File::BEG);
	file->read(bucket, bucketSize);

	while (bucket->count == 0 || searchRecordIndex >= bucket->count) {
		searchBucketIndex++;
		if (searchBucketIndex >= size)
			throw RecordNotFoundException();

		searchRecordIndex = 0;
		file->seek(searchBucketIndex * bucketSize, File::BEG);
		file->read(bucket, bucketSize);
	}

	char * buffer = bucket->bytes;
	for (unsigned int i = 0; i < searchRecordIndex; i++) {
		delete (recordFound);
		recordFound = new T(&buffer);
	}

	delete (recordFound);
	recordFound = new T(&buffer);
	searchRecordIndex++;
	delete (bucket);
	return *recordFound;

}

template<class T, unsigned int bucketSize>
HashTable<T, bucketSize>::~HashTable() {
	delete (hashFunction);
	delete (rehashFunction);
	delete (file);
}

template<class T, unsigned int bucketSize>
std::string HashTable<T, bucketSize>::keyToString(typename T::Key key) {

	if (!T::Key::isString) {
		return StringUtils::intToString(key.getUint());
	} else {
		return key.getString();
	}
}

#endif /* HASHTABLE_H_ */

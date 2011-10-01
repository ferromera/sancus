#include "HashTable.h"

template<class T, unsigned int bucketSize>
HashTable<T, bucketSize>::HashTable(File & file,
		const unsigned int recordsPerBucket,
		const unsigned int maxNumberOfRecords) {

	this->file = file;
	this->maxNumberOfRecords = maxNumberOfRecords;
	this->size = maxNumberOfRecords / (PACKAGE_DENSITY * recordsPerBucket);

	for (int i = 0; i < size; i++) {
		table[i] = -1;
	}

	this->offset_to_file_data = sizeof(table);
}

template<class T, unsigned int bucketSize>
void HashTable<T, bucketSize>::insert(T & record) {

	unsigned int rehashingCount = 0;

	int jump = insertRecord(record, hash, 0);

	if (jump != 0) {

		if (rehashingCount < MAX_NUMBER_OF_REHASHES) {
			rehashingCount++;
			insertRecord(record, rehash, jump);
		} else {
			throw new RehashCountException();
		}
	}

}

template<class T, unsigned int bucketSize>
int HashTable<T, bucketSize>::insertRecord(T record,
		int* (*hashFunction)(Record::Key * key),
		int jump) {

	Bucket<bucketSize> * bucket;
	unsigned int position = hashFunction(record.getKey()) + jump;
	unsigned int insertOffset = (position * bucketSize) + offset_to_file_data;

	//CASO DONDE INSERTO y no habia ningun bucket
	if (table[position] == -1) {
		char * buffer = bucket->bytes;
		bucket->freeSpace -= record->size();
		bucket->count = 1;

		record.write(&buffer);
		file.seek(offset_to_file_data, File::BEG);
		file.write(bucket, sizeof(bucket));

		table[position] = insertOffset;

		return 0;

	} else {
		//EXISTE UN BUCKET y debo validar que se cumpla la unicidad
		file.seek(table[position], File::BEG);
		file.read(bucket, bucketSize);

		char * buffer = bucket->bytes;

		//Salto los bytes de control
		file.seek(5, File::CUR);

		typename std::list<T> bufferedRecords;

		for (int i = 0; i < bucket->count; i++) {
			T * recordFromBucket = new T(&buffer);

			bufferedRecords.push_back(*recordFromBucket);
			delete (recordFromBucket);

			if (recordFromBucket > record) {
				//NO ESTA! Porque los registros estan almacenados en orden
				break;
			}

			if (recordFromBucket == record) {
				//no se cumpla la unicidad -> TIRAR EXCEPTION
				throw new UniqueViolationException();
			}
		}

		//VERIFICAR EL FACTOR DE CARGA DEL BUCKET
		unsigned int effectiveSizeForRecords = bucketSize - 5;

		if ((bucket->freeSpace - record.size())
				> effectiveSizeForRecords * BUCKET_LOAD_FACTOR) {
			//No existe en el bucket debemos insertar (respetando el orden);
			bufferedRecords.push_front(record);

			//ORDENAR LA LISTA!
			typename std::list<T>::iterator recordsIterator;

			bucket->count = 0;

			while (recordsIterator != bufferedRecords.end()) {
				char * buffer = bucket->bytes;

				bucket->freeSpace -= record->size();
				bucket->count++;
				record.write(&buffer);
			}

			file.seek(insertOffset, File::BEG);
			file.write(bucket, sizeof(bucket));
			table[position] = insertOffset;

			return 0;

		} else {
			//hubo overflow, retornara con el ultimo resultado de la funcion de hash
			return position;
		}
	}
}

template<class T, unsigned int bucketSize>
void HashTable<T, bucketSize>::load(File & file) {
}

template<class T, unsigned int bucketSize>
int HashTable<T, bucketSize>::hash(Record::Key * key) {
	return 12;
}

template<class T, unsigned int bucketSize>
int HashTable<T, bucketSize>::rehash(Record::Key * key) {
	return 12;
}




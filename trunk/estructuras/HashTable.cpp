#include "HashTable.h"

template<class T, unsigned int bucketSize>
HashTable<T, bucketSize>::HashTable(File & file,
		const unsigned int recordsPerBucket,
		const unsigned int maxNumberOfRecords) {

	this->file = file;
	this->maxNumberOfRecords = maxNumberOfRecords;
	this->size = maxNumberOfRecords / (PACKAGE_DENSITY * recordsPerBucket);

	if(!MathUtils::isPrime(size)){
		this->size = MathUtils::nextPrime(size);
	}

	//por propiedad de los numeros primos luego de size rehashes se vuelve a repetir el primer resultado.
	this->maxNumberOfRehashes = size;

	Bucket<bucketSize> * bucket;
	bucket->freeSpace = sizeof(bucket->freeSpace);
	bucket->count = 0;
	bucket->overflow = false;

	//Cambiar esto para que escriba todo de una sola vez
	for(int i=0; i<size; i++ ){
		file.write(bucket, sizeof(bucket));
	}

	delete(bucket);
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
		int* (*hashFunction)(Record::Key * key), int jump) {
	Bucket<bucketSize> * bucket;
	unsigned int position = hashFunction(record.getKey()) + jump;
	unsigned int insertOffset = (position * bucketSize);

	file.seek(insertOffset, File::BEG);
	file.read(bucket, bucketSize);
	char * buffer = bucket->bytes;

	if (!bucket->overflow) {
		//Hay un bucket disponible sin overflow y debo validar que se cumpla la unicidad

		//Salto los bytes de control
		file.seek(5, File::CUR);

		typename std::list<T> bufferedRecords;

		for (int i = 0; i < bucket->count; i++) {
			T * recordFromBucket = new T(&buffer);

			bufferedRecords.push_back(*recordFromBucket);
			delete (recordFromBucket);

			if (recordFromBucket > record)  //NO ESTA! Porque los registros estan almacenados en orden
				break;

			if (recordFromBucket == record)
				throw new UniqueViolationException();
		}

		//VERIFICAR EL FACTOR DE CARGA DEL BUCKET
		unsigned int effectiveSizeForRecords = bucketSize - 5;

		if ((bucket->freeSpace - record.size()) > effectiveSizeForRecords * BUCKET_LOAD_FACTOR) {
			//No existe en el bucket debemos insertar (respetando el orden);
			bufferedRecords.push_front(record);

			//ORDENAR LA LISTA!
			typename std::list<T>::iterator recordsIterator;

			while (recordsIterator != bufferedRecords.end()) {
				bucket->freeSpace -= record->size();
				bucket->count++;
				record.write(&buffer);
			}

			file.seek(insertOffset, File::BEG);
			file.write(bucket, sizeof(bucket));

			delete(bucket);

			return 0;
		}

	}else{ //hubo overflow, retornara con el ultimo resultado de la funcion de hash
		delete(bucket);
		return position;
	}
}

template<class T, unsigned int bucketSize>
void HashTable<T, bucketSize>::load(File & file) {
}

template<class T, unsigned int bucketSize>
int HashTable<T, bucketSize>::hash(Record::Key * key) {
	Uint16Key * integerKey = dynamic_cast<Uint16Key *> (&key);
	return integerKey->getKey() % size;
}

template<class T, unsigned int bucketSize>
int HashTable<T, bucketSize>::rehash(Record::Key * key) {
	Uint16Key * integerKey = dynamic_cast<Uint16Key *> (&key);
	return (integerKey->getKey() + 1) % size;;
}



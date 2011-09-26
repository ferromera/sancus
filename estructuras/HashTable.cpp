#include "HashTable.h"

template <class T>
HashTable<T>::HashTable(const File & file, const unsigned int recordsPerBucket, const unsigned int maxNumberOfRecords){
	this->file = file;
	this->bucketSize = (sizeof(T)*recordsPerBucket)+3;
	this->maxNumberOfRecords = maxNumberOfRecords;
	this->size = maxNumberOfRecords / (PACKAGE_DENSITY * bucketSize);

	for (int i=0; i< size ; i++){
		table[i] = -1;
	}
}


template <class T>
void HashTable<T>::load(File & file){
	char buffer[bucketSize];
	//while(!feof(file))
	file.read(&buffer,bucketSize);

}


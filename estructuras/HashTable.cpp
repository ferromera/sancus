#include "HashTable.h"

template <class T>
HashTable<T>::HashTable(const RandomAccessFile<T> & randomAccessFile, const int bucketSize,const  int numberOfRecords){

	this->size = numberOfRecords / (PACKAGE_DENSITY * bucketSize);

	for (int i=0; i<this->size ; i++){
		this->table[i] = -1;
	}

	this->file = randomAccessFile.getFile();
}

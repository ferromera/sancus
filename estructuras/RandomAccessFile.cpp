#include "RandomAccessFile.h"


template <class T>
RandomAccessFile<T>::RandomAccessFile(const File & file, const int bucketSize){
	this->file = file;
	this->table = new HashTable<T>(this,bucketSize,10);
}

template <class T>
File & RandomAccessFile<T>::getFile(){
	return this->file;
}

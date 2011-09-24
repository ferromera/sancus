#include "HashTable.h"

template <class T>
HashTable<T>::HashTable(const RandomAccessFile<T> & randomAccessFile){
	this->size = 1;
	this->file = randomAccessFile.getFile();
}

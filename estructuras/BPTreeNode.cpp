
#include "BPTreeNode.h"
using namespace std;

template<class Record,unsigned int blockSize>
BPTreeNode<Record,blockSize>::BPTreeNode():count_(0),freeSpace_(blockSize-2){}

template<class Record,unsigned int blockSize>
void BPTreeNode<Record,blockSize>::load(File & file,unsigned long pos){
	// Setea el archivo y la posición en el archivo asociada al nodo.
	// y carga el nodo leyendolo desde el archivo.
	file_=file;
	pos_=pos;
	read();
}

template<class Record,unsigned int blockSize>
unsigned int BPTreeNode<Record,blockSize>::create(File & file){
	//Crea un nuevo bloque en el archivo con los datos del Nodo.

	file_=file;
	FreeSpaceStackBlock<blockSize> *freeBlock;//= new FreeSpaceStackBlock<size>;
	file_.seek(0,File::BEG);
	file_.read((char *)freeBlock,blockSize);
	unsigned int blockNumber=freeBlock->blockNumber;
	pos_=((unsigned long)blockNumber)*blockSize;
	if(freeBlock->inFile){
		file_.seek(pos_,File::BEG);
		file_.read((char *)freeBlock,blockSize);
		file_.seek(0,File::BEG);
		file_.write((char *)freeBlock,blockSize);
	}else{
		freeBlock->blockNumber++;
		file_.write((char *)freeBlock,blockSize);
	}
	delete freeBlock;
	write();
	return blockNumber;
}


template<class Record,unsigned int blockSize>
unsigned int BPTreeNode<Record,blockSize>::level()const{
    return level_;
}

template<class Record,unsigned int blockSize>
unsigned int BPTreeNode<Record,blockSize>::count()const{
    return count_;
}

template<class Record,unsigned int blockSize>
unsigned long BPTreeNode<Record,blockSize>::pos()const{
   return pos_;
}

template<class Record,unsigned int blockSize>
void BPTreeNode<Record,blockSize>::level(unsigned int l){
    level_=l;
}

template<class Record,unsigned int blockSize>
void BPTreeNode<Record,blockSize>::count(unsigned int c){
    count_=c;
}

template<class Record,unsigned int blockSize>
void BPTreeNode<Record,blockSize>::file(File & f){
    file_=f;
}

template<class Record,unsigned int blockSize>
void BPTreeNode<Record,blockSize>::pos(unsigned long p){
    pos_=p;
}

template<class Record,unsigned int blockSize>
BPTreeNode<Record,blockSize>::~BPTreeNode(){
}

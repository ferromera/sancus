
#include "BPTreeNode.h"
using namespace std;

template<class Record>
BPTreeNode<Record>::BPTreeNode(unsigned int blockSize):count_(0),blockSize_(blockSize){}

template<class Record>
BPTreeNode<Record>::load(fstream & file,unsigned int pos):
file_(file),pos_(pos){
	// Setea el archivo y la posición en el archivo asociada al nodo.
	// y carga el nodo leyendolo desde el archivo.
	read();
}

template<class Record>
BPTreeNode<Record>::create(fstream & file):file_(file){
	//Crea un nuevo bloque en el archivo con los datos del Nodo.
	FreeSpaceStackBlock<blockSize> *freeBlock= new FreeSpaceStackBlock<blockSize>;
	file_.seekg(0,ios_base::beg);
	file_.read((char *)freeBlock,blockSize);
	pos_=(freeBlock->blockNumber)*blockSize;
	if(freeBlock->inFile){
		file_.seekg(pos_,ios_base::beg);
		file_.read((char *)freeBlock,blockSize);
		file_.seekg(0,ios_base::beg);
		file_.write((char *)freeBlock,blockSize);
	}else{
		freeBlock->blockNumber++;
		file_.write((char *)freeBlock,blockSize);
	}
	delete freeBlock;
	write();
	return pos_;
}


template<class Record>
unsigned int BPTreeNode<Record>::level()const{
    return level_;
}
template<class Record>
unsigned int BPTreeNode<Record>::count()const{
    return count_;
}
template<class Record>
unsigned int BPTreeNode<Record>::pos()const{
    return pos_;
}
template<class Record>
void BPTreeNode<Record>::level(unsigned int l){
    level_=l;
}
template<class Record>
void BPTreeNode<Record>::count(unsigned int c){
    count_=c;
}
template<class Record>
void BPTreeNode<Record>::file(std::fstream & f){
    file_=f;
}
template<class Record>
void BPTreeNode<Record>::pos(unsigned int p){
    pos_=p;
}
template<class Record>
BPTreeNode<Record>::~BPTreeNode(){
}

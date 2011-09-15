/*
 * BPTreeVariableLeaf.cpp
 *
 *  Created on: 14/09/2011
 *      Author: fernando
 */

#include "BPTreeVariableLeaf.h"

using namespace std;

template<class Record,unsigned int blockSize>
BPTreeVariableLeaf<Record,blockSize>::BPTreeVariableLeaf(unsigned int capacity,File & file):
BPTreeLeaf<Record,blockSize>(capacity,file){
}

template<class Record,unsigned int blockSize>
BPTreeVariableLeaf<Record,blockSize>::BPTreeVariableLeaf(unsigned int capacity,File & file,unsigned int pos):
BPTreeLeaf<Record,blockSize>(capacity,file,pos){
}

template<class Record,unsigned int blockSize>
void BPTreeVariableLeaf<Record,blockSize>::read(){
	BPTreeVariableLeafBlock<blockSize> * block= new BPTreeVariableLeafBlock<blockSize>;
	file_.seek(pos_,File::BEG);
	file_.read((char*)block,blockSize);
	count_=0;
	while(block->nestedBlocks!=0){
		count_+=block->count;
		blockNumbers.push_back(block->next);
		readRecords(block);
		unsigned long nextPos=((unsigned long)block->next)*blockSize;
		file_.seek(nextPos,File::BEG);
		file_.read((char*)block,blockSize);
	}
	count_+=block->count;
	freeSpace_=block->freeSpace;
	next_=block->next;
	readRecords(block);
}

template<class Record,unsigned int blockSize>
void BPTreeVariableLeaf<Record,blockSize>::readRecords(BPTreeVariableLeafBlock<blockSize> * block){
	unsigned int alreadyRead=0;
	uint16_t length;
	char * currentPos=block->bytes;
	while(alreadyRead==block->count){
		Record * newRecord=new Record(& currentPos);
		records_.push_back(*newRecord);
		delete newRecord;
		alreadyRead++;
	}

}

template<class Record,unsigned int blockSize>
void BPTreeVariableLeaf<Record,blockSize>::write(){
	// No Implementado
}

template<class Record,unsigned int blockSize>
void BPTreeVariableLeaf<Record,blockSize>::insert(Record &){
	// No Implementado
}

template<class Record,unsigned int blockSize>
void BPTreeVariableLeaf<Record,blockSize>::remove(Record &){
	// No Implementado
}
template<class Record,unsigned int blockSize>
BPTreeVariableLeaf<Record,blockSize>::~BPTreeVariableLeaf(){
}

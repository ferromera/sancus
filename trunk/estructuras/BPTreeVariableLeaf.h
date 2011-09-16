/*
 * BPTreeVariableLeaf.h
 *
 *  Created on: 14/09/2011
 *      Author: fernando
 */
#ifndef BPTREEVARIABLELEAF_H_INCLUDED
#define BPTREEVARIABLELEAF_H_INCLUDED

#include "BPTreeLeaf.h"
#include "BPTreeVariableLeafBlock.h"

template<class Record,unsigned int blockSize>
class BPTreeVariableLeaf: public BPTreeLeaf<Record,blockSize>{
	std::list<unsigned int> blockNumbers;
	std::list<unsigned int> blockCounts;
	std::list<unsigned int> freeSpaces;
	void readRecords(BPTreeVariableLeafBlock<blockSize> *);
	void writeRecords(BPTreeVariableLeafBlock<blockSize> *);
public:
	BPTreeVariableLeaf(unsigned int capacity,File & file);
	BPTreeVariableLeaf(unsigned int capacity,File & file,unsigned long pos);
	void read();
	void write();
	void insert(Record &);
	void remove(Record &);
	BPTreeLeaf<Record,blockSize> * nextLeaf();
	~BPTreeVariableLeaf();

};

template<class Record,unsigned int blockSize>
BPTreeVariableLeaf<Record,blockSize>::BPTreeVariableLeaf(unsigned int capacity,File & file):
BPTreeLeaf<Record,blockSize>(capacity,file){
	blockCounts.push_back(0);
	blockNumbers.push_back(BPTreeNode<Record,blockSize>::blockNumber_);
	BPTreeNode<Record,blockSize>::freeSpace_=blockSize-9;
	freeSpaces.push_back(blockSize-9);
	write();
}

template<class Record,unsigned int blockSize>
BPTreeVariableLeaf<Record,blockSize>::BPTreeVariableLeaf(unsigned int capacity,File & file,unsigned long blockNumber):
BPTreeLeaf<Record,blockSize>(capacity,file,blockNumber){
	read();
}

template<class Record,unsigned int blockSize>
void BPTreeVariableLeaf<Record,blockSize>::read(){
	BPTreeVariableLeafBlock<blockSize> * block= new BPTreeVariableLeafBlock<blockSize>;

	File * file=BPTreeNode<Record,blockSize>::file_;
	unsigned long & blockNumber=BPTreeNode<Record,blockSize>::blockNumber_;
	unsigned int & count=BPTreeNode<Record,blockSize>::count_;

	blockCounts.clear();
	blockNumbers.clear();
	freeSpaces.clear();
	BPTreeLeaf<Record,blockSize>::records_.clear();

	blockNumbers.push_back(blockNumber);
	unsigned long pos=blockNumber*blockSize;
	file->seek(pos,File::BEG);
	file->read((char*)block,blockSize);
	count=0;
	while(block->nestedBlocks!=0){
		blockCounts.push_back(block->count);
		count+=block->count;
		blockNumbers.push_back(block->next);
		freeSpaces.push_back(block->freeSpace);
		readRecords(block);
		unsigned long nextPos=((unsigned long)block->next)*blockSize;
		file->seek(nextPos,File::BEG);
		file->read((char*)block,blockSize);
	}
	blockCounts.push_back(block->count);
	count+=block->count;
	freeSpaces.push_back(block->freeSpace);
	BPTreeNode<Record,blockSize>::freeSpace_=block->freeSpace;
	BPTreeLeaf<Record,blockSize>::next_=block->next;
	readRecords(block);
}

template<class Record,unsigned int blockSize>
void BPTreeVariableLeaf<Record,blockSize>::readRecords(BPTreeVariableLeafBlock<blockSize> * block){
	unsigned int alreadyRead=0;
	char * currentPos=block->bytes;
	while(alreadyRead!=block->count){
		Record * newRecord=new Record(& currentPos);
		BPTreeLeaf<Record,blockSize>::records_.push_back(*newRecord);
		delete newRecord;
		alreadyRead++;
	}
}

template<class Record,unsigned int blockSize>
void BPTreeVariableLeaf<Record,blockSize>::write(){
	BPTreeVariableLeafBlock<blockSize> * block= new BPTreeVariableLeafBlock<blockSize>;
	File * file=BPTreeNode<Record,blockSize>::file_;
	std::list<unsigned int>::iterator itNumber,itCount,itFree,itLastNumber;
	itLastNumber=--(blockNumbers.end());
	unsigned int blockQuantity=blockNumbers.size();


	for(itNumber=blockNumbers.begin() , itCount=blockCounts.begin(), itFree=freeSpaces.begin()
		; itNumber!=blockNumbers.end(); itNumber++ , itCount++, itFree++){
		block->count=*itCount;
		block->freeSpace=*itFree;
		block->nestedBlocks=--blockQuantity;
		if(itNumber!=itLastNumber){
			itNumber++;
			block->next=*itNumber;
			itNumber--;
		}else
			block->next=BPTreeLeaf<Record,blockSize>::next_;
		writeRecords(block);
		file->seek((*itNumber)*blockSize,File::BEG);
		file->write(block,blockSize);
	}

}

template<class Record,unsigned int blockSize>
void BPTreeVariableLeaf<Record,blockSize>::writeRecords(BPTreeVariableLeafBlock<blockSize> * block){
	unsigned int alreadyWritten=0;
	char * currentPos=block->bytes;
	typename std::list<Record>::iterator itRecord=BPTreeLeaf<Record,blockSize>::records_.begin();

	while(alreadyWritten!=block->count){
		itRecord->write(&currentPos);
		alreadyWritten++;
		itRecord++;
	}
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
BPTreeLeaf<Record,blockSize>* BPTreeVariableLeaf<Record,blockSize>::nextLeaf( ){
	// No Implementado
}
template<class Record,unsigned int blockSize>
BPTreeVariableLeaf<Record,blockSize>::~BPTreeVariableLeaf(){
}

#endif //BPTREEVARIABLELEAF_H

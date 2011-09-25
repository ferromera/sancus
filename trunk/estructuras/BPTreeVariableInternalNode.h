/*
 * BPTreeVariableInternalNode.h
 *
 *  Created on: 24/09/2011
 *      Author: juanmromera
 */

#ifndef BPTREEVARIABLEINTERNALNODE_H_
#define BPTREEVARIABLEINTERNALNODE_H_

#include "BPTreeInternalNode.h"
#include "BPTreeVariableNodeBlock.h"
#include <cstring>

using namespace std;

template<class Record,unsigned int blockSize>
class BPTreeVariableInternalNode : public BPTreeInternalNode<Record,blockSize>{
	std::list<unsigned int> blockNumbers;
	std::list<unsigned int> blockCounts;
	std::list<unsigned int> freeSpaces;
	void readRecords( BPTreeVariableNodeBlock<blockSize> *);
	void writeRecords( BPTreeVariableNodeBlock<blockSize> *,unsigned int);
	unsigned int getBlockPosition(typename Record::Key &);
	typename std::list<typename Record::Key>::iterator getFirstRecordOfBlock(unsigned int);
	void expand();
public:
	BPTreeVariableInternalNode(unsigned int capacity,unsigned int level,File & file);
	BPTreeVariableInternalNode(unsigned int capacity,File & file,unsigned long blockNumber);

	void read();
	void write();
	void insert(Record &);
	void remove(Record &);
	virtual ~BPTreeVariableInternalNode(){}
};

template<class Record,unsigned int blockSize>
BPTreeVariableInternalNode<Record,blockSize>::BPTreeVariableInternalNode(unsigned int capacity,unsigned int level,File & file):
BPTreeInternalNode<Record,blockSize>(capacity,level,file){
	blockCounts.push_back(0);
	blockNumbers.push_back(BPTreeNode<Record,blockSize>::blockNumber_);
	freeSpaces.push_back(VARIABLE_NODE_RECORDS_SPACE);
	write();
}

template<class Record,unsigned int blockSize>
BPTreeVariableInternalNode<Record,blockSize>::BPTreeVariableInternalNode(unsigned int capacity,File & file,unsigned long blockNumber):
BPTreeInternalNode<Record,blockSize>(capacity,file,blockNumber){
	read();
}
template<class Record,unsigned int blockSize>
void BPTreeVariableInternalNode<Record,blockSize>::read(){

	BPTreeVariableNodeBlock<blockSize> * block= new BPTreeVariableNodeBlock<blockSize>;

	File * file=BPTreeNode<Record,blockSize>::file_;
	unsigned long & blockNumber=BPTreeNode<Record,blockSize>::blockNumber_;
	unsigned int & count=BPTreeNode<Record,blockSize>::count_;

	blockCounts.clear();
	blockNumbers.clear();
	freeSpaces.clear();
	BPTreeInternalNode<Record,blockSize>::keys_.clear();

	blockNumbers.push_back(blockNumber);
	unsigned long pos=blockNumber*blockSize;
	file->seek(pos,File::BEG);
	file->read((char*)block,blockSize);
	count=0;
	BPTreeNode<Record,blockSize>::level_ = block->level;
	BPTreeInternalNode<Record,blockSize>::children_.push_back(block->leftChild);

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
	readRecords(block);
}
template<class Record,unsigned int blockSize>
void BPTreeVariableInternalNode<Record,blockSize>::readRecords(BPTreeVariableNodeBlock<blockSize> *block )
{
		unsigned int alreadyRead=0;
		char * currentPos=block->bytes;
		while(alreadyRead!=block->count)
		{

			typename Record::Key * newKey = new typename Record::Key();
			newKey->read(& currentPos);
			BPTreeInternalNode<Record,blockSize>::keys_.push_back(*newKey);
			unsigned int rightChild;
			memcpy(&rightChild,currentPos,4);
			currentPos+=4;
			BPTreeInternalNode<Record,blockSize>::children_.push_back(rightChild);
			delete newKey;
			alreadyRead++;
		}
}
template<class Record,unsigned int blockSize>
void BPTreeVariableInternalNode<Record,blockSize>::write(){
	BPTreeVariableNodeBlock<blockSize> * block= new BPTreeVariableNodeBlock<blockSize>;
	File * file=BPTreeNode<Record,blockSize>::file_;
	std::list<unsigned int>::iterator itNumber,itCount,itFree,itLastNumber;
	itLastNumber=--(blockNumbers.end());
	unsigned int blockQuantity=blockNumbers.size();
	unsigned int blockPos;

	for( blockPos=0,itNumber=blockNumbers.begin() , itCount=blockCounts.begin(), itFree=freeSpaces.begin()
		; itNumber!=blockNumbers.end(); itNumber++ , itCount++, itFree++,blockPos++){
		block->count=*itCount;
		block->freeSpace=*itFree;
		block->nestedBlocks=--blockQuantity;
		block->level = BPTreeNode<Record,blockSize>::level_;
		block->leftChild = BPTreeInternalNode<Record,blockSize>::children_.front();
		if(itNumber!=itLastNumber){
			itNumber++;
			block->next=*itNumber;
			itNumber--;
		}

		writeRecords(block,blockPos);
		file->seek((*itNumber)*blockSize,File::BEG);
		file->write(block,blockSize);
	}

}
template<class Record,unsigned int blockSize>
void BPTreeVariableInternalNode<Record,blockSize>::writeRecords(BPTreeVariableNodeBlock<blockSize> * block,unsigned int blockPos){

	typename std::list<typename Record::Key>::iterator itKeys=BPTreeInternalNode<Record,blockSize>::keys_.begin();
	std::list<unsigned int>::iterator itChildren=BPTreeInternalNode<Record,blockSize>::children_.begin();
	itChildren++;
	std::list<unsigned int>::iterator itBlockCount=blockCounts.begin();
	unsigned int currBlock=0;

	for(unsigned int count=1;currBlock!=blockPos;itKeys++,count++,itChildren++)
			if(count==(*itBlockCount)){
				count=0;
				itBlockCount++;
				currBlock++;
			}

	unsigned int alreadyWritten=0;
	char * currentPos=block->bytes;
	while(alreadyWritten!=block->count){
		itKeys->write(&currentPos);
		unsigned int t = *itChildren;
		memcpy(currentPos,&(*itChildren),4);
		currentPos+=4;
		alreadyWritten++;
		itKeys++;
		itChildren++;
	}
}
template<class Record,unsigned int blockSize>
void BPTreeVariableInternalNode<Record,blockSize>::insert(Record &)
{

}
template<class Record,unsigned int blockSize>
void BPTreeVariableInternalNode<Record,blockSize>::remove(Record &)
{

}



#endif /* BPTREEVARIABLEINTERNALNODE_H_ */

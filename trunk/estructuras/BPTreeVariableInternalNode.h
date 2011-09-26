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
#include "BPTreeVariableLeaf.h"
#include <cstring>

using namespace std;

template<class Record,unsigned int blockSize>
class BPTreeVariableInternalNode : public BPTreeInternalNode<Record,blockSize>{
	std::list<unsigned int> blockNumbers;
	std::list<unsigned int> blockCounts;
	std::list<unsigned int> freeSpaces;
	void readRecords( BPTreeVariableNodeBlock<blockSize> *);
	void writeRecords( BPTreeVariableNodeBlock<blockSize> *,unsigned int);
	unsigned int getBlockPosition(typename std::list<typename Record::Key>::iterator  &);
	typename std::list<typename Record::Key>::iterator getFirstKeyOfBlock(unsigned int);
	void insertInNode(typename Record::Key & , unsigned int);
	void expand();
public:
	BPTreeVariableInternalNode(unsigned int capacity,unsigned int leafCapacity,unsigned int level,File & file);
	BPTreeVariableInternalNode(unsigned int capacity,unsigned int leafCapacity,File & file,unsigned long blockNumber);

	void read();
	void write();
	void insert(Record &);
	void remove(Record &);
	virtual ~BPTreeVariableInternalNode(){}
};

template<class Record,unsigned int blockSize>
BPTreeVariableInternalNode<Record,blockSize>::BPTreeVariableInternalNode(unsigned int capacity,unsigned int leafCapacity,unsigned int level,File & file):
BPTreeInternalNode<Record,blockSize>(capacity,leafCapacity,level,file){
	blockCounts.push_back(0);
	blockNumbers.push_back(BPTreeNode<Record,blockSize>::blockNumber_);
	freeSpaces.push_back(VARIABLE_NODE_RECORDS_SPACE);
	write();
}

template<class Record,unsigned int blockSize>
BPTreeVariableInternalNode<Record,blockSize>::BPTreeVariableInternalNode(unsigned int capacity,unsigned int leafCapacity,File & file,unsigned long blockNumber):
BPTreeInternalNode<Record,blockSize>(capacity,leafCapacity,file,blockNumber){
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
		memcpy(currentPos,&(*itChildren),4);
		currentPos+=4;
		alreadyWritten++;
		itKeys++;
		itChildren++;
	}
}
template<class Record,unsigned int blockSize>
void BPTreeVariableInternalNode<Record,blockSize>::insert(Record & record)
{
	typename Record::Key key = dynamic_cast<const typename Record::Key &>(record.getKey());
	//Busco la clave en la lista
	typename std::list<typename Record::Key>::iterator itKey =
			BPTreeInternalNode<Record,blockSize>::search(key);
	std::list<unsigned int>::iterator itChildren;
	bool entrySideLeft = true;

	//Obtengo el bloque donde se debera insertar

	if(itKey != BPTreeInternalNode<Record,blockSize>::keys_.end())
		itChildren = BPTreeInternalNode<Record,blockSize>::getLeftChild(*itKey);
	else
	{
		itKey--;
		itChildren = BPTreeInternalNode<Record,blockSize>::getRightChild(*itKey);
		entrySideLeft = false;
	}

	if(BPTreeNode<Record,blockSize>::level_ == 1)
	{
		BPTreeVariableLeaf<Record,blockSize> *childLeaf = new BPTreeVariableLeaf<Record,blockSize>(BPTreeInternalNode<Record,blockSize>::leafCapacity_,
				*BPTreeNode<Record,blockSize>::file_,*itChildren);

		try
		{
			childLeaf->insert(record);
			childLeaf->write();

		}
		catch(LeafOverflowException exception)
		{
			BPTreeVariableLeaf<Record,blockSize> *newLeaf = new BPTreeVariableLeaf<Record,blockSize>(BPTreeInternalNode<Record,blockSize>::leafCapacity_,
					*BPTreeNode<Record,blockSize>::file_);
			typename::list<Record> recordList = childLeaf->getRecords();

			unsigned int mediaList = BPTreeInternalNode<Record,blockSize>::leafCapacity_/2;
			typename::list<Record>::iterator recordIt = recordList.begin();

			for(unsigned int i = 0 ; i < mediaList;i++,recordIt++);

			if(record < *recordIt)
			{
				while(recordIt != recordList.end())
				{
					childLeaf->remove(*recordIt);
					newLeaf->insert(*recordIt);
					recordIt++;
				}
				childLeaf->insert(record);
			}
			else
			{
				recordIt++;
				while(recordIt != recordList.end())
				{
					childLeaf->remove(*recordIt);
					newLeaf->insert(*recordIt);
					recordIt++;
				}
				newLeaf->insert(record);
			}
			childLeaf->write();
			newLeaf->write();
			recordList  = newLeaf->getRecords();
			Record firstRecord = recordList.front();
			typename Record::Key newKey = dynamic_cast<const typename Record::Key &>(firstRecord.getKey());
			insertInNode(newKey,newLeaf->blockNumber());
			delete newLeaf;
		}

		delete childLeaf;

	}
	else
	{

	}


}
template<class Record,unsigned int blockSize>
void BPTreeVariableInternalNode<Record,blockSize>::insertInNode(typename Record::Key & key, unsigned int child)
{

	std::list<unsigned int>::iterator itBlockCount=blockCounts.begin();
	std::list<unsigned int>::iterator itFreeSpace=freeSpaces.begin();

	typename std::list<typename Record::Key>::iterator itKey =
			BPTreeInternalNode<Record,blockSize>::search(key);

	typename std::list<unsigned int>::iterator itChildren =
			 BPTreeInternalNode<Record,blockSize>::getRightChild(*itKey);


	//Posiciono los iteradores en el bloque donde se va a insertar el registro.
	unsigned int blockPos = getBlockPosition(itKey);

	//if(BPTreeNode<Record,blockSize>::count_ == BPTreeInternalNode<Record,blockSize>::capacity_)
	  // throw NodeOverflowException<typename Record::Key>(child,key);

	BPTreeInternalNode<Record,blockSize>::keys_.insert(itKey,key);
	BPTreeInternalNode<Record,blockSize>::children_.insert(itChildren,child);

	BPTreeNode<Record,blockSize>::count_++;

	for(unsigned int i=0;i<blockPos;i++){
		itBlockCount++;
		itFreeSpace++;
	}

	//empiezo a insertar desde el primer registro del bloque.
	typename std::list<typename Record::Key>::iterator currentKey=getFirstKeyOfBlock(blockPos);

	while(currentKey!=BPTreeInternalNode<Record,blockSize>::keys_.end()){
		(*itBlockCount)=0;
		//Tengo todo el espacio libre.
		(*itFreeSpace)=VARIABLE_NODE_RECORDS_SPACE;
		while((*itFreeSpace)>=currentKey->size()){
			//inserto el current record aumentando el count y disminuyendo el freeSpace
			(*itBlockCount)++;
			(*itFreeSpace)-=currentKey->size() + 4;
			currentKey++;
			if(currentKey==BPTreeInternalNode<Record,blockSize>::keys_.end())
				break;
		}
		if(currentKey==BPTreeInternalNode<Record,blockSize>::keys_.end())
			break;
		//Me muevo al siguiente bloque
		itFreeSpace++;
		itBlockCount++;
		if(itFreeSpace==freeSpaces.end()){
			expand();
			//Me ubico en el último bloque recién creado.
			itFreeSpace--;
			itBlockCount--;
		}

	}
}
template<class Record,unsigned int blockSize>
void BPTreeVariableInternalNode<Record,blockSize>::expand(){

	unsigned long newBlockNumber=BPTreeNode<Record,blockSize>::getFreeBlock();
	blockNumbers.push_back(newBlockNumber);
	blockCounts.push_back(0);
	freeSpaces.push_back(VARIABLE_NODE_RECORDS_SPACE);

}
template<class Record,unsigned int blockSize>
unsigned int BPTreeVariableInternalNode<Record,blockSize>::getBlockPosition(typename std::list<typename Record::Key>::iterator & itKey){
	typename std::list<typename Record::Key>::iterator it=BPTreeInternalNode<Record,blockSize>::keys_.begin();
	std::list<unsigned int>::iterator itBlockCount=blockCounts.begin();
	unsigned int blockPos=0;

	if(itKey==BPTreeInternalNode<Record,blockSize>::keys_.end())
			return blockCounts.size()-1;

	for(unsigned int count=1;it!=itKey;it++,count++)
		if(count==(*itBlockCount)){
			count=0;
			itBlockCount++;
			blockPos++;
		}

	return blockPos;
}
template<class Record,unsigned int blockSize>
typename std::list<typename Record::Key>::iterator BPTreeVariableInternalNode<Record,blockSize>::getFirstKeyOfBlock(unsigned int blockPos){
	typename std::list<typename Record::Key>::iterator itFirstRecord = BPTreeInternalNode<Record,blockSize>::keys_.begin();
	std::list<unsigned int>::iterator itBlockCount=blockCounts.begin();
	unsigned int currBlock=0;


	for(unsigned int count=1;currBlock!=blockPos;itFirstRecord++,count++)
		if(count==(*itBlockCount)){
			count=0;
			itBlockCount++;
			currBlock++;
		}
	return itFirstRecord;
}
template<class Record,unsigned int blockSize>
void BPTreeVariableInternalNode<Record,blockSize>::remove(Record &)
{

}



#endif /* BPTREEVARIABLEINTERNALNODE_H_ */

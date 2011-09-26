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

template<class TRecord,unsigned int blockSize>
class BPTreeVariableInternalNode : public BPTreeInternalNode<TRecord,blockSize>{
	std::list<unsigned int> blockNumbers;
	std::list<unsigned int> blockCounts;
	std::list<unsigned int> freeSpaces;
	void readRecords( BPTreeVariableNodeBlock<blockSize> *);
	void writeRecords( BPTreeVariableNodeBlock<blockSize> *,unsigned int);
	unsigned int getBlockPosition(typename std::list<typename TRecord::Key>::iterator  &);
	typename std::list<typename TRecord::Key>::iterator getFirstKeyOfBlock(unsigned int);

	void expand();
public:
	BPTreeVariableInternalNode(unsigned int capacity,unsigned int leafCapacity,unsigned int level,File & file);
	BPTreeVariableInternalNode(unsigned int capacity,unsigned int leafCapacity,File & file,unsigned long blockNumber);

	void read();
	void write();
	void insert(TRecord &);
	void remove(TRecord &);
	void insertInNode(typename TRecord::Key & , unsigned int);
	void removeInNode(typename TRecord::Key & , unsigned int);
	virtual ~BPTreeVariableInternalNode(){}
};

template<class TRecord,unsigned int blockSize>
BPTreeVariableInternalNode<TRecord,blockSize>::BPTreeVariableInternalNode(unsigned int capacity,unsigned int leafCapacity,unsigned int level,File & file):
BPTreeInternalNode<TRecord,blockSize>(capacity,leafCapacity,level,file){
	blockCounts.push_back(0);
	blockNumbers.push_back(BPTreeNode<TRecord,blockSize>::blockNumber_);
	freeSpaces.push_back(VARIABLE_NODE_RECORDS_SPACE);
	write();
}

template<class TRecord,unsigned int blockSize>
BPTreeVariableInternalNode<TRecord,blockSize>::BPTreeVariableInternalNode(unsigned int capacity,unsigned int leafCapacity,File & file,unsigned long blockNumber):
BPTreeInternalNode<TRecord,blockSize>(capacity,leafCapacity,file,blockNumber){
	read();
}
template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::read(){

	BPTreeVariableNodeBlock<blockSize> * block= new BPTreeVariableNodeBlock<blockSize>;

	File * file=BPTreeNode<TRecord,blockSize>::file_;
	unsigned long & blockNumber=BPTreeNode<TRecord,blockSize>::blockNumber_;
	unsigned int & count=BPTreeNode<TRecord,blockSize>::count_;

	blockCounts.clear();
	blockNumbers.clear();
	freeSpaces.clear();
	BPTreeInternalNode<TRecord,blockSize>::keys_.clear();

	blockNumbers.push_back(blockNumber);
	unsigned long pos=blockNumber*blockSize;
	file->seek(pos,File::BEG);
	file->read((char*)block,blockSize);
	count=0;
	BPTreeNode<TRecord,blockSize>::level_ = block->level;
	BPTreeInternalNode<TRecord,blockSize>::children_.push_back(block->leftChild);

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
template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::readRecords(BPTreeVariableNodeBlock<blockSize> *block )
{
		unsigned int alreadyRead=0;
		char * currentPos=block->bytes;
		while(alreadyRead!=block->count)
		{

			typename TRecord::Key * newKey = new typename TRecord::Key();
			newKey->read(& currentPos);
			BPTreeInternalNode<TRecord,blockSize>::keys_.push_back(*newKey);
			unsigned int rightChild;
			memcpy(&rightChild,currentPos,4);
			currentPos+=4;
			BPTreeInternalNode<TRecord,blockSize>::children_.push_back(rightChild);
			delete newKey;
			alreadyRead++;
		}
}
template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::write(){
	BPTreeVariableNodeBlock<blockSize> * block= new BPTreeVariableNodeBlock<blockSize>;
	File * file=BPTreeNode<TRecord,blockSize>::file_;
	std::list<unsigned int>::iterator itNumber,itCount,itFree,itLastNumber;
	itLastNumber=--(blockNumbers.end());
	unsigned int blockQuantity=blockNumbers.size();
	unsigned int blockPos;

	for( blockPos=0,itNumber=blockNumbers.begin() , itCount=blockCounts.begin(), itFree=freeSpaces.begin()
		; itNumber!=blockNumbers.end(); itNumber++ , itCount++, itFree++,blockPos++){
		block->count=*itCount;
		block->freeSpace=*itFree;
		block->nestedBlocks=--blockQuantity;
		block->level = BPTreeNode<TRecord,blockSize>::level_;
		block->leftChild = BPTreeInternalNode<TRecord,blockSize>::children_.front();
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
template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::writeRecords(BPTreeVariableNodeBlock<blockSize> * block,unsigned int blockPos){

	typename std::list<typename TRecord::Key>::iterator itKeys=BPTreeInternalNode<TRecord,blockSize>::keys_.begin();
	std::list<unsigned int>::iterator itChildren=BPTreeInternalNode<TRecord,blockSize>::children_.begin();
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
template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::insert(TRecord & record)
{
	typename TRecord::Key key = dynamic_cast<const typename TRecord::Key &>(record.getKey());
	//Busco la clave en la lista
	typename std::list<typename TRecord::Key>::iterator itKey =
			BPTreeInternalNode<TRecord,blockSize>::search(key);
	std::list<unsigned int>::iterator itChildren;
	bool entrySideLeft = true;

	//Obtengo el bloque donde se debera insertar

	if(itKey != BPTreeInternalNode<TRecord,blockSize>::keys_.end())
		itChildren = BPTreeInternalNode<TRecord,blockSize>::getLeftChild(*itKey);
	else
	{
		itKey--;
		itChildren = BPTreeInternalNode<TRecord,blockSize>::getRightChild(*itKey);
		entrySideLeft = false;
	}

	if(BPTreeNode<TRecord,blockSize>::level_ == 1)
	{
		BPTreeVariableLeaf<TRecord,blockSize> *childLeaf = new BPTreeVariableLeaf<TRecord,blockSize>(BPTreeInternalNode<TRecord,blockSize>::leafCapacity_,
				*BPTreeNode<TRecord,blockSize>::file_,*itChildren);

		try
		{
			childLeaf->insert(record);
			childLeaf->write();
			delete childLeaf;

		}
		catch(LeafOverflowException exception)
		{
			BPTreeVariableLeaf<TRecord,blockSize> *newLeaf = new BPTreeVariableLeaf<TRecord,blockSize>(BPTreeInternalNode<TRecord,blockSize>::leafCapacity_,
					*BPTreeNode<TRecord,blockSize>::file_);
			typename std::list<TRecord> recordList = childLeaf->getRecords();

			unsigned int mediaList = BPTreeInternalNode<TRecord,blockSize>::leafCapacity_/2;
			typename std::list<TRecord>::iterator recordIt = recordList.begin();

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
			TRecord firstRecord = recordList.front();
			typename TRecord::Key newKey = dynamic_cast<const typename TRecord::Key &>(firstRecord.getKey());
			unsigned int newBlockNumber=newLeaf->blockNumber();
			delete newLeaf;
			delete childLeaf;

			insertInNode(newKey,newBlockNumber);
			return;
		}
		catch(LeafUnicityException exception)
		{
			delete childLeaf;
			throw;
		}
	}
	else
	{
		BPTreeVariableInternalNode<TRecord,blockSize>* childNode=new BPTreeVariableInternalNode<TRecord,blockSize>(
				BPTreeInternalNode<TRecord,blockSize>::capacity_,BPTreeInternalNode<TRecord,blockSize>::leafCapacity_,*BPTreeNode<TRecord,blockSize>::file_,*itChildren);
		try
		{
			childNode->insert(record);
			childNode->write();
			delete childNode;
		}
		catch(NodeOverflowException<typename TRecord::Key> ovException)
		{
			BPTreeVariableInternalNode<TRecord,blockSize>* newNode=new BPTreeVariableInternalNode<TRecord,blockSize>(BPTreeInternalNode<TRecord,blockSize>::capacity_,
					BPTreeInternalNode<TRecord,blockSize>::leafCapacity_,BPTreeNode<TRecord,blockSize>::level_,*BPTreeNode<TRecord,blockSize>::file_);

			typename std::list<typename TRecord::Key> childKeys=childNode->getKeys();
			std::list <unsigned int> childChildren = childNode->getChildrens();

			typename std::list<typename TRecord::Key>::iterator itKey =
						BPTreeInternalNode<TRecord,blockSize>::search(ovException.key);

			typename std::list<unsigned int>::iterator itChildren ;
			if(itKey!= BPTreeInternalNode<TRecord,blockSize>::keys_.end())
						itChildren=BPTreeInternalNode<TRecord,blockSize>::getRightChild(*itKey);
			else
						itChildren=BPTreeInternalNode<TRecord,blockSize>::children_.end();
			childKeys.insert(itKey,ovException.key);
			childChildren.insert(itChildren,ovException.child);
			unsigned int mediaList = BPTreeInternalNode<TRecord,blockSize>::capacity_/2;
			itKey = childKeys.begin();
			itChildren = childChildren.begin();
			itChildren++;
			for(unsigned int i = 0 ; i < mediaList;i++,itKey++,itChildren++);

			newNode->setFirstChild(*itChildren);
			if((*itKey)!=ovException.key)
				childNode->removeInNode(*itKey,*itChildren);
			itKey++;
			itChildren++;
			while(itKey != childKeys.end())
			{
				childNode->removeInNode(*itKey,*itChildren);
				newNode->insertInNode(*itKey,*itChildren);
				itKey++;
				itChildren++;
			}
			if((*itKey)>ovException.key)
				childNode->insertInNode(ovException.key,ovException.child);
			else if(*itKey<ovException.key)
				newNode->insertInNode(ovException.key,ovException.child);

			childNode->write();
			newNode->write();

			for(unsigned int i = 0 ; i < mediaList;i++,itKey++,itChildren++);

			unsigned int newBlockNumber=newNode->blockNumber();
			delete newNode;
			delete childNode;

			insertInNode(*itKey,newBlockNumber);
			return;

		}
		catch(LeafUnicityException exception)
		{
			delete childNode;
			throw;
		}

	}

}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::insertInNode(typename TRecord::Key & key, unsigned int child)
{

	if(BPTreeNode<TRecord,blockSize>::count_ == BPTreeInternalNode<TRecord,blockSize>::capacity_)
		   throw NodeOverflowException<typename TRecord::Key>(child,key);

	std::list<unsigned int>::iterator itBlockCount=blockCounts.begin();
	std::list<unsigned int>::iterator itFreeSpace=freeSpaces.begin();

	typename std::list<typename TRecord::Key>::iterator itKey =
			BPTreeInternalNode<TRecord,blockSize>::search(key);

	typename std::list<unsigned int>::iterator itChildren ;
	if(itKey!= BPTreeInternalNode<TRecord,blockSize>::keys_.end())
			 itChildren=BPTreeInternalNode<TRecord,blockSize>::getRightChild(*itKey);
	else
			itChildren=BPTreeInternalNode<TRecord,blockSize>::children_.end();


	unsigned int blockPos = getBlockPosition(itKey);

	BPTreeInternalNode<TRecord,blockSize>::keys_.insert(itKey,key);
	BPTreeInternalNode<TRecord,blockSize>::children_.insert(itChildren,child);
	BPTreeNode<TRecord,blockSize>::count_++;

	//Posiciono los iteradores en el bloque donde se va a insertar la clave.
	for(unsigned int i=0;i<blockPos;i++){
		itBlockCount++;
		itFreeSpace++;
	}

	//empiezo a insertar desde la primer clave del bloque.
	typename std::list<typename TRecord::Key>::iterator currentKey=getFirstKeyOfBlock(blockPos);

	while(currentKey!=BPTreeInternalNode<TRecord,blockSize>::keys_.end()){
		(*itBlockCount)=0;
		//Tengo todo el espacio libre.
		(*itFreeSpace)=VARIABLE_NODE_RECORDS_SPACE;
		while((*itFreeSpace)>=currentKey->size()){
			//inserto el current Key aumentando el count y disminuyendo el freeSpace
			(*itBlockCount)++;
			(*itFreeSpace)-=currentKey->size() + 4;
			currentKey++;
			if(currentKey==BPTreeInternalNode<TRecord,blockSize>::keys_.end())
				break;
		}
		if(currentKey==BPTreeInternalNode<TRecord,blockSize>::keys_.end())
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
template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::expand(){

	unsigned long newBlockNumber=BPTreeNode<TRecord,blockSize>::getFreeBlock();
	blockNumbers.push_back(newBlockNumber);
	blockCounts.push_back(0);
	freeSpaces.push_back(VARIABLE_NODE_RECORDS_SPACE);

}
template<class TRecord,unsigned int blockSize>
unsigned int BPTreeVariableInternalNode<TRecord,blockSize>::getBlockPosition(typename std::list<typename TRecord::Key>::iterator & itKey){
	typename std::list<typename TRecord::Key>::iterator it=BPTreeInternalNode<TRecord,blockSize>::keys_.begin();
	std::list<unsigned int>::iterator itBlockCount=blockCounts.begin();
	unsigned int blockPos=0;

	if(itKey==BPTreeInternalNode<TRecord,blockSize>::keys_.end())
			return blockCounts.size()-1;

	for(unsigned int count=1;it!=itKey;it++,count++)
		if(count==(*itBlockCount)){
			count=0;
			itBlockCount++;
			blockPos++;
		}

	return blockPos;
}
template<class TRecord,unsigned int blockSize>
typename std::list<typename TRecord::Key>::iterator BPTreeVariableInternalNode<TRecord,blockSize>::getFirstKeyOfBlock(unsigned int blockPos){
	typename std::list<typename TRecord::Key>::iterator itFirstRecord = BPTreeInternalNode<TRecord,blockSize>::keys_.begin();
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
template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::remove(TRecord &)
{

}
template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::removeInNode(typename TRecord::Key & key, unsigned int child){

}


#endif /* BPTREEVARIABLEINTERNALNODE_H_ */

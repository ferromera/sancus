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
	unsigned int freeSpace;

	void readRecords( BPTreeVariableNodeBlock<blockSize> *);
	void writeRecords( BPTreeVariableNodeBlock<blockSize> *);

public:
	BPTreeVariableInternalNode(unsigned int level,File & file);
	BPTreeVariableInternalNode(File & file,unsigned long blockNumber);

	void read();
	void write();
	void insert(TRecord &);
	void remove(TRecord &);
	void insertInNode(typename TRecord::Key & , unsigned int);
	void removeInNode(typename TRecord::Key & );
	void clear();
	virtual ~BPTreeVariableInternalNode(){}
};

template<class TRecord,unsigned int blockSize>
BPTreeVariableInternalNode<TRecord,blockSize>::BPTreeVariableInternalNode(unsigned int level,File & file):
BPTreeInternalNode<TRecord,blockSize>(level,file){
	freeSpace=blockSize-VARIABLE_NODE_CONTROL_BYTES;
}

template<class TRecord,unsigned int blockSize>
BPTreeVariableInternalNode<TRecord,blockSize>::BPTreeVariableInternalNode(File & file,unsigned long blockNumber):
BPTreeInternalNode<TRecord,blockSize>(file,blockNumber){
	read();
}
template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::read(){

	BPTreeVariableNodeBlock<blockSize> * block= new BPTreeVariableNodeBlock<blockSize>;

	File * file=BPTreeNode<TRecord,blockSize>::file_;
	unsigned long & blockNumber=BPTreeNode<TRecord,blockSize>::blockNumber_;

	BPTreeInternalNode<TRecord,blockSize>::keys_.clear();

	unsigned long pos=blockNumber*blockSize;
	file->seek(pos,File::BEG);
	file->read((char*)block,blockSize);
	BPTreeNode<TRecord,blockSize>::level_ = block->level;
	BPTreeInternalNode<TRecord,blockSize>::children_.push_back(block->leftChild);

	freeSpace=block->freeSpace;
	readRecords(block);

}
template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::readRecords(BPTreeVariableNodeBlock<blockSize> *block )
{
	char * currentPos=block->bytes;
	unsigned int readSpace=0;
	unsigned int usedSpace=blockSize - VARIABLE_NODE_CONTROL_BYTES - freeSpace;

	while(usedSpace>readSpace)
	{
		typename TRecord::Key * newKey = new typename TRecord::Key();
		newKey->read(& currentPos);
		BPTreeInternalNode<TRecord,blockSize>::keys_.push_back(*newKey);
		unsigned int rightChild;
		memcpy(&rightChild,currentPos,4);
		currentPos+=4;
		BPTreeInternalNode<TRecord,blockSize>::children_.push_back(rightChild);
		readSpace+=4+newKey->size();
		delete newKey;
	}
}
template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::write(){
	BPTreeVariableNodeBlock<blockSize> * block= new BPTreeVariableNodeBlock<blockSize>;
	File * file=BPTreeNode<TRecord,blockSize>::file_;

	block->freeSpace=freeSpace;
	block->level = BPTreeNode<TRecord,blockSize>::level_;
	block->leftChild = BPTreeInternalNode<TRecord,blockSize>::children_.front();

	writeRecords(block);
	file->seek(BPTreeNode<TRecord,blockSize>::blockNumber_*blockSize,File::BEG);
	file->write(block,blockSize);

}
template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::writeRecords(BPTreeVariableNodeBlock<blockSize> * block){

	typename std::list<typename TRecord::Key>::iterator itKeys=BPTreeInternalNode<TRecord,blockSize>::keys_.begin();
	std::list<unsigned int>::iterator itChildren=BPTreeInternalNode<TRecord,blockSize>::children_.begin();
	itChildren++;

	char * currentPos=block->bytes;
	while(itKeys!=BPTreeInternalNode<TRecord,blockSize>::keys_.end()){
		itKeys->write(&currentPos);
		memcpy(currentPos,&(*itChildren),4);
		currentPos+=4;
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

	//Obtengo el hijo donde se debera insertar

	if(itKey == BPTreeInternalNode<TRecord,blockSize>::keys_.end()){
		itKey--;
		itChildren = BPTreeInternalNode<TRecord,blockSize>::getRightChild(*itKey);
	}
	else if(*itKey==key)
		itChildren = BPTreeInternalNode<TRecord,blockSize>::getRightChild(*itKey);
	else // (*itKey)>key
		itChildren = BPTreeInternalNode<TRecord,blockSize>::getLeftChild(*itKey);


	if(BPTreeNode<TRecord,blockSize>::level_ == 1)
	{
		BPTreeVariableLeaf<TRecord,blockSize> *childLeaf = new BPTreeVariableLeaf<TRecord,blockSize>(*BPTreeNode<TRecord,blockSize>::file_,*itChildren);

		try
		{
			childLeaf->insert(record);
			childLeaf->write();
			delete childLeaf;

		}
		catch(LeafOverflowException ovException)
		{
			typename std::list<TRecord> recordList = childLeaf->getRecords();
			typename std::list<TRecord>::iterator recordIt = recordList.begin();

			BPTreeVariableLeaf<TRecord,blockSize> *newLeaf = new BPTreeVariableLeaf<TRecord,blockSize>(*BPTreeNode<TRecord,blockSize>::file_);

			unsigned int halfSize = (blockSize - VARIABLE_NODE_CONTROL_BYTES)/2;

			for( ;recordIt != recordList.end();recordIt++)
				if((*recordIt)>record)
					break;
			recordList.insert(recordIt,record);

			childLeaf->clear();

			recordIt = recordList.begin();
			for(unsigned int currSize=0;currSize<halfSize;recordIt++){
				childLeaf->insert(*recordIt);
				currSize+=recordIt->size();
			}
			for(;recordIt!=recordList.end();recordIt++)
				newLeaf->insert(*recordIt);

			newLeaf->next(childLeaf->next());
			childLeaf->next(newLeaf->blockNumber());
			childLeaf->write();
			newLeaf->write();

			TRecord firstRecord = newLeaf->getFirstRecord();
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
	else  //level > 1
	{
		BPTreeVariableInternalNode<TRecord,blockSize>* childNode=new BPTreeVariableInternalNode<TRecord,blockSize>(*BPTreeNode<TRecord,blockSize>::file_,*itChildren);
		try
		{
			childNode->insert(record);
			childNode->write();
			delete childNode;
		}
		catch(NodeOverflowException<typename TRecord::Key> ovException)
		{
			BPTreeVariableInternalNode<TRecord,blockSize>* newNode=new BPTreeVariableInternalNode<TRecord,blockSize>(BPTreeNode<TRecord,blockSize>::level_-1,
					*BPTreeNode<TRecord,blockSize>::file_);

			typename std::list<typename TRecord::Key> childKeys=childNode->getKeys();
			std::list <unsigned int> childChildren = childNode->getChildrens();

			//Busco donde voy a insertar la clave que subio y donde voy a insertar su hijo derecho.
			typename std::list<typename TRecord::Key>::iterator itKey=childKeys.begin() ;
			std::list<unsigned int>::iterator itChildren=childChildren.begin()  ;
			for(itChildren++;itKey!=childKeys.end();itKey++,itChildren++){
				       if(ovException.key <= (*itKey))
					       break;
			}
			childKeys.insert(itKey,ovException.key);
			childChildren.insert(itChildren,ovException.child);

			unsigned int halfSize = (blockSize - VARIABLE_NODE_CONTROL_BYTES)/2;
			childNode->clear();
			itKey = childKeys.begin();
			itChildren = childChildren.begin();
			childNode->setFirstChild(*itChildren);
			itChildren++;
			for(unsigned int currSize=0 ; currSize  < halfSize ; itKey++ , itChildren++){
				childNode->insertInNode(*itKey,*itChildren);
				currSize+=itKey->size() + 4;
			}
			typename TRecord::Key middleKey=*itKey;
			itKey++;
			newNode->setFirstChild(*itChildren);
			itChildren++;
			for(;itKey!=childKeys.end();itKey++ , itChildren++)
				newNode->insertInNode(*itKey,*itChildren);


			childNode->write();
			newNode->write();


			unsigned int newBlockNumber=newNode->blockNumber();
			delete newNode;
			delete childNode;

			insertInNode(middleKey,newBlockNumber);
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
	if(freeSpace< key.size() + 4)
		   throw NodeOverflowException<typename TRecord::Key>(child,key);

	typename std::list<typename TRecord::Key>::iterator itKey =
			BPTreeInternalNode<TRecord,blockSize>::search(key);

	typename std::list<unsigned int>::iterator itChildren ;

	if(itKey!= BPTreeInternalNode<TRecord,blockSize>::keys_.end())
			itChildren=BPTreeInternalNode<TRecord,blockSize>::getRightChild(*itKey);
	else
			itChildren=BPTreeInternalNode<TRecord,blockSize>::children_.end();


	BPTreeInternalNode<TRecord,blockSize>::keys_.insert(itKey,key);
	BPTreeInternalNode<TRecord,blockSize>::children_.insert(itChildren,child);
	freeSpace-= key.size() + 4;

}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::remove(TRecord &)
{

}
template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::removeInNode(typename TRecord::Key & key){

	typename std::list<typename TRecord::Key>::iterator itRemovePos=search(key);
	if( itRemovePos==BPTreeInternalNode<TRecord,blockSize>::keys_.end()
	|| (*itRemovePos)!=key)
		throw NodeKeyNotFoundException();

	if(freeSpace + key.size() + 4 > (blockSize - VARIABLE_NODE_CONTROL_BYTES)/2)
			throw NodeUnderflowException();

	std::list<unsigned int>::iterator itRightChild=getRightChild(key);
	BPTreeInternalNode<TRecord,blockSize>::children_.erase(itRightChild);
	BPTreeInternalNode<TRecord,blockSize>::keys_.erase(itRemovePos);
	freeSpace+=  key.size() + 4 ;

}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord,blockSize>::clear(){
	BPTreeInternalNode<TRecord,blockSize>::clear();
	freeSpace=blockSize-VARIABLE_NODE_CONTROL_BYTES;

}


#endif /* BPTREEVARIABLEINTERNALNODE_H_ */

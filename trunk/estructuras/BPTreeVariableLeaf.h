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

template<class TRecord,unsigned int blockSize>
class BPTreeVariableLeaf: public BPTreeLeaf<TRecord,blockSize>{
	std::list<unsigned int> blockNumbers;
	std::list<unsigned int> blockCounts;
	std::list<unsigned int> freeSpaces;
	void readRecords(BPTreeVariableLeafBlock<blockSize> *);
	void writeRecords(BPTreeVariableLeafBlock<blockSize> *,unsigned int);
	unsigned int getBlockPosition(typename std::list<TRecord>::iterator &);
	typename std::list<TRecord>::iterator getFirstRecordOfBlock(unsigned int);
	void expand();
	void reduce();
public:
	BPTreeVariableLeaf(unsigned int capacity,File & file);
	BPTreeVariableLeaf(unsigned int capacity,File & file,unsigned long pos);

	void read();
	void write();
	void insert(TRecord &);
	void remove(TRecord &);
	BPTreeLeaf<TRecord,blockSize> * nextLeaf();
	~BPTreeVariableLeaf();

};

template<class TRecord,unsigned int blockSize>
BPTreeVariableLeaf<TRecord,blockSize>::BPTreeVariableLeaf(unsigned int capacity,File & file):
BPTreeLeaf<TRecord,blockSize>(capacity,file){
	blockCounts.push_back(0);
	blockNumbers.push_back(BPTreeNode<TRecord,blockSize>::blockNumber_);
	freeSpaces.push_back(VARIABLE_LEAF_RECORDS_SPACE);
	write();
}

template<class TRecord,unsigned int blockSize>
BPTreeVariableLeaf<TRecord,blockSize>::BPTreeVariableLeaf(unsigned int capacity,File & file,unsigned long blockNumber):
BPTreeLeaf<TRecord,blockSize>(capacity,file,blockNumber){
	read();
}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::read(){
	BPTreeVariableLeafBlock<blockSize> * block= new BPTreeVariableLeafBlock<blockSize>;

	File * file=BPTreeNode<TRecord,blockSize>::file_;
	unsigned long & blockNumber=BPTreeNode<TRecord,blockSize>::blockNumber_;
	unsigned int & count=BPTreeNode<TRecord,blockSize>::count_;

	blockCounts.clear();
	blockNumbers.clear();
	freeSpaces.clear();
	BPTreeLeaf<TRecord,blockSize>::records_.clear();

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
	BPTreeLeaf<TRecord,blockSize>::next_=block->next;
	readRecords(block);
}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::readRecords(BPTreeVariableLeafBlock<blockSize> * block){
	unsigned int alreadyRead=0;
	char * currentPos=block->bytes;
	while(alreadyRead!=block->count){
		TRecord * newRecord=new TRecord(& currentPos);
		BPTreeLeaf<TRecord,blockSize>::records_.push_back(*newRecord);
		delete newRecord;
		alreadyRead++;
	}
}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::write(){
	BPTreeVariableLeafBlock<blockSize> * block= new BPTreeVariableLeafBlock<blockSize>;
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
		if(itNumber!=itLastNumber){
			itNumber++;
			block->next=*itNumber;
			itNumber--;
		}else
			block->next=BPTreeLeaf<TRecord,blockSize>::next_;
		writeRecords(block,blockPos);
		file->seek((*itNumber)*blockSize,File::BEG);
		file->write(block,blockSize);
	}

}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::writeRecords(BPTreeVariableLeafBlock<blockSize> * block,unsigned int blockPos){

	typename std::list<TRecord>::iterator itRecord=BPTreeLeaf<TRecord,blockSize>::records_.begin();
	std::list<unsigned int>::iterator itBlockCount=blockCounts.begin();
	unsigned int currBlock=0;

	for(unsigned int count=1;currBlock!=blockPos;itRecord++,count++)
			if(count==(*itBlockCount)){
				count=0;
				itBlockCount++;
				currBlock++;
			}

	unsigned int alreadyWritten=0;
	char * currentPos=block->bytes;
	while(alreadyWritten!=block->count){
		itRecord->write(&currentPos);
		alreadyWritten++;
		itRecord++;
	}
}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::insert(TRecord & rec){

	if(rec.size()>VARIABLE_LEAF_RECORDS_SPACE)
		throw BPTreeRecordSizeException();
	if(BPTreeNode<TRecord,blockSize>::count_ == BPTreeLeaf<TRecord,blockSize>::capacity_)
			   throw LeafOverflowException();

	typename std::list<TRecord>::iterator itInsertionPos=search(rec);
	if( itInsertionPos!=BPTreeLeaf<TRecord,blockSize>::records_.end()
			&& (*itInsertionPos)==rec)
		throw LeafUnicityException();
	TRecord * rp=new TRecord(rec);


	std::list<unsigned int>::iterator itBlockCount=blockCounts.begin();
	std::list<unsigned int>::iterator itFreeSpace=freeSpaces.begin();


	//Posiciono los iteradores en el bloque donde se va a insertar el registro.
	unsigned int blockPos=getBlockPosition(itInsertionPos);

	BPTreeLeaf<TRecord,blockSize>::records_.insert(itInsertionPos,*rp);
	BPTreeNode<TRecord,blockSize>::count_++;

	for(unsigned int i=0;i<blockPos;i++){
		itBlockCount++;
		itFreeSpace++;
	}

	//empiezo a insertar desde el primer registro del bloque.
	typename std::list<TRecord>::iterator currentRecord=getFirstRecordOfBlock(blockPos);

	while(currentRecord!=BPTreeLeaf<TRecord,blockSize>::records_.end()){
		(*itBlockCount)=0;
		//Tengo todo el espacio libre.
		(*itFreeSpace)=VARIABLE_LEAF_RECORDS_SPACE;
		while((*itFreeSpace)>=currentRecord->size()){
			//inserto el current record aumentando el count y disminuyendo el freeSpace
			(*itBlockCount)++;
			(*itFreeSpace)-=currentRecord->size();
			currentRecord++;
			if(currentRecord==BPTreeLeaf<TRecord,blockSize>::records_.end())
				break;
		}
		if(currentRecord==BPTreeLeaf<TRecord,blockSize>::records_.end())
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
unsigned int BPTreeVariableLeaf<TRecord,blockSize>::getBlockPosition(typename std::list<TRecord>::iterator & itRecord){
	typename std::list<TRecord>::iterator it=BPTreeLeaf<TRecord,blockSize>::records_.begin();
	std::list<unsigned int>::iterator itBlockCount=blockCounts.begin();
	unsigned int blockPos=0;

	if(itRecord==BPTreeLeaf<TRecord,blockSize>::records_.end())
			return blockCounts.size()-1;

	for(unsigned int count=1;it!=itRecord;it++,count++)
		if(count==(*itBlockCount)){
			count=0;
			itBlockCount++;
			blockPos++;
		}

	return blockPos;
}

template<class TRecord,unsigned int blockSize>
typename std::list<TRecord>::iterator BPTreeVariableLeaf<TRecord,blockSize>::getFirstRecordOfBlock(unsigned int blockPos){
	typename std::list<TRecord>::iterator itFirstRecord = BPTreeLeaf<TRecord,blockSize>::records_.begin();
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
void BPTreeVariableLeaf<TRecord,blockSize>::expand(){

	unsigned long newBlockNumber=BPTreeNode<TRecord,blockSize>::getFreeBlock();
	blockNumbers.push_back(newBlockNumber);
	blockCounts.push_back(0);
	freeSpaces.push_back(VARIABLE_LEAF_RECORDS_SPACE);

}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::remove(TRecord & rec){

	if(BPTreeNode<TRecord,blockSize>::count_==BPTreeLeaf<TRecord,blockSize>::capacity_/2)
			   throw LeafUnderflowException();

	typename std::list<TRecord>::iterator itRemovePos=search(rec);
	if( itRemovePos==BPTreeLeaf<TRecord,blockSize>::records_.end()
				|| (*itRemovePos)!=rec)
			throw LeafRecordNotFoundException();

	std::list<unsigned int>::iterator itBlockCount=blockCounts.begin();
	std::list<unsigned int>::iterator itFreeSpace=freeSpaces.begin();


	//Posiciono los iteradores en el bloque donde se va a borrar el registro.
	unsigned int blockPos=getBlockPosition(itRemovePos);

	BPTreeLeaf<TRecord,blockSize>::records_.erase(itRemovePos);
	BPTreeNode<TRecord,blockSize>::count_--;

		for(unsigned int i=0;i<blockPos;i++){
			itBlockCount++;
			itFreeSpace++;
		}

	//empiezo a insertar desde el primer registro del bloque.
	typename std::list<TRecord>::iterator currentRecord=getFirstRecordOfBlock(blockPos);

	while(currentRecord!=BPTreeLeaf<TRecord,blockSize>::records_.end()){
			(*itBlockCount)=0;
			//Tengo todo el espacio libre.
			(*itFreeSpace)=VARIABLE_LEAF_RECORDS_SPACE;
			while((*itFreeSpace)>=currentRecord->size()){
				//inserto el current record aumentando el count y disminuyendo el freeSpace
				(*itBlockCount)++;
				(*itFreeSpace)-=currentRecord->size();
				currentRecord++;
				if(currentRecord==BPTreeLeaf<TRecord,blockSize>::records_.end())
					break;
			}
			if(currentRecord==BPTreeLeaf<TRecord,blockSize>::records_.end())
				break;
			//Me muevo al siguiente bloque
			itFreeSpace++;
			itBlockCount++;
	}
	itBlockCount++;
	if(itBlockCount!=blockCounts.end())
		reduce();




}
template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::reduce(){
	BPTreeNode<TRecord,blockSize>::eraseBlock(*(--blockNumbers.end()));
	blockNumbers.pop_back();
	freeSpaces.pop_back();
	blockCounts.pop_back();
}

template<class TRecord,unsigned int blockSize>
BPTreeLeaf<TRecord,blockSize>* BPTreeVariableLeaf<TRecord,blockSize>::nextLeaf( ){
	// No Implementado
}
template<class TRecord,unsigned int blockSize>
BPTreeVariableLeaf<TRecord,blockSize>::~BPTreeVariableLeaf(){
}

#endif //BPTREEVARIABLELEAF_H

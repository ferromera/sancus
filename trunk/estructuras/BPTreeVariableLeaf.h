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
	unsigned int freeSpace;

	void readRecords(BPTreeVariableLeafBlock<blockSize> *);
	void writeRecords(BPTreeVariableLeafBlock<blockSize> *);

public:
	BPTreeVariableLeaf(File & file);
	BPTreeVariableLeaf(File & file,unsigned long pos);

	void read();
	void write();
	void insert(TRecord &);
	void remove(TRecord &);
	const TRecord & getFirstRecord()const;
	BPTreeLeaf<TRecord,blockSize> * nextLeaf();
	void clear();
	~BPTreeVariableLeaf();

};

template<class TRecord,unsigned int blockSize>
BPTreeVariableLeaf<TRecord,blockSize>::BPTreeVariableLeaf(File & file):
BPTreeLeaf<TRecord,blockSize>(file){
	freeSpace=blockSize - VARIABLE_LEAF_CONTROL_BYTES;
}

template<class TRecord,unsigned int blockSize>
BPTreeVariableLeaf<TRecord,blockSize>::BPTreeVariableLeaf(File & file,unsigned long blockNumber):
BPTreeLeaf<TRecord,blockSize>(file,blockNumber){
	read();
}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::read(){
	BPTreeVariableLeafBlock<blockSize> * block= new BPTreeVariableLeafBlock<blockSize>;

	File * file=BPTreeNode<TRecord,blockSize>::file_;
	unsigned long & blockNumber=BPTreeNode<TRecord,blockSize>::blockNumber_;

	BPTreeLeaf<TRecord,blockSize>::records_.clear();

	unsigned long pos=blockNumber*blockSize;
	file->seek(pos,File::BEG);
	file->read((char*)block,blockSize);

	freeSpace=block->freeSpace;
	BPTreeLeaf<TRecord,blockSize>::next_=block->next;
	readRecords(block);
	delete block;
}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::readRecords(BPTreeVariableLeafBlock<blockSize> * block){
	char * currentPos=block->bytes;
	unsigned int readSpace=0;
	unsigned int usedSpace=blockSize - VARIABLE_LEAF_CONTROL_BYTES - freeSpace;
	while(usedSpace!=readSpace){
		TRecord * newRecord=new TRecord(& currentPos);
		readSpace+=newRecord->size();
		BPTreeLeaf<TRecord,blockSize>::records_.push_back(*newRecord);
		delete newRecord;
	}
}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::write(){
	BPTreeVariableLeafBlock<blockSize> * block= new BPTreeVariableLeafBlock<blockSize>;
	File * file=BPTreeNode<TRecord,blockSize>::file_;
	unsigned long & blockNumber=BPTreeNode<TRecord,blockSize>::blockNumber_;

	block->freeSpace=freeSpace;
	block->next=BPTreeLeaf<TRecord,blockSize>::next_;

	writeRecords(block);

	file->seek(blockNumber * blockSize,File::BEG);
	file->write(block,blockSize);

}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::writeRecords(BPTreeVariableLeafBlock<blockSize> * block){

	typename std::list<TRecord>::iterator itRecord=BPTreeLeaf<TRecord,blockSize>::records_.begin();

	char * currentPos=block->bytes;

	while(itRecord!=BPTreeLeaf<TRecord,blockSize>::records_.end()){
		itRecord->write(&currentPos);
		itRecord++;
	}
}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::insert(TRecord & rec){

	if(rec.size()>blockSize - VARIABLE_LEAF_CONTROL_BYTES)
		throw BPTreeRecordSizeException();
	if(rec.size()>freeSpace)
		throw LeafOverflowException();

	typename std::list<TRecord>::iterator itInsertionPos=search(rec);
	if( itInsertionPos!=BPTreeLeaf<TRecord,blockSize>::records_.end()
			&& (*itInsertionPos)==rec)
		throw LeafUnicityException();
	TRecord * rp=new TRecord(rec);

	BPTreeLeaf<TRecord,blockSize>::records_.insert(itInsertionPos,*rp);
	freeSpace-=rec.size();

}

template<class TRecord,unsigned int blockSize>
const TRecord & BPTreeVariableLeaf<TRecord,blockSize>::getFirstRecord()const{
	return BPTreeLeaf<TRecord,blockSize>::records_.front();
}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::remove(TRecord & rec){

	typename std::list<TRecord>::iterator itRemovePos=search(rec);
	if( itRemovePos==BPTreeLeaf<TRecord,blockSize>::records_.end()
				|| (*itRemovePos)!=rec)
			throw LeafRecordNotFoundException();

	if(freeSpace + rec.size() > (blockSize - VARIABLE_LEAF_CONTROL_BYTES)/2)
				   throw LeafUnderflowException();


	BPTreeLeaf<TRecord,blockSize>::records_.erase(itRemovePos);
	freeSpace+=rec.size();

}

template<class TRecord,unsigned int blockSize>
BPTreeLeaf<TRecord,blockSize>* BPTreeVariableLeaf<TRecord,blockSize>::nextLeaf( ){
	// No Implementado
}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::clear(){
	BPTreeLeaf<TRecord,blockSize>::clear();
	freeSpace=blockSize-VARIABLE_LEAF_CONTROL_BYTES;
}

template<class TRecord,unsigned int blockSize>
BPTreeVariableLeaf<TRecord,blockSize>::~BPTreeVariableLeaf(){
}

#endif //BPTREEVARIABLELEAF_H

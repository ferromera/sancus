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
	bool hasMinimumCapacity();
	TRecord * search(const TRecord &);
	void update( TRecord & rec);
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

	if(BPTreeNode<TRecord,blockSize>::isFree_)
				throw ReadInAFreeNodeException();

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

	if(BPTreeNode<TRecord,blockSize>::isFree_)
				throw WriteInAFreeNodeException();

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

	typename std::list<TRecord>::iterator itInsertionPos=itSearch(rec);
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

	typename std::list<TRecord>::iterator itRemovePos=itSearch(rec);
	if( itRemovePos==BPTreeLeaf<TRecord,blockSize>::records_.end()
				|| (*itRemovePos)!=rec)
			throw LeafRecordNotFoundException();



	if(freeSpace > (blockSize - VARIABLE_LEAF_CONTROL_BYTES)/2){
		freeSpace+=itRemovePos->size();
		BPTreeLeaf<TRecord,blockSize>::records_.erase(itRemovePos);
		throw LeafUnderflowException();
	}

	freeSpace+=itRemovePos->size();
	BPTreeLeaf<TRecord,blockSize>::records_.erase(itRemovePos);

}

template<class TRecord,unsigned int blockSize>
BPTreeLeaf<TRecord,blockSize>* BPTreeVariableLeaf<TRecord,blockSize>::nextLeaf( ){
	if(BPTreeLeaf<TRecord,blockSize>::next_==0)
		throw ThereIsNoNextLeafException();

	return new BPTreeVariableLeaf<TRecord,blockSize>(*BPTreeNode<TRecord,blockSize>::file_,
											BPTreeLeaf<TRecord,blockSize>::next_);
}

template<class TRecord,unsigned int blockSize>
bool BPTreeVariableLeaf<TRecord,blockSize>::hasMinimumCapacity(){
	if(freeSpace > (blockSize - VARIABLE_LEAF_CONTROL_BYTES)/2)
		return true;
	else
		return false;
}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::clear(){
	BPTreeLeaf<TRecord,blockSize>::clear();
	freeSpace=blockSize-VARIABLE_LEAF_CONTROL_BYTES;
}

template<class TRecord,unsigned int blockSize>
TRecord * BPTreeVariableLeaf<TRecord,blockSize>::search(const TRecord & rec){
	TRecord * found;
	typename std::list<TRecord>::iterator it=BPTreeLeaf<TRecord,blockSize>::itSearch(rec);
	if(it==BPTreeLeaf<TRecord,blockSize>::records_.end()){
		BPTreeVariableLeaf<TRecord,blockSize> * myNextLeaf;
		try{
			myNextLeaf=(BPTreeVariableLeaf<TRecord,blockSize> *)nextLeaf();
			it=myNextLeaf->records_.begin();
			found=new TRecord(*it);
			delete myNextLeaf;
		}catch(ThereIsNoNextLeafException e){
			throw ThereIsNoGreaterRecordException();
		}
	}else{
		found=new TRecord(*it);
	}

	return found;
}
template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::update(TRecord & rec){
	unsigned int oldFreeSpace=freeSpace;
	try{
		remove(rec);
	}catch(LeafUnderflowException){
	}
	insert(rec);
	if(oldFreeSpace >(blockSize - VARIABLE_LEAF_CONTROL_BYTES)/2
		&& oldFreeSpace<freeSpace){
		throw LeafUnderflowException();
}



}
template<class TRecord,unsigned int blockSize>
BPTreeVariableLeaf<TRecord,blockSize>::~BPTreeVariableLeaf(){
}

#endif //BPTREEVARIABLELEAF_H

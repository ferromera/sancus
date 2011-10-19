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
	BPTreeVariableLeaf(const BPTreeVariableLeaf<TRecord,blockSize> & leaf);

	void read();
	void write();
	void insert(const TRecord &);
	void loadInsert(const TRecord &,float );
	void remove(const TRecord &);
	BPTreeLeaf<TRecord,blockSize> * nextLeaf();
	void clear();
	bool hasMinimumCapacity();
	void update(const TRecord & rec);
	void preOrderReport(File & , unsigned int);
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
	BPTreeLeaf<TRecord,blockSize>::searchIterator=BPTreeLeaf<TRecord,blockSize>::records_.begin();
}

template<class TRecord,unsigned int blockSize>
BPTreeVariableLeaf<TRecord,blockSize>::BPTreeVariableLeaf(const BPTreeVariableLeaf<TRecord,blockSize> & leaf):
BPTreeLeaf<TRecord,blockSize>(leaf),freeSpace(leaf.freeSpace){
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
	BPTreeNode<TRecord,blockSize>::level_ = block->level;
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
	while(usedSpace>readSpace){
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
	block->level=BPTreeNode<TRecord,blockSize>::level_;
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
void BPTreeVariableLeaf<TRecord,blockSize>::insert(const TRecord & rec){
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
void BPTreeVariableLeaf<TRecord,blockSize>::loadInsert(const TRecord & rec ,float compression){
	if(((float)(blockSize-VARIABLE_LEAF_CONTROL_BYTES))*(1.0-compression)+(float)rec.size() > (float)freeSpace)
		throw LeafOverflowException();

	typename std::list<TRecord>::iterator itInsertionPos=itSearch(rec);
	TRecord * rp=new TRecord(rec);
	BPTreeLeaf<TRecord,blockSize>::records_.insert(itInsertionPos,*rp);
	freeSpace-=rec.size();
}

template<class TRecord,unsigned int blockSize>
void BPTreeVariableLeaf<TRecord,blockSize>::remove(const TRecord & rec){

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
		throw ThereIsNoNextLeafException<TRecord>(BPTreeLeaf<TRecord,blockSize>::records_.back());

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
void BPTreeVariableLeaf<TRecord,blockSize>::update(const TRecord & rec){
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
void  BPTreeVariableLeaf<TRecord,blockSize>::preOrderReport(File & reportFile,unsigned int treeLevel){
	for(unsigned int i=0; i<treeLevel ;i++)
		reportFile<<"|\t";
	reportFile<<"Node "<<BPTreeNode<TRecord,blockSize>::blockNumber_<<" : ";
	reportFile<<"("<<0<<")("<<freeSpace<<")("<<BPTreeLeaf<TRecord,blockSize>::next_<<") -- ";

	typename std::list<TRecord>::iterator itRecords=BPTreeLeaf<TRecord,blockSize>::records_.begin();
	for(;itRecords!=BPTreeLeaf<TRecord,blockSize>::records_.end();itRecords++){
		reportFile<<"("<<itRecords->getKey().getKey()<<")";
	}
	reportFile<<"\n";
}

template<class TRecord,unsigned int blockSize>
BPTreeVariableLeaf<TRecord,blockSize>::~BPTreeVariableLeaf(){
}

#endif //BPTREEVARIABLELEAF_H

/*
 * IndexedDataFile.h
 *
 *  Created on: 13/10/2011
 *      Author: fernando
 */

#ifndef INDEXEDDATAFILE_H_
#define INDEXEDDATAFILE_H_

#include "BPTreeVariableLeaf.h"
#include "FreeSpaceStackBlock.h"
#include "IndexedDataFileExceptions.h"


template<class TRecord,unsigned int blockSize>
class IndexedDataFile {
	bool overflow_;
	bool underflow_;
	bool fusion_;
	unsigned int newBlock_;
	TRecord::Key * newLastKey_;
	TRecord::Key * lastKeyOfNewBlock_;
	BPTreeVariableLeaf<TRecord,blockSize> * searchBlock_;
	File * file_;
	TRecord * found_;

	void handleOverflow(const TRecord &rec,BPTreeVariableLeaf<TRecord,blockSize> *block);
	void handleUnderflow(BPTreeVariableLeaf<TRecord,blockSize> *block);
public:
	IndexedDataFile(const string & path,const string & creationMode="create");
	void insert(const TRecord & rec, unsigned int blockNumber);
	void remove(const TRecord & rec, unsigned int blockNumber);
	const TRecord & search(const TRecord & rec , unsigned int blockNumber);
	const TRecord & next();
	void update(const TRecord &, unsigned int blockNumber);
	virtual ~IndexedDataFile();
};


template<class TRecord,unsigned int blockSize>
IndexedDataFile<TRecord,blockSize>::IndexedDataFile(const string & path,const string & creationMode)
:overflow_(false),underflow_(false),fusion_(false),newBlock_(0),newLastKey_(NULL),lastKeyOfNewBlock_(NULL),searchBlock_(NULL),found_(NULL){
	if(creationMode.compare("create")==0)
		create();
	else if(creationMode.compare("load")==0)
		load();
	else
		throw IndexedDataFileWrongCreationModeException();
}

template<class TRecord,unsigned int blockSize>
void IndexedDataFile<TRecord,blockSize>::create(){
	file_= new File(path,File::NEW|File::BIN|File::IO);
	FreeSpaceStackBlock<blockSize>* fblock = new FreeSpaceStackBlock<blockSize>;
	fblock->blockNumber=2;
	fblock->infile=0;
	file_->write((char *)fblock,blockSize);
	delete fblock;
	BPTreeVariableLeafBlock<blockSize> * emptyBlock= new BPTreeVariableLeafBlock<blockSize>;
	emptyBlock->freeSpace=blockSize-VARIABLE_LEAF_CONTROL_BYTES;
	emptyBlock->next=0;
	file_->write((char*)emptyBlock,blockSIze);
	delete emptyBlock;
}

template<class TRecord,unsigned int blockSize>
void IndexedDataFile<TRecord,blockSize>::load(){
	file_= new File(path,File::BIN|File::IO);
}

template<class TRecord,unsigned int blockSize>
void IndexedDataFile<TRecord,blockSize>::insert(const TRecord & rec, unsigned int blockNumber){
	overflow_=false;
	delete searchBlock_;
	searchBlock_=NULL;
	BPTreeVariableLeaf<TRecord,blockSize> * block=BPTreeVariableLeaf<TRecord,blockSize>(*file_,blockNumber);
	try{
		block->insert(rec);
		block->write();
		delete block;
	}catch(LeafOverflowException e){
		handleOverflow(rec,block);
	}
}

template<class TRecord,unsigned int blockSize>
void IndexedDataFile<TRecord,blockSize>::handleOverflow(const TRecord &rec,BPTreeVariableLeaf<TRecord,blockSize> *block){
	// Se crea un nuevo bloque y muevo la mitad superior de los registros al nuevo bloque.
	overflow_=true;
	delete newLastKey_;
	delete lastKeyOfNewBlock_;

	BPTreeVariableLeaf<TRecord,blockSize> * newBlock = BPTreeVariableLeaf<TRecord,blockSize>(*file_);
	typename std::list<TRecord> records = block->getRecords;
	typename std::list<TRecord> recordsIt = records.begin();

	for( ;recordsIt != records.end();recordsIt++)
		if((*recordsIt)>rec)
			break;
	records.insert(recordsIt,rec);

	block->clear();

	recordsIt = records.begin();

	unsigned int freeSpace= blockSize- VARIABLE_LEAF_CONTROL_BYTES;
	unsigned int halfSize= freeSpace/2;

	for(;freeSpace>halfSize;recordsIt++){
		block->insert(*recordsIt);
		freeSpace-=recordsIt->size();
	}
	newLastKey_=new TRecord::Key(recordsIt->getKey());

	for(;recordsIt!=recordsIt.end();recordsIt++)
		newBlock->insert(*recordsIt);
	recordsIt--;
	lastKeyOfNewBlock_=new TRecord::Key(recordsIt->getKey());
	newBlock->next(block->next());
	block->next(newBlock->blockNumber());

	newBlock->write();
	block->write();

	newBlock_=newBlock->blockNumber();

	delete block;
	delete newBlock;

}

template<class TRecord,unsigned int blockSize>
void IndexedDataFile<TRecord,blockSize>::remove(const TRecord & rec, unsigned int blockNumber){
	underflow_=false;
	fusion_=false;
	delete searchBlock_;
	searchBlock_=NULL;
	delete newLastKey_;
	BPTreeVariableLeaf<TRecord,blockSize> * block=BPTreeVariableLeaf<TRecord,blockSize>(*file_,blockNumber);
	try{
		block->remove(rec);
		newLastKey_=new TRecord::Key(block->getLastRecord().getKey());
		block->write();
		delete block;
	}catch(LeafUnderflowException e){
		handleUnderflow(block);
	}
}

template<class TRecord,unsigned int blockSize>
void IndexedDataFile<TRecord,blockSize>::handleUnderflow(BPTreeVariableLeaf<TRecord,blockSize> *block){
	// Se balancea o fusiona con el siguiente bloque.
	underflow_=true;
	fusion_=false;
	BPTreeVariableLeaf<TRecord,blockSize> * nextBlock ;
	try{
		nextBlock = block->nextLeaf();
	}catch(ThereIsNoNextLeafException e){
		newLastKey_=new TRecord::Key(block->getLastRecord().getKey());
		block->write();
		return;
	}

	typename std::list<TRecord> mixedRecords=block->getRecords();
	typename std::list<TRecord> nextRecords=nextBlock->getRecords();

	mixedRecords.insert(mixedRecords.end(),nextRecords.begin(),nextRecords.end());


	if(nextRecords->hasMinimumCapacity()){
		//fusion
		fusion_=true;

		block->next(nextBlock->next());
		nextBlock->free();
		block->clear();
		typename std::list<TRecord>::iterator itMixedRecords=mixedRecords.begin();
		for( ; itMixedRecords!=mixedRecords.end();itMixedRecords++)
			block->insert(*itMixedRecords);
		block->write();
		itMixedRecords--;
		newLastKey_=new TRecord::Key(itMixedRecods->getKey());

		delete block;
		delete nextBlock;

		return;
	}
	else{
		//balanceo
		block->clear();
		nextBlock->clear();
		unsigned int blockBytes=0 , newBlockBytes=0;
		typename std::list<TRecord>::iterator itFront=mixedRecords.begin();
		typename std::list<TRecord>::iterator itBack=--mixedRecords.end();
		while(itFront!=itBack){
			if(blockBytes<=newBlockBytes){
				block->insert(*itFront);
					blockBytes+=itFront->size();
					itFront++;
			}
			else{
					newBlock->insert(*itBack);
					newBlockBytes+=itBack->size();
					itBack--;
			}
		}

		if(blockBytes<=newBlockBytes)
			block->insert(*itFront);
		else
			newBlock->insert(*itFront);

		block->write();
		newBlock->write();
		newLastKey_=new TRecord::Key(block->getFirstRecprd().getKey());
		delete block;
		delete newBlock;
		return;
	}
}

template<class TRecord,unsigned int blockSize>
const TRecord & IndexedDataFile<TRecord,blockSize>::search(const TRecord & rec , unsigned int blockNumber){
	BPTreeVariableLeaf<TRecord,blockSize> * block=BPTreeVariableLeaf<TRecord,blockSize>(*file_,blockNumber);
	delete found_;
	found_=NULL;
	try{
		found_=block->search(rec);
		delete searchBlock_;
		searchBlock_= new BPTreeVariableLeaf<TRecord,blockSize>(*block);
		delete block;
		return *found_;
	}catch(LeafRecordNotFoundException e){
		delete searchBlock_;
		searchBlock_=NULL;
		delete block;
		throw IndexedDataRecordNotFoundException();
	}
}

template<class TRecord,unsigned int blockSize>
const TRecord & IndexedDataFile<TRecord,blockSize>::next(){
	delete found_;
	if(searchBlock_==NULL)
		throw IndexedDataNextException();
	try{
		found_=searchBlock_->nextRecord();
		return *found_;
	}catch(LeafRecordNotFoundException e){
		delete searchBlock_;
		searchBlock_=NULL;
		throw IndexedDataNoMoreRecordsInBlockException();

	}
}

template<class TRecord,unsigned int blockSize>
void IndexedDataFile<TRecord,blockSize>::update(const TRecord & rec, unsigned int blockNumber){
	underflow_=false;
	overflow_=false;
	fusion_=false;
	delete searchBlock_;
	searchBlock_=NULL;
	delete newLastKey_;
	BPTreeVariableLeaf<TRecord,blockSize> * block=BPTreeVariableLeaf<TRecord,blockSize>(*file_,blockNumber);
	try{
		block->update(rec);
		block->write();
		delete block;
	}catch(LeafUnderflowException e){
		handleUnderflow(block);
	}catch(LeafOverflowException e){
		handleOverflow(rec,block);
	}
}

template<class TRecord,unsigned int blockSize>
IndexedDataFile<TRecord,blockSize>::~IndexedDataFile(){
	delete newLastKey_;
	delete lastKeyOfNewBlock_;
	delete searchBlock_;
	delete file_;
	delete found_;
}



#endif /* INDEXEDDATAFILE_H_ */

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
	typename TRecord::Key * newLastKey_;
	BPTreeVariableLeaf<TRecord,blockSize> * searchBlock_;
	File * file_;
	TRecord * found_;
	std::string path_;

	void handleOverflow(const TRecord &rec,BPTreeVariableLeaf<TRecord,blockSize> *block);
	void handleUnderflow(BPTreeVariableLeaf<TRecord,blockSize> *block);
	void create();
	void load();

public:
	IndexedDataFile(const string & path);
	void insert(const TRecord & rec, unsigned int blockNumber);
	void remove(const TRecord & rec, unsigned int blockNumber);
	const TRecord & search(const typename TRecord::Key & key , unsigned int blockNumber);
	const TRecord & next();
	void update(const TRecord &, unsigned int blockNumber);
	const typename TRecord::Key & getNewKey(){return *newLastKey_;}
	unsigned int getNewBlock(){return newBlock_;}
	bool overflow(){return overflow_;}
	bool underflow(){return underflow_;}
	bool fusion(){return fusion_;}
 	virtual ~IndexedDataFile();
};


template<class TRecord,unsigned int blockSize>
IndexedDataFile<TRecord,blockSize>::IndexedDataFile(const string & path)
:overflow_(false),underflow_(false),fusion_(false),newBlock_(0),newLastKey_(NULL),searchBlock_(NULL),found_(NULL),path_(path){
	try{
		load();
	}catch(OpenFileException){
		create();
	}
}

template<class TRecord,unsigned int blockSize>
void IndexedDataFile<TRecord,blockSize>::create(){
	file_= new File(path_,File::NEW|File::BIN|File::IO);
	FreeSpaceStackBlock<blockSize>* fblock = new FreeSpaceStackBlock<blockSize>;
	fblock->blockNumber=1;
	fblock->inFile=0;
	file_->write((char *)fblock,blockSize);
	delete fblock;
	BPTreeVariableLeaf<TRecord,blockSize>* emptyBlock=new BPTreeVariableLeaf<TRecord,blockSize>(*file_);
	emptyBlock->write();
	delete emptyBlock;


}

template<class TRecord,unsigned int blockSize>
void IndexedDataFile<TRecord,blockSize>::load(){
	file_= new File(path_,File::BIN|File::IO);
}

template<class TRecord,unsigned int blockSize>
void IndexedDataFile<TRecord,blockSize>::insert(const TRecord & rec, unsigned int blockNumber){
	overflow_=false;
	delete searchBlock_;
	searchBlock_=NULL;
	BPTreeVariableLeaf<TRecord,blockSize> * block=new BPTreeVariableLeaf<TRecord,blockSize>(*file_,blockNumber);
	try{
		block->insert(rec);
		block->write();
		delete block;
	}catch(LeafOverflowException e){
		handleOverflow(rec,block);
	}catch(LeafRecordNotFoundException e){
		delete block;
		throw IndexedDataRecordNotFoundException();
	}catch(LeafUnicityException e){
		throw IndexedDataInsertException();
	}
}

template<class TRecord,unsigned int blockSize>
void IndexedDataFile<TRecord,blockSize>::handleOverflow(const TRecord &rec,BPTreeVariableLeaf<TRecord,blockSize> *block){
	// Se crea un nuevo bloque y muevo la mitad superior de los registros al nuevo bloque.
	overflow_=true;
	delete newLastKey_;


	BPTreeVariableLeaf<TRecord,blockSize> * newBlock = new BPTreeVariableLeaf<TRecord,blockSize>(*file_);
	typename std::list<TRecord> records = block->getRecords();
	typename std::list<TRecord>::iterator recordsIt = records.begin();

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
	recordsIt--;
	newLastKey_=new typename TRecord::Key(recordsIt->getKey());
	recordsIt++;

	for(;recordsIt!=records.end();recordsIt++)
		newBlock->insert(*recordsIt);

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
	BPTreeVariableLeaf<TRecord,blockSize> * block=new BPTreeVariableLeaf<TRecord,blockSize>(*file_,blockNumber);
	try{
		block->remove(rec);
		newLastKey_=new typename TRecord::Key(block->getLastRecord().getKey());
		block->write();
		delete block;
	}catch(LeafUnderflowException e){
		handleUnderflow(block);
	}catch(LeafRecordNotFoundException e){
		delete block;
		throw IndexedDataRecordNotFoundException();
	}
}

template<class TRecord,unsigned int blockSize>
void IndexedDataFile<TRecord,blockSize>::handleUnderflow(BPTreeVariableLeaf<TRecord,blockSize> *block){
	// Se balancea o fusiona con el siguiente bloque.
	underflow_=true;
	fusion_=false;
	BPTreeVariableLeaf<TRecord,blockSize> * nextBlock ;
	try{
		nextBlock = (BPTreeVariableLeaf<TRecord,blockSize> *)block->nextLeaf();
	}catch(ThereIsNoNextLeafException<TRecord>& e){
		newLastKey_=new typename TRecord::Key(block->getLastRecord().getKey());
		block->write();
		return;
	}

	typename std::list<TRecord> mixedRecords=block->getRecords();
	typename std::list<TRecord> nextRecords=nextBlock->getRecords();

	mixedRecords.insert(mixedRecords.end(),nextRecords.begin(),nextRecords.end());


	if(nextBlock->hasMinimumCapacity()){
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
		newLastKey_=new typename TRecord::Key(itMixedRecords->getKey());

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
				nextBlock->insert(*itBack);
					newBlockBytes+=itBack->size();
					itBack--;
			}
		}

		if(blockBytes<=newBlockBytes){
			block->insert(*itFront);
			newLastKey_=new typename TRecord::Key(itFront->getKey());
		}
		else{
			nextBlock->insert(*itFront);
			itFront--;
			newLastKey_=new typename TRecord::Key(itFront->getKey());
		}

		block->write();
		nextBlock->write();
		delete block;
		delete nextBlock;
		return;
	}
}

template<class TRecord,unsigned int blockSize>
const TRecord & IndexedDataFile<TRecord,blockSize>::search(const typename TRecord::Key & key , unsigned int blockNumber){
	TRecord* rec=new TRecord(key);
	BPTreeVariableLeaf<TRecord,blockSize> * block=new BPTreeVariableLeaf<TRecord,blockSize>(*file_,blockNumber);
	delete found_;
	found_=NULL;
	try{
		found_=block->search(*rec);
		delete searchBlock_;
		searchBlock_= new BPTreeVariableLeaf<TRecord,blockSize>(*block);
		delete block;
		delete rec;
		return *found_;
	}catch(LeafRecordNotFoundException e){
		delete searchBlock_;
		searchBlock_=NULL;
		delete block;
		delete rec;
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
		BPTreeVariableLeaf<TRecord,blockSize> * oldSearchLeaf=searchBlock_;
		try{
			searchBlock_=(BPTreeVariableLeaf<TRecord,blockSize> *)searchBlock_->nextLeaf();
		}catch(ThereIsNoNextLeafException<TRecord>& e){
			delete searchBlock_;
			searchBlock_=NULL;
			throw IndexedDataNextException();
		}
		delete oldSearchLeaf;
		found_ = new TRecord(searchBlock_->getFirstRecord());

		return *found_;
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
	BPTreeVariableLeaf<TRecord,blockSize> * block=new BPTreeVariableLeaf<TRecord,blockSize>(*file_,blockNumber);
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
	delete searchBlock_;
	delete file_;
	delete found_;
}



#endif /* INDEXEDDATAFILE_H_ */

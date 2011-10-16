/*
 * ListFile.cpp
 *
 *  Created on: 16/10/2011
 *      Author: fernando
 */

#include "ListFile.h"

ListFile* ListFile::instance=NULL;

static ListFile * ListFile::getInstance(){
	if(instance==NULL)
		instance= new ListFile();
	return instance;
}

ListFile::ListFile(){
	electionIndex= new BPlusTree<SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key>,4096>(LIST_FILE_ELECTION_INDEX_PATH);
	nameIndex= new BPlusTree<SecondaryIndexRecord<StringKey,VoteCountingRecord::Key>,4096>(LIST_FILE_NAME_INDEX_PATH);
	primaryIndex= new BPlusTree<PrimaryIndexRecord<ListRecord::Key>,4096>(LIST_FILE_PRIMARY_INDEX_PATH);
	dataFile= new IndexedDataFile<ListFile,8192>(LIST_FILE_DATA_PATH);
	lastSearch= NO_SEARCH;
	electionSearched=NULL;
	nameSearched=NULL;
	found=NULL;
}

void ListFile::insert(const ListRecord & record){
	PrimaryIndexRecord<ListRecord::Key> indexToFind(record.getKey(),0);
	PrimaryIndexRecord<ListRecord::Key>* primaryIndexFound=NULL;
	try{
		primaryIndexFound=new PrimaryIndexRecord<ListRecord::Key>(primaryIndex->search(indexToFind));
		dataFile->insert(record,primaryIndexFound->getBlockNumber());
		if(dataFile->overflow()){
			VoteCountingRecord::Key newKey=dataFile->getNewKey();
			PrimaryIndexRecord<VoteCountingRecord::Key> newIndexRecord(newKey,primaryIndexFound->getBlockNumber());
			primaryIndex->insert(*primaryIndexFound);
			primaryIndexFound->setBlockNumber(dataFile->getNewBlock());
			primaryIndex->update(*primaryIndexFound);
		}
	}catch(ThereIsNoGreaterRecordException e){
		unsigned int block=dataFile->append(record);
		PrimaryIndexRecord<VoteCountingRecord::Key> newIndexRecord(record.getKey(),block);
	}
	catch(IndexedDataRecordNotFoundException e){
		throw FileInsertException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key>* newElection;
	SecondaryIndexRecord<StringKey,ListRecord::Key>* newName;
	newElection= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(record.getElection(),record.getKey());
	StringKey nameKey(record.getName());
	newName= new SecondaryIndexRecord<StringKey,ListRecord::Key>(nameKey,record.getKey());
	electionIndex->insert(*newElection);
	nameIndex->insert(*newName);
	delete newElection;
	delete newName;
}

void ListFile::remove(const ListRecord & record){
	PrimaryIndexRecord<ListRecord::Key> indexToFind(record.getKey(),0);
	PrimaryIndexRecord<ListRecord::Key>* primaryIndexFound=NULL;
	try{
		primaryIndexFound=new PrimaryIndexRecord<ListRecord::Key>(primaryIndex->search(indexToFind));
		ListRecord * deletedRecord= new ListRecord(dataFile->search(record,primaryIndexFound->getBlockNumber()));
		dataFile->remove(record,primaryIndexFound->getBlockNumber());
		if(dataFile->underflow()){
			ListRecord::Key newKey=dataFile->getNewKey();
			if(fusion){
				primaryIndex->remove(*primaryIndexFound);
				primaryIndexFound->setKey(newKey);
				primaryIndex->update(*primaryIndexFound);
			}else{
				primaryIndex->remove(*primaryIndexFound);
				primaryIndexFound->setKey(newKey);
				primaryIndex->insert(*primaryIndexFound);
			}
		}
	}catch(ThereIsNoGreaterRecordException e ){
		throw FileRemoveException();
	}catch(IndexedDataRecordNotFoundException e){
		throw FileRemoveException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>* deletedElection;
	SecondaryIndexRecord<StringKey,VoteCountingRecord::Key>* deletedName;
	deletedElection= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(deletedRecord.getElection(),deletedRecord.getKey());
	StringKey nameKey(deletedRecord.getName());
	deletedName= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(nameKey,deletedRecord.getKey());
	electionIndex->remove(*deletedElection);
	nameIndex->remove(*deletedName);
	delete deletedElection;
	delete deletedName;
	delete deletedRecord;
}
void ListFile::update(const ListRecord & record){
	PrimaryIndexRecord<ListRecord::Key> indexToFind(record.getKey(),0);
	PrimaryIndexRecord<ListRecord::Key>* primaryIndexFound=NULL;
	try{
		primaryIndexFound=new PrimaryIndexRecord<ListRecord::Key>(primaryIndex->search(indexToFind));
		ListRecord * oldRecord= new ListRecord(dataFile->search(record,primaryIndexFound->getBlockNumber()));
		dataFile->update(record,primaryIndexFound->getBlockNumber());
		if(dataFile->overflow()){
			ListRecord::Key newKey=dataFile->getNewKey();
			PrimaryIndexRecord<ListRecord::Key> newIndexRecord(newKey,primaryIndexFound->getBlockNumber());
			primaryIndex->insert(*primaryIndexFound);
			primaryIndexFound->setBlockNumber(dataFile->getNewBlock());
			primaryIndex->update(*primaryIndexFound);
		}else if(dataFile->underflow()){
			ListRecord::Key newKey=dataFile->getNewKey();
			if(fusion){
				primaryIndex->remove(*primaryIndexFound);
				primaryIndexFound->setKey(newKey);
				primaryIndex->update(*primaryIndexFound);
			}else{
				primaryIndex->remove(*primaryIndexFound);
				primaryIndexFound->setKey(newKey);
				primaryIndex->insert(*primaryIndexFound);
			}
		}
	}catch(ThereIsNoGreaterRecordException e ){
		throw FileUpdateException();
	}catch(IndexedDataRecordNotFoundException e){
		throw FileUpdateException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key>* oldElection,*newElection;
	SecondaryIndexRecord<StringKey,ListRecord::Key>* oldName,*newName;
	oldElection= new SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key>(oldRecord.getElection(),oldRecord.getKey());
	StringKey oldNameKey(oldRecord.getName());
	oldName= new SecondaryIndexRecord<StringKey,ListRecord::Key>(oldNameKey,oldRecord.getKey());
	newElection= new SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key>(record.getElection(),record.getKey());
	StringKey newNameKey(record.getName());
	newName= new SecondaryIndexRecord<StringKey,ListRecord::Key>(newNameKey,record.getKey());

	electionIndex->remove(*oldElection);
	nameIndex->remove(*oldName);
	electionIndex->insert(*newElection);
	nameIndex->insert(*newName);
	delete oldElection;
	delete oldName;
	delete newElection;
	delete newName;
	delete oldRecord;
}
const ListRecord & ListFile::searchByElection(const ElectionRecord::Key & election){
	lastSearch=ELECTION_SEARCH;
	delete electionSearched;
	electionSearched= new ElectionRecord::Key (election);
	SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key> firstSecIndex(election);
	firstSecIndex=electionIndex->search(firstSecIndex);
	if(firstSecIndex.getAttribute()!=election)
			throw FileSearchNotFoundException();
	ListRecord::Key listKey=firstSecIndex.getPrimary();
	PrimaryIndexRecord<ListRecord::Key> indexToFind(listKey,0);
	indexToFind=primaryIndex->search(indexToFind);
	return dataFile->search(indexToFind,indexToFind.getBlockNumber());
}
const ListRecord & ListFile::searchByName(const std::string & name){
	lastSearch=NAME_SEARCH;
	delete nameSearched;
	nameSearched= new StringKey (name);
	SecondaryIndexRecord<StringKey,ListRecord::Key> firstSecIndex(election);
	firstSecIndex=electionIndex->search(firstSecIndex);
	if(firstSecIndex.getAttribute()!=(*nameSearched))
		throw FileSearchNotFoundException();
	ListRecord::Key listKey=firstSecIndex.getPrimary();
	PrimaryIndexRecord<ListRecord::Key> indexToFind(listKey,0);
	indexToFind=primaryIndex->search(indexToFind);
	return dataFile->search(indexToFind,indexToFind.getBlockNumber());
}
const ListRecord & ListFile::search(const ListRecord::Key & list){
	lastSearch=PRIMARY_SEARCH;
	PrimaryIndexRecord<ListRecord::Key> indexToFind(list,0);
	indexToFind=primaryIndex->search(indexToFind);
	return dataFile->search(indexToFind,indexToFind.getBlockNumber());
}
const ListRecord & ListFile::nextElection(){
	if(lastSearch!=ELECTION_SEARCH)
		throw FileNextException();
	try{
		delete found;
		found= new ListRecord(dataFile->next());
		if(found->getElection()!=(*electionSearched))
			throw FileNextException();
		return *found;
	}catch(IndexedDataNextException e){
		throw FileNextException();
	}
}
const ListRecord & ListFile::nextName(){
	if(lastSearch!=NAME_SEARCH)
		throw FileNextException();
	try{
		delete found;
		found= new ListRecord(dataFile->next());
		if(found->getElection()!=(*nameSearched))
			throw FileNextException();
		return *found;
	}catch(IndexedDataNextException e){
		throw FileNextException();
	}

}
const ListRecord & ListFile::next(){
	if(lastSearch!=PRIMARY_SEARCH)
		throw FileNextException();
	try{
		delete found;
		found= new ListRecord(dataFile->next());
		return *found;
	}catch(IndexedDataNextException e){
		throw FileNextException();
	}
}

ListFile::~ListFile(){
	delete electionIndex;
	delete nameIndex;
	delete primaryIndex;
	delete dataFile;
	delete electionSearched;
	delete nameSearched;
	delete found;
}

/*
 * ListFile.cpp
 *
 *  Created on: 16/10/2011
 *      Author: fernando
 */

#include "ListFile.h"

ListFile* ListFile::instance=NULL;

ListFile * ListFile::getInstance(){
	if(instance==NULL)
		instance= new ListFile();
	return instance;
}

ListFile::ListFile(){
	electionIndex= new BPlusTree<SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key>,4096>(LIST_FILE_ELECTION_INDEX_PATH);
	nameIndex= new BPlusTree<SecondaryIndexRecord<StringKey,ListRecord::Key>,4096>(LIST_FILE_NAME_INDEX_PATH);
	primaryIndex= new BPlusTree<PrimaryIndexRecord<ListRecord::Key>,4096>(LIST_FILE_PRIMARY_INDEX_PATH);
	ListRecord::Key highKey;
	highKey.setHighValue();
	PrimaryIndexRecord<ListRecord::Key> lastRecord(highKey,1);
	primaryIndex->insert(lastRecord);
	dataFile= new IndexedDataFile<ListRecord,8192>(LIST_FILE_DATA_PATH);
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
			ListRecord::Key newKey=dataFile->getNewKey();
			PrimaryIndexRecord<ListRecord::Key> newIndexRecord(newKey,primaryIndexFound->getBlockNumber());
			primaryIndex->insert(newIndexRecord);
			primaryIndexFound->setBlockNumber(dataFile->getNewBlock());
			primaryIndex->update(*primaryIndexFound);
		}
	}catch(ThereIsNoNextLeafException<PrimaryIndexRecord<ListRecord::Key> > e){
		dataFile->insert(record,e.rec.getBlockNumber());
		if(dataFile->overflow()){
			primaryIndex->remove(e.rec);
			ListRecord::Key newKey=dataFile->getNewKey();
			PrimaryIndexRecord<ListRecord::Key> newIndexRecord(newKey,e.rec.getBlockNumber());
			primaryIndex->insert(newIndexRecord);
			indexToFind.setBlockNumber(dataFile->getNewBlock());
			primaryIndex->insert(indexToFind);
		}
		else{
			primaryIndex->remove(e.rec);
			ListRecord::Key newKey=dataFile->getNewKey();
			indexToFind.setBlockNumber(e.rec.getBlockNumber());
			primaryIndex->insert(indexToFind);
		}
	}
	catch(IndexedDataRecordNotFoundException e){
		throw FileInsertException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key>* newElection;
	SecondaryIndexRecord<StringKey,ListRecord::Key>* newName;
	newElection= new SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key>(record.getElection(),record.getKey());
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
	ListRecord * deletedRecord;
	try{
		primaryIndexFound=new PrimaryIndexRecord<ListRecord::Key>(primaryIndex->search(indexToFind));
		deletedRecord= new ListRecord(dataFile->search(record.getKey(),primaryIndexFound->getBlockNumber()));
		dataFile->remove(record,primaryIndexFound->getBlockNumber());
		if(dataFile->underflow()){
			ListRecord::Key newKey=dataFile->getNewKey();
			if(dataFile->fusion()){
				primaryIndex->remove(*primaryIndexFound);
				primaryIndexFound->setKey(newKey);
				primaryIndex->update(*primaryIndexFound);
			}else{
				primaryIndex->remove(*primaryIndexFound);
				primaryIndexFound->setKey(newKey);
				primaryIndex->insert(*primaryIndexFound);
			}
		}
	}catch(ThereIsNoNextLeafException<PrimaryIndexRecord<ListRecord::Key> > e ){
		throw FileRemoveException();
	}catch(IndexedDataRecordNotFoundException e){
		throw FileRemoveException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key>* deletedElection;
	SecondaryIndexRecord<StringKey,ListRecord::Key>* deletedName;
	deletedElection= new SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key>(deletedRecord->getElection(),deletedRecord->getKey());
	StringKey nameKey(deletedRecord->getName());
	deletedName= new SecondaryIndexRecord<StringKey,ListRecord::Key>(nameKey,deletedRecord->getKey());
	electionIndex->remove(*deletedElection);
	nameIndex->remove(*deletedName);
	delete deletedElection;
	delete deletedName;
	delete deletedRecord;
}
void ListFile::update(const ListRecord & record){
	PrimaryIndexRecord<ListRecord::Key> indexToFind(record.getKey(),0);
	PrimaryIndexRecord<ListRecord::Key>* primaryIndexFound=NULL;
	ListRecord * oldRecord;
	try{
		primaryIndexFound=new PrimaryIndexRecord<ListRecord::Key>(primaryIndex->search(indexToFind));
		oldRecord= new ListRecord(dataFile->search(record.getKey(),primaryIndexFound->getBlockNumber()));
		dataFile->update(record,primaryIndexFound->getBlockNumber());
		if(dataFile->overflow()){
			ListRecord::Key newKey=dataFile->getNewKey();
			PrimaryIndexRecord<ListRecord::Key> newIndexRecord(newKey,primaryIndexFound->getBlockNumber());
			primaryIndex->insert(*primaryIndexFound);
			primaryIndexFound->setBlockNumber(dataFile->getNewBlock());
			primaryIndex->update(*primaryIndexFound);
		}else if(dataFile->underflow()){
			ListRecord::Key newKey=dataFile->getNewKey();
			if(dataFile->fusion()){
				primaryIndex->remove(*primaryIndexFound);
				primaryIndexFound->setKey(newKey);
				primaryIndex->update(*primaryIndexFound);
			}else{
				primaryIndex->remove(*primaryIndexFound);
				primaryIndexFound->setKey(newKey);
				primaryIndex->insert(*primaryIndexFound);
			}
		}
	}catch(ThereIsNoNextLeafException<PrimaryIndexRecord<ListRecord::Key> > e ){
		throw FileUpdateException();
	}catch(IndexedDataRecordNotFoundException e){
		throw FileUpdateException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key>* oldElection,*newElection;
	SecondaryIndexRecord<StringKey,ListRecord::Key>* oldName,*newName;
	oldElection= new SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key>(oldRecord->getElection(),oldRecord->getKey());
	StringKey oldNameKey(oldRecord->getName());
	oldName= new SecondaryIndexRecord<StringKey,ListRecord::Key>(oldNameKey,oldRecord->getKey());
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
			throw FileSearchException();
	ListRecord::Key listKey=firstSecIndex.getPrimary();
	PrimaryIndexRecord<ListRecord::Key> indexToFind(listKey,0);
	indexToFind=primaryIndex->search(indexToFind);
	return dataFile->search(firstSecIndex.getPrimary(),indexToFind.getBlockNumber());
}
const ListRecord & ListFile::searchByName(const std::string & name){
	lastSearch=NAME_SEARCH;
	delete nameSearched;
	nameSearched= new StringKey (name);
	SecondaryIndexRecord<StringKey,ListRecord::Key> firstSecIndex(name);
	firstSecIndex=nameIndex->search(firstSecIndex);
	if(firstSecIndex.getAttribute()!=(*nameSearched))
		throw FileSearchException();
	std::cout<<firstSecIndex.getPrimary().getKey()<<endl;
	ListRecord::Key listKey=firstSecIndex.getPrimary();
	PrimaryIndexRecord<ListRecord::Key> indexToFind(listKey,0);
	indexToFind=primaryIndex->search(indexToFind);
	std::cout<<indexToFind.getPrimary().getKey()<<endl;
	return dataFile->search(firstSecIndex.getPrimary(),indexToFind.getBlockNumber());
}
const ListRecord & ListFile::search(const ListRecord::Key & list){
	lastSearch=PRIMARY_SEARCH;
	PrimaryIndexRecord<ListRecord::Key> indexToFind(list,0);
	indexToFind=primaryIndex->search(indexToFind);
	return dataFile->search(indexToFind.getPrimary(),indexToFind.getBlockNumber());
}
const ListRecord & ListFile::nextElection(){
	if(lastSearch!=ELECTION_SEARCH)
		throw FileNextException();
	try{
		SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key> firstSecIndex=electionIndex->next();
		if(firstSecIndex.getAttribute()!=(*electionSearched))
				throw FileNextException();
		ListRecord::Key listKey=firstSecIndex.getPrimary();
		PrimaryIndexRecord<ListRecord::Key> indexToFind(listKey,0);
		indexToFind=primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(),indexToFind.getBlockNumber());
	}catch(ThereIsNoNextLeafException<SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key> >){
		throw FileNextException();
	}

}
const ListRecord & ListFile::nextName(){
	if(lastSearch!=NAME_SEARCH)
		throw FileNextException();
	try{
		SecondaryIndexRecord<StringKey,ListRecord::Key> firstSecIndex=nameIndex->next();
		if(firstSecIndex.getAttribute()!=(*nameSearched))
				throw FileNextException();
		ListRecord::Key listKey=firstSecIndex.getPrimary();
		PrimaryIndexRecord<ListRecord::Key> indexToFind(listKey,0);
		indexToFind=primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(),indexToFind.getBlockNumber());
	}catch(ThereIsNoNextLeafException<SecondaryIndexRecord<ElectionRecord::Key,ListRecord::Key> >){
		throw FileNextException();
	}
	/*try{
		delete found;
		found= new ListRecord(dataFile->next());
		if(found->getName()!=(nameSearched->getKey()))
			throw FileNextException();
		return *found;
	}catch(IndexedDataNextException e){
		throw FileNextException();
	}*/

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

void ListFile::report(){
	nameIndex->preOrderReport();
	electionIndex->preOrderReport();
	primaryIndex->preOrderReport();
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

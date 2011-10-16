/*
 * VoteCountingFile.cpp
 *
 *  Created on: 21/09/2011
 *      Author: juanmromera
 */

#include "VoteCountingFile.h"

VoteCountingFile* VoteCountingFile::instance=NULL;

VoteCountingFile::VoteCountingFile(){

	electionIndex= new BPlusTree<SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>,4096>(VOTE_COUNTING_ELECTION_INDEX_PATH);
	districtIndex= new BPlusTree<SecondaryIndexRecord<DistrictRecord::Key,VoteCountingRecord::Key>,4096>(VOTE_COUNTING_DISTRICT_INDEX_PATH);
	listIndex= new BPlusTree<SecondaryIndexRecord<ListRecord::Key,VoteCountingRecord::Key>,4096>(VOTE_COUNTING_LIST_INDEX_PATH);
	primaryIndex= new BPlusTree<PrimaryIndexRecord<VoteCountingRecord::Key>,4096>(VOTE_COUNTING_PRIMARY_INDEX_PATH);
	dataFile= new IndexedDataFile<VoteCountingRecord,8192>(VOTE_COUNTING_DATA_PATH);
	lastSearch= NO_SEARCH;

}
VoteCountingFile* VoteCountingFile::getInstance(){
	if(instance==NULL)
		instance= new VoteCountingFile();
	return instance;
}

VoteCountingFile::~VoteCountingFile(){
	delete electionIndex;
	delete districtIndex;
	delete listIndex;
	delete primaryIndex;
	delete dataFile;
}

void VoteCountingFile::insert(const VoteCountingRecord & record){
	PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(record.getKey(),0);
	PrimaryIndexRecord<VoteCountingRecord::Key>* primaryIndexFound=NULL;
	try{
		primaryIndexFound=new PrimaryIndexRecord<VoteCountingRecord::Key>(primaryIndex->search(indexToFind));
		dataFile->insert(record,primaryIndexFound->getBlockNumber());
		if(dataFile->overflow()){
			VoteCountingRecord::Key newKey=dataFile->getNewKey();
			PrimaryIndexRecord<VoteCountingRecord::Key> newIndexRecord(newKey,primaryIndexFound->getBlockNumber());
			primaryIndex->insert(*primaryIndexFound);
			primaryIndexFound->setBlockNumber(dataFile->getNewBlock());
			primaryIndex->update(*primaryIndexFound);
		}
	}catch(ThereIsNoGreaterRecordException e){
		unsigned int block=dataFile.append(record);
		PrimaryIndexRecord<VoteCountingRecord::Key> newIndexRecord(record.getKey(),block);
	}
	catch(IndexedDataRecordNotFoundException e){
				throw FileInsertException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>* newElection;
	SecondaryIndexRecord<DistrictRecord::Key,VoteCountingRecord::Key>* newDistrict;
	SecondaryIndexRecord<ListRecord::Key,VoteCountingRecord::Key>* newList;
	newElection= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(record.getElection(),record.getKey());
	newDistrict= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(record.getDistrict(),record.getKey());
	newList= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(record.getList(),record.getKey());
	electionIndex->insert(*newElection);
	districtIndex->insert(*newDistrict);
	listIndex->insert(*newList);
	delete newElection;
	delete newDistrict;
	delete newList;

}

void VoteCountingFile::remove(const ElectionRecord & record){
	PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(record.getKey(),0);
	PrimaryIndexRecord<VoteCountingRecord::Key>* primaryIndexFound=NULL;
	try{
		primaryIndexFound=new PrimaryIndexRecord<VoteCountingRecord::Key>(primaryIndex->search(indexToFind));
		ElectionRecord * deletedRecord= new ElectionRecord(dataFile->search(record,primaryIndexFound->getBlockNumber()));
		dataFile->remove(record,primaryIndexFound->getBlockNumber());
		if(dataFile->underflow()){
			VoteCountingRecord::Key newKey=dataFile->getNewKey();
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
	SecondaryIndexRecord<DistrictRecord::Key,VoteCountingRecord::Key>* deletedDistrict;
	SecondaryIndexRecord<ListRecord::Key,VoteCountingRecord::Key>* deletedList;
	deletedElection= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(deletedRecord.getElection(),deletedRecord.getKey());
	deletedDistrict= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(deletedRecord.getDistrict(),deletedRecord.getKey());
	deletedList= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(deletedRecord.getList(),deletedRecord.getKey());
	electionIndex->remove(*deletedElection);
	districtIndex->remove(*deletedDistrict);
	listIndex->remove(*deletedList);
	delete deletedElection;
	delete deletedDistrict;
	delete deletedList;
	delete deletedRecord;
}

void VoteCountingFile::update(const ElectionRecord & record){
	PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(record.getKey(),0);
	PrimaryIndexRecord<VoteCountingRecord::Key>* primaryIndexFound=NULL;
	try{
		primaryIndexFound=new PrimaryIndexRecord<VoteCountingRecord::Key>(primaryIndex->search(indexToFind));
		ElectionRecord * oldRecord= new ElectionRecord(dataFile->search(record,primaryIndexFound->getBlockNumber()));
		dataFile->update(record,primaryIndexFound->getBlockNumber());
		if(dataFile->overflow()){
			VoteCountingRecord::Key newKey=dataFile->getNewKey();
			PrimaryIndexRecord<VoteCountingRecord::Key> newIndexRecord(newKey,primaryIndexFound->getBlockNumber());
			primaryIndex->insert(*primaryIndexFound);
			primaryIndexFound->setBlockNumber(dataFile->getNewBlock());
			primaryIndex->update(*primaryIndexFound);
		}else if(dataFile->underflow()){
			VoteCountingRecord::Key newKey=dataFile->getNewKey();
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
	SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>* oldElection,*newElection;
	SecondaryIndexRecord<DistrictRecord::Key,VoteCountingRecord::Key>* oldDistrict,*newDistrict;
	SecondaryIndexRecord<ListRecord::Key,VoteCountingRecord::Key>* oldList,*newList;
	oldElection= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(oldRecord.getElection(),oldRecord.getKey());
	oldDistrict= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(oldRecord.getDistrict(),oldRecord.getKey());
	oldList= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(oldRecord.getList(),oldRecord.getKey());
	newElection= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(record.getElection(),record.getKey());
	newDistrict= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(record.getDistrict(),record.getKey());
	newList= new SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>(record.getList(),record.getKey());
	electionIndex->remove(*oldElection);
	districtIndex->remove(*oldDistrict);
	listIndex->remove(*oldList);
	electionIndex->insert(*newElection);
	districtIndex->insert(*newDistrict);
	listIndex->insert(*newList);
	delete oldElection;
	delete oldDistrict;
	delete oldList;
	delete newElection;
	delete newDistrict;
	delete newList;
	delete oldRecord;

}

const VoteCountingRecord & VoteCountingFile::searchByDistrict(const DistrictRecord::Key & district){
	lastSearch=DISTRICT_SEARCH;
	SecondaryIndexRecord<DistrictRecord::Key,VoteCountingRecord::Key> firstSecIndex(district);
	firstSecIndex=districtIndex->search(firstSecIndex);
	if(firstSecIndex.getAttribute()!=district)
		throw FileSearchNotFoundException();
	VoteCountingRecord::Key voteCountKey=firstSecIndex.getPrimary();
	PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(voteCountKey,0);
	indexToFind=primaryIndex->search(indexToFind);
	return dataFile->search(indexToFind,indexToFind.getBlockNumber());
}

const VoteCountingRecord & VoteCountingFile::searchByElection(const ElectionRecord::Key & election){
	lastSearch=ELECTION_SEARCH;
	SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key> firstSecIndex(election);
	firstSecIndex=electionIndex->search(firstSecIndex);
	if(firstSecIndex.getAttribute()!=election)
			throw FileSearchNotFoundException();
	VoteCountingRecord::Key voteCountKey=firstSecIndex.getPrimary();
	PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(voteCountKey,0);
	indexToFind=primaryIndex->search(indexToFind);
	return dataFile->search(indexToFind,indexToFind.getBlockNumber());
}



const VoteCountingRecord & VoteCountingFile::searchByList(const ListRecord::Key & list){
	lastSearch=LIST_SEARCH;
	SecondaryIndexRecord<ListRecord::Key,VoteCountingRecord::Key> firstSecIndex(list);
	firstSecIndex=listIndex->search(firstSecIndex);
	if(firstSecIndex.getAttribute()!=list)
		throw FileSearchNotFoundException();
	VoteCountingRecord::Key voteCountKey=firstSecIndex.getPrimary();
	PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(voteCountKey,0);
	indexToFind=primaryIndex->search(indexToFind);
	return dataFile->search(indexToFind,indexToFind.getBlockNumber());
}

const VoteCountingRecord & VoteCountingFile::search(const VoteCountingFile::Key & voteCountKey){
	lastSearch=PRIMARY_SEARCH;
	PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(voteCountKey,0);
	indexToFind=primaryIndex->search(indexToFind);
	return dataFile->search(indexToFind,indexToFind.getBlockNumber());
}

const VoteCountingRecord & VoteCountingFile::nextDistrict(){
	if(lastSearch!=DISTRICT_SEARCH)
		throw FileNextException();
	try{
		return dataFile->next();
	}catch(IndexedDataNextException e){
		throw FileNextException();
	}
}

const VoteCountingRecord & VoteCountingFile::nextElection(){
	if(lastSearch!=ELECTION_SEARCH)
		throw FileNextException();
	try{
		return dataFile->next();
	}catch(IndexedDataNextException e){
		throw FileNextException();
	}
}
const VoteCountingRecord & VoteCountingFile::nextList(){
	if(lastSearch!=LIST_SEARCH)
		throw FileNextException();
	try{
		return dataFile->next();
	}catch(IndexedDataNextException e){
		throw FileNextException();
	}
}
const VoteCountingRecord & VoteCountingFile::next(){
	if(lastSearch!=PRIMARY_SEARCH)
		throw FileNextException();
	try{
		return dataFile->next();
	}catch(IndexedDataNextException e){
		throw FileNextException();
	}
}




/*
 * ChargeFile.cpp
 *
 *  Created on: 19/10/2011
 *      Author: fernando
 */

#include "ChargeFile.h"


ChargeFile* ChargeFile::instance=NULL;

ChargeFile * ChargeFile::getInstance(){
	if(instance==NULL)
		instance= new ChargeFile();
	return instance;
}

ChargeFile::ChargeFile() {
	fatherIndex= new BPlusTree<SecondaryIndexRecord<ChargeRecord::Key,ChargeRecord::Key>,4096>(CHARGE_FILE_FATHER_INDEX_PATH);
	districtIndex= new BPlusTree<SecondaryIndexRecord<DistrictRecord::Key,ChargeRecord::Key>,4096>(CHARGE_FILE_DISTRICT_INDEX_PATH);
	primaryIndex= new BPlusTree<PrimaryIndexRecord<ChargeRecord::Key>,4096>(CHARGE_FILE_PRIMARY_INDEX_PATH);
	ChargeRecord::Key highKey;
	highKey.setHighValue();
	PrimaryIndexRecord<ChargeRecord::Key> lastRecord(highKey,1);
	primaryIndex->insert(lastRecord);
	dataFile= new IndexedDataFile<ChargeRecord,8192>(CHARGE_FILE_DATA_PATH);
	lastSearch= NO_SEARCH;
	fatherSearched=NULL;
	districtSearched=NULL;
	found=NULL;

}

void ChargeFile::insert(const ChargeRecord & record){
	PrimaryIndexRecord<ChargeRecord::Key> indexToFind(record.getKey(),0);
	PrimaryIndexRecord<ChargeRecord::Key>* primaryIndexFound=NULL;
	try{
		primaryIndexFound=new PrimaryIndexRecord<ChargeRecord::Key>(primaryIndex->search(indexToFind));
		dataFile->insert(record,primaryIndexFound->getBlockNumber());
		if(dataFile->overflow()){
			ChargeRecord::Key newKey=dataFile->getNewKey();
			PrimaryIndexRecord<ChargeRecord::Key> newIndexRecord(newKey,primaryIndexFound->getBlockNumber());
			primaryIndex->insert(newIndexRecord);
			primaryIndexFound->setBlockNumber(dataFile->getNewBlock());
			primaryIndex->update(*primaryIndexFound);
		}
	}catch(ThereIsNoNextLeafException<PrimaryIndexRecord<ChargeRecord::Key> > e){
		dataFile->insert(record,e.rec.getBlockNumber());
		if(dataFile->overflow()){
			primaryIndex->remove(e.rec);
			ChargeRecord::Key newKey=dataFile->getNewKey();
			PrimaryIndexRecord<ChargeRecord::Key> newIndexRecord(newKey,e.rec.getBlockNumber());
			primaryIndex->insert(newIndexRecord);
			indexToFind.setBlockNumber(dataFile->getNewBlock());
			primaryIndex->insert(indexToFind);
		}
		else{
			primaryIndex->remove(e.rec);
			ChargeRecord::Key newKey=dataFile->getNewKey();
			indexToFind.setBlockNumber(e.rec.getBlockNumber());
			primaryIndex->insert(indexToFind);
		}
	}
	catch(IndexedDataRecordNotFoundException e){
		throw FileInsertException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<ChargeRecord::Key,ChargeRecord::Key>* newFather=NULL;
	SecondaryIndexRecord<DistrictRecord::Key,ChargeRecord::Key>* newDistrict;
	if(record.hasFather()){
		newFather= new SecondaryIndexRecord<ChargeRecord::Key,ChargeRecord::Key>(record.getChargeFather(),record.getKey());
		fatherIndex->insert(*newFather);
	}
	newDistrict= new SecondaryIndexRecord<DistrictRecord::Key,ChargeRecord::Key>(record.getDistrict(),record.getKey());

	districtIndex->insert(*newDistrict);
	delete newFather;
	delete newDistrict;
}

void ChargeFile::remove(const ChargeRecord & record){
	PrimaryIndexRecord<ChargeRecord::Key> indexToFind(record.getKey(),0);
	PrimaryIndexRecord<ChargeRecord::Key>* primaryIndexFound=NULL;
	ChargeRecord * deletedRecord;
	try{
		primaryIndexFound=new PrimaryIndexRecord<ChargeRecord::Key>(primaryIndex->search(indexToFind));
		deletedRecord= new ChargeRecord(dataFile->search(record.getKey(),primaryIndexFound->getBlockNumber()));
		dataFile->remove(record,primaryIndexFound->getBlockNumber());
		if(dataFile->underflow()){
			ChargeRecord::Key newKey=dataFile->getNewKey();
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
	}catch(ThereIsNoNextLeafException<PrimaryIndexRecord<ChargeRecord::Key> > e ){
		throw FileRemoveException();
	}catch(IndexedDataRecordNotFoundException e){
		throw FileRemoveException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<ChargeRecord::Key,ChargeRecord::Key>* deletedFather=NULL;
	SecondaryIndexRecord<DistrictRecord::Key,ChargeRecord::Key>* deletedDistrict;
	if(deletedRecord->hasFather()){
		deletedFather= new SecondaryIndexRecord<ChargeRecord::Key,ChargeRecord::Key>(deletedRecord->getChargeFather(),deletedRecord->getKey());
		fatherIndex->remove(*deletedFather);
	}
	deletedDistrict= new SecondaryIndexRecord<DistrictRecord::Key,ChargeRecord::Key>(deletedRecord->getDistrict(),deletedRecord->getKey());
	districtIndex->remove(*deletedDistrict);
	delete deletedFather;
	delete deletedDistrict;
	delete deletedRecord;
}
void ChargeFile::update(const ChargeRecord & record){
	PrimaryIndexRecord<ChargeRecord::Key> indexToFind(record.getKey(),0);
	PrimaryIndexRecord<ChargeRecord::Key>* primaryIndexFound=NULL;
	ChargeRecord * oldRecord;
	try{
		primaryIndexFound=new PrimaryIndexRecord<ChargeRecord::Key>(primaryIndex->search(indexToFind));
		oldRecord= new ChargeRecord(dataFile->search(record.getKey(),primaryIndexFound->getBlockNumber()));
		dataFile->update(record,primaryIndexFound->getBlockNumber());
		if(dataFile->overflow()){
			ChargeRecord::Key newKey=dataFile->getNewKey();
			PrimaryIndexRecord<ChargeRecord::Key> newIndexRecord(newKey,primaryIndexFound->getBlockNumber());
			primaryIndex->insert(newIndexRecord);
			primaryIndexFound->setBlockNumber(dataFile->getNewBlock());
			primaryIndex->update(*primaryIndexFound);
		}else if(dataFile->underflow()){
			ChargeRecord::Key newKey=dataFile->getNewKey();
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
	}catch(ThereIsNoNextLeafException<PrimaryIndexRecord<ChargeRecord::Key> > e ){
		throw FileUpdateException();
	}catch(IndexedDataRecordNotFoundException e){
		throw FileUpdateException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<ChargeRecord::Key,ChargeRecord::Key>* oldFather=NULL,*newFather=NULL;
	SecondaryIndexRecord<DistrictRecord::Key,ChargeRecord::Key>* oldDistrict,*newDistrict;
	if(oldRecord->hasFather()){
		oldFather= new SecondaryIndexRecord<ChargeRecord::Key,ChargeRecord::Key>(oldRecord->getChargeFather(),oldRecord->getKey());
		fatherIndex->remove(*oldFather);
	}
	if(record.hasFather()){
		newFather= new SecondaryIndexRecord<ChargeRecord::Key,ChargeRecord::Key>(record.getChargeFather(),record.getKey());
		fatherIndex->insert(*newFather);
	}

	oldDistrict= new SecondaryIndexRecord<DistrictRecord::Key,ChargeRecord::Key>(oldRecord->getDistrict(),oldRecord->getKey());
	newDistrict= new SecondaryIndexRecord<DistrictRecord::Key,ChargeRecord::Key>(record.getDistrict(),record.getKey());

	districtIndex->remove(*oldDistrict);
	districtIndex->insert(*newDistrict);

	delete oldFather;
	delete newFather;
	delete oldDistrict;
	delete newDistrict;
	delete oldRecord;
}
const ChargeRecord & ChargeFile::searchByFather(const ChargeRecord::Key & father){
	lastSearch=FATHER_SEARCH;
	delete fatherSearched;
	fatherSearched= new ChargeRecord::Key (father);
	SecondaryIndexRecord<ChargeRecord::Key,ChargeRecord::Key> firstSecIndex(father);
	firstSecIndex=fatherIndex->search(firstSecIndex);
	if(firstSecIndex.getAttribute()!=father)
			throw FileSearchException();
	ChargeRecord::Key chargeKey=firstSecIndex.getPrimary();
	PrimaryIndexRecord<ChargeRecord::Key> indexToFind(chargeKey,0);
	indexToFind=primaryIndex->search(indexToFind);
	return dataFile->search(firstSecIndex.getPrimary(),indexToFind.getBlockNumber());
}
const ChargeRecord & ChargeFile::searchByDistrict(const DistrictRecord::Key & district){
	lastSearch=DISTRICT_SEARCH;
	delete districtSearched;
	districtSearched= new DistrictRecord::Key (district);
	SecondaryIndexRecord<DistrictRecord::Key,ChargeRecord::Key> firstSecIndex(district);
	firstSecIndex=districtIndex->search(firstSecIndex);
	if(firstSecIndex.getAttribute()!=district)
			throw FileSearchException();
	ChargeRecord::Key chargeKey=firstSecIndex.getPrimary();
	PrimaryIndexRecord<ChargeRecord::Key> indexToFind(chargeKey,0);
	indexToFind=primaryIndex->search(indexToFind);
	return dataFile->search(firstSecIndex.getPrimary(),indexToFind.getBlockNumber());
}

const ChargeRecord & ChargeFile::search(const ChargeRecord::Key & charge){
	lastSearch=PRIMARY_SEARCH;
	PrimaryIndexRecord<ChargeRecord::Key> indexToFind(charge,0);
	indexToFind=primaryIndex->search(indexToFind);
	return dataFile->search(charge,indexToFind.getBlockNumber());
}
const ChargeRecord & ChargeFile::nextFather(){
	if(lastSearch!=FATHER_SEARCH)
		throw FileNextException();
	try{
		SecondaryIndexRecord<ChargeRecord::Key,ChargeRecord::Key> firstSecIndex=fatherIndex->next();
		if(firstSecIndex.getAttribute()!=(*fatherSearched))
				throw FileNextException();
		ChargeRecord::Key chargeKey=firstSecIndex.getPrimary();
		PrimaryIndexRecord<ChargeRecord::Key> indexToFind(chargeKey,0);
		indexToFind=primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(),indexToFind.getBlockNumber());
	}catch(ThereIsNoNextLeafException<SecondaryIndexRecord<ChargeRecord::Key,ChargeRecord::Key> >){
		throw FileNextException();
	}

}
const ChargeRecord & ChargeFile::nextDistrict(){
	if(lastSearch!=DISTRICT_SEARCH)
		throw FileNextException();
	try{
		SecondaryIndexRecord<DistrictRecord::Key,ChargeRecord::Key> firstSecIndex=districtIndex->next();
		if(firstSecIndex.getAttribute()!=(*districtSearched))
				throw FileNextException();
		ChargeRecord::Key chargeKey=firstSecIndex.getPrimary();
		PrimaryIndexRecord<ChargeRecord::Key> indexToFind(chargeKey,0);
		indexToFind=primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(),indexToFind.getBlockNumber());
	}catch(ThereIsNoNextLeafException<SecondaryIndexRecord<DistrictRecord::Key,ChargeRecord::Key> >){
		throw FileNextException();
	}

}
const ChargeRecord & ChargeFile::next(){
	if(lastSearch!=PRIMARY_SEARCH)
		throw FileNextException();
	try{
		delete found;
		found= new ChargeRecord(dataFile->next());
		return *found;
	}catch(IndexedDataNextException e){
		throw FileNextException();
	}
}

void ChargeFile::report(){

	districtIndex->preOrderReport();
	primaryIndex->preOrderReport();
	fatherIndex->preOrderReport();
}

ChargeFile::~ChargeFile(){
	delete fatherIndex;
	delete districtIndex;
	delete primaryIndex;
	delete dataFile;
	delete fatherSearched;
	delete districtSearched;
	delete found;
}

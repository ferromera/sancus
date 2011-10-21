/*
 * ElectionFile.cpp
 *
 *  Created on: 19/10/2011
 *      Author: fernando
 */

#include "ElectionFile.h"

ElectionFile* ElectionFile::instance = NULL;

ElectionFile::ElectionFile() {
	dateIndex = new BPlusTree<SecondaryIndexRecord<Uint32Key, ElectionRecord::Key> ,
			ELECTION_FILE__SEC_INDEX_BLOCK_SIZE> (ELECTION_FILE_DATE_INDEX_PATH);
	districtIndex = new BPlusTree<SecondaryIndexRecord<DistrictRecord::Key, ElectionRecord::Key> ,
			ELECTION_FILE__SEC_INDEX_BLOCK_SIZE> (ELECTION_FILE_DISTRICT_INDEX_PATH);
	chargeIndex = new BPlusTree<SecondaryIndexRecord<ChargeRecord::Key, ElectionRecord::Key> ,
			ELECTION_FILE__SEC_INDEX_BLOCK_SIZE> (ELECTION_FILE_CHARGE_INDEX_PATH);
	primaryIndex = new BPlusTree<PrimaryIndexRecord<ElectionRecord::Key> , ELECTION_FILE__PRI_INDEX_BLOCK_SIZE> (
			ELECTION_FILE_PRIMARY_INDEX_PATH);
	ElectionRecord::Key highKey;
	highKey.setHighValue();
	PrimaryIndexRecord<ElectionRecord::Key> lastRecord(highKey, 1);
	try {
		primaryIndex->insert(lastRecord);
	} catch (LeafUnicityException) {
	}
	dataFile = new IndexedDataFile<ElectionRecord, ELECTION_FILE_DATA_BLOCK_SIZE> (ELECTION_FILE_DATA_PATH);
	lastSearch = NO_SEARCH;
	dateSearched = NULL;
	districtSearched = NULL;
	chargeSearched = NULL;
	found = NULL;

}

ElectionFile * ElectionFile::getInstance() {
	if (instance == NULL)
		instance = new ElectionFile();
	return instance;
}
void ElectionFile::insert(const ElectionRecord & record) {
	PrimaryIndexRecord<ElectionRecord::Key> indexToFind(record.getKey(), 0);
	PrimaryIndexRecord<ElectionRecord::Key>* primaryIndexFound = NULL;
	try {
		primaryIndexFound = new PrimaryIndexRecord<ElectionRecord::Key> (primaryIndex->search(indexToFind));
		dataFile->insert(record, primaryIndexFound->getBlockNumber());
		if (dataFile->overflow()) {
			ElectionRecord::Key newKey = dataFile->getNewKey();
			PrimaryIndexRecord<ElectionRecord::Key> newIndexRecord(newKey, primaryIndexFound->getBlockNumber());
			primaryIndex->insert(newIndexRecord);
			primaryIndexFound->setBlockNumber(dataFile->getNewBlock());
			primaryIndex->update(*primaryIndexFound);
		}
	} catch (ThereIsNoNextLeafException<PrimaryIndexRecord<ElectionRecord::Key> > e) {
		dataFile->insert(record, e.rec.getBlockNumber());
		if (dataFile->overflow()) {
			primaryIndex->remove(e.rec);
			ElectionRecord::Key newKey = dataFile->getNewKey();
			PrimaryIndexRecord<ElectionRecord::Key> newIndexRecord(newKey, e.rec.getBlockNumber());
			primaryIndex->insert(newIndexRecord);
			indexToFind.setBlockNumber(dataFile->getNewBlock());
			primaryIndex->insert(indexToFind);
		} else {
			primaryIndex->remove(e.rec);
			ElectionRecord::Key newKey = dataFile->getNewKey();
			indexToFind.setBlockNumber(e.rec.getBlockNumber());
			primaryIndex->insert(indexToFind);
		}
	} catch (IndexedDataRecordNotFoundException e) {
		throw FileInsertException();
	} catch (IndexedDataInsertException e) {
		throw FileInsertException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<Uint32Key, ElectionRecord::Key>* newDate;
	SecondaryIndexRecord<DistrictRecord::Key, ElectionRecord::Key>* newDistrict;
	SecondaryIndexRecord<ChargeRecord::Key, ElectionRecord::Key>* newCharge;
	newDistrict = new SecondaryIndexRecord<DistrictRecord::Key, ElectionRecord::Key> (record.getDistrict(),
			record.getKey());
	newCharge = new SecondaryIndexRecord<ChargeRecord::Key, ElectionRecord::Key> (record.getCharge(), record.getKey());
	Uint32Key dateKey = record.getDate();
	newDate = new SecondaryIndexRecord<Uint32Key, ElectionRecord::Key> (dateKey, record.getKey());
	districtIndex->insert(*newDistrict);
	chargeIndex->insert(*newCharge);
	dateIndex->insert(*newDate);
	delete newDistrict;
	delete newCharge;
	delete newDate;
}

void ElectionFile::remove(const ElectionRecord & record) {
	PrimaryIndexRecord<ElectionRecord::Key> indexToFind(record.getKey(), 0);
	PrimaryIndexRecord<ElectionRecord::Key>* primaryIndexFound = NULL;
	ElectionRecord * deletedRecord;
	try {
		primaryIndexFound = new PrimaryIndexRecord<ElectionRecord::Key> (primaryIndex->search(indexToFind));
		deletedRecord = new ElectionRecord(dataFile->search(record.getKey(), primaryIndexFound->getBlockNumber()));
		dataFile->remove(record, primaryIndexFound->getBlockNumber());
		if (dataFile->underflow()) {
			ElectionRecord::Key newKey = dataFile->getNewKey();
			if (dataFile->fusion()) {
				primaryIndex->remove(*primaryIndexFound);
				primaryIndexFound->setKey(newKey);
				primaryIndex->update(*primaryIndexFound);
			} else {
				primaryIndex->remove(*primaryIndexFound);
				primaryIndexFound->setKey(newKey);
				primaryIndex->insert(*primaryIndexFound);
			}
		}
	} catch (ThereIsNoNextLeafException<PrimaryIndexRecord<ElectionRecord::Key> > e) {
		throw FileRemoveException();
	} catch (IndexedDataRecordNotFoundException e) {
		throw FileRemoveException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<DistrictRecord::Key, ElectionRecord::Key> * deletedDistrict;
	SecondaryIndexRecord<Uint32Key, ElectionRecord::Key>* deletedDate;
	SecondaryIndexRecord<ChargeRecord::Key, ElectionRecord::Key>* deletedCharge;
	deletedDistrict = new SecondaryIndexRecord<DistrictRecord::Key, ElectionRecord::Key> (deletedRecord->getDistrict(),
			deletedRecord->getKey());
	Uint32Key dateKey = deletedRecord->getDate();
	deletedDate = new SecondaryIndexRecord<Uint32Key, ElectionRecord::Key> (dateKey, deletedRecord->getKey());
	deletedCharge = new SecondaryIndexRecord<ChargeRecord::Key, ElectionRecord::Key> (deletedRecord->getCharge(),
			deletedRecord->getKey());

	districtIndex->remove(*deletedDistrict);
	dateIndex->remove(*deletedDate);
	chargeIndex->remove(*deletedCharge);
	delete deletedDistrict;
	delete deletedDate;
	delete deletedCharge;
}
void ElectionFile::update(const ElectionRecord & record) {
	PrimaryIndexRecord<ElectionRecord::Key> indexToFind(record.getKey(), 0);
	PrimaryIndexRecord<ElectionRecord::Key>* primaryIndexFound = NULL;
	ElectionRecord * oldRecord;
	try {
		primaryIndexFound = new PrimaryIndexRecord<ElectionRecord::Key> (primaryIndex->search(indexToFind));
		oldRecord = new ElectionRecord(dataFile->search(record.getKey(), primaryIndexFound->getBlockNumber()));
		dataFile->update(record, primaryIndexFound->getBlockNumber());
		if (dataFile->overflow()) {
			ElectionRecord::Key newKey = dataFile->getNewKey();
			PrimaryIndexRecord<ElectionRecord::Key> newIndexRecord(newKey, primaryIndexFound->getBlockNumber());
			primaryIndex->insert(newIndexRecord);
			primaryIndexFound->setBlockNumber(dataFile->getNewBlock());
			primaryIndex->update(*primaryIndexFound);
		} else if (dataFile->underflow()) {
			ElectionRecord::Key newKey = dataFile->getNewKey();
			if (dataFile->fusion()) {
				primaryIndex->remove(*primaryIndexFound);
				primaryIndexFound->setKey(newKey);
				primaryIndex->update(*primaryIndexFound);
			} else {
				primaryIndex->remove(*primaryIndexFound);
				primaryIndexFound->setKey(newKey);
				primaryIndex->insert(*primaryIndexFound);
			}
		}
	} catch (ThereIsNoNextLeafException<PrimaryIndexRecord<ElectionRecord::Key> > e) {
		throw FileUpdateException();
	} catch (IndexedDataRecordNotFoundException e) {
		throw FileUpdateException();
	} catch (IndexedDataInsertException e) {
		throw FileUpdateException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<DistrictRecord::Key, ElectionRecord::Key>* oldDistrict, *newDistrict;
	SecondaryIndexRecord<Uint32Key, ElectionRecord::Key>* oldDate, *newDate;
	SecondaryIndexRecord<ChargeRecord::Key, ElectionRecord::Key>* oldCharge, *newCharge;
	oldDistrict = new SecondaryIndexRecord<DistrictRecord::Key, ElectionRecord::Key> (oldRecord->getDistrict(),
			oldRecord->getKey());
	Uint32Key oldDateKey(oldRecord->getDate());
	oldDate = new SecondaryIndexRecord<Uint32Key, ElectionRecord::Key> (oldDateKey, oldRecord->getKey());
	oldCharge = new SecondaryIndexRecord<ChargeRecord::Key, ElectionRecord::Key> (oldRecord->getCharge(),
			oldRecord->getKey());

	newDistrict = new SecondaryIndexRecord<DistrictRecord::Key, ElectionRecord::Key> (record.getDistrict(),
			record.getKey());
	Uint32Key newDateKey(record.getDate());
	newDate = new SecondaryIndexRecord<Uint32Key, ElectionRecord::Key> (newDateKey, record.getKey());
	newCharge = new SecondaryIndexRecord<ChargeRecord::Key, ElectionRecord::Key> (record.getCharge(), record.getKey());

	districtIndex->remove(*oldDistrict);
	dateIndex->remove(*oldDate);
	chargeIndex->remove(*oldCharge);
	districtIndex->insert(*newDistrict);
	dateIndex->insert(*newDate);
	chargeIndex->insert(*newCharge);
	delete oldDistrict;
	delete oldDate;
	delete oldCharge;
	delete newDistrict;
	delete newDate;
	delete newCharge;
	delete oldRecord;
}
const ElectionRecord & ElectionFile::searchByDate(unsigned int date) {
	lastSearch = DATE_SEARCH;
	delete dateSearched;
	try {
		dateSearched = new Uint32Key(date);
		SecondaryIndexRecord<Uint32Key, ElectionRecord::Key> firstSecIndex(*dateSearched);
		firstSecIndex = dateIndex->search(firstSecIndex);
		if (firstSecIndex.getAttribute() != (*dateSearched))
			throw FileSearchException();
		ElectionRecord::Key elecKey = firstSecIndex.getPrimary();
		PrimaryIndexRecord<ElectionRecord::Key> indexToFind(elecKey, 0);
		indexToFind = primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(), indexToFind.getBlockNumber());
	} catch (ThereIsNoNextLeafException<SecondaryIndexRecord<DistrictRecord::Key, ElectionRecord::Key> > ) {
		throw FileSearchException();
	}
}
const ElectionRecord & ElectionFile::searchByCharge(const ChargeRecord::Key & charge) {
	lastSearch = CHARGE_SEARCH;
	delete chargeSearched;
	try {
		chargeSearched = new ChargeRecord::Key(charge);
		SecondaryIndexRecord<ChargeRecord::Key, ElectionRecord::Key> firstSecIndex(charge);
		firstSecIndex = chargeIndex->search(firstSecIndex);
		if (firstSecIndex.getAttribute() != (*chargeSearched))
			throw FileSearchException();
		ElectionRecord::Key elecKey = firstSecIndex.getPrimary();
		PrimaryIndexRecord<ElectionRecord::Key> indexToFind(elecKey, 0);
		indexToFind = primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(), indexToFind.getBlockNumber());
	} catch (ThereIsNoNextLeafException<SecondaryIndexRecord<DistrictRecord::Key, ElectionRecord::Key> > ) {
		throw FileSearchException();
	}
}
const ElectionRecord & ElectionFile::searchByDistrict(const DistrictRecord::Key & district) {
	lastSearch = DISTRICT_SEARCH;
	delete districtSearched;
	districtSearched = new DistrictRecord::Key(district);
	try {
		SecondaryIndexRecord<DistrictRecord::Key, ElectionRecord::Key> firstSecIndex(district);
		firstSecIndex = districtIndex->search(firstSecIndex);
		if (firstSecIndex.getAttribute() != (*districtSearched))
			throw FileSearchException();
		ElectionRecord::Key elecKey = firstSecIndex.getPrimary();
		PrimaryIndexRecord<ElectionRecord::Key> indexToFind(elecKey, 0);
		indexToFind = primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(), indexToFind.getBlockNumber());
	} catch (ThereIsNoNextLeafException<SecondaryIndexRecord<DistrictRecord::Key, ElectionRecord::Key> > ) {
		throw FileSearchException();
	}

}
const ElectionRecord & ElectionFile::search(const ElectionRecord::Key & election) {
	lastSearch = PRIMARY_SEARCH;
	try {
		PrimaryIndexRecord<ElectionRecord::Key> indexToFind(election, 0);
		indexToFind = primaryIndex->search(indexToFind);
		return dataFile->search(election, indexToFind.getBlockNumber());
	} catch (ThereIsNoNextLeafException<PrimaryIndexRecord<ElectionRecord::Key> > ) {
		throw FileSearchException();
	} catch (IndexedDataRecordNotFoundException) {
		throw FileSearchException();
	}
}
const ElectionRecord & ElectionFile::nextDate() {
	if (lastSearch != DATE_SEARCH)
		throw FileNextException();
	try {
		SecondaryIndexRecord<Uint32Key, ElectionRecord::Key> firstSecIndex = dateIndex->next();
		if (firstSecIndex.getAttribute() != (*dateSearched))
			throw FileNextException();
		ElectionRecord::Key elecKey = firstSecIndex.getPrimary();
		PrimaryIndexRecord<ElectionRecord::Key> indexToFind(elecKey, 0);
		indexToFind = primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(), indexToFind.getBlockNumber());
	} catch (ThereIsNoNextLeafException<SecondaryIndexRecord<Uint32Key, ElectionRecord::Key> > ) {
		throw FileNextException();
	}
}
const ElectionRecord & ElectionFile::nextDistrict() {
	if (lastSearch != DISTRICT_SEARCH)
		throw FileNextException();
	try {
		SecondaryIndexRecord<DistrictRecord::Key, ElectionRecord::Key> firstSecIndex = districtIndex->next();
		if (firstSecIndex.getAttribute() != (*districtSearched))
			throw FileNextException();
		ElectionRecord::Key elecKey = firstSecIndex.getPrimary();
		PrimaryIndexRecord<ElectionRecord::Key> indexToFind(elecKey, 0);
		indexToFind = primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(), indexToFind.getBlockNumber());
	} catch (ThereIsNoNextLeafException<SecondaryIndexRecord<DistrictRecord::Key, ElectionRecord::Key> > ) {
		throw FileNextException();
	}
}
const ElectionRecord & ElectionFile::nextCharge() {
	if (lastSearch != CHARGE_SEARCH)
		throw FileNextException();
	try {
		SecondaryIndexRecord<ChargeRecord::Key, ElectionRecord::Key> firstSecIndex = chargeIndex->next();
		if (firstSecIndex.getAttribute() != (*chargeSearched))
			throw FileNextException();
		ElectionRecord::Key elecKey = firstSecIndex.getPrimary();
		PrimaryIndexRecord<ElectionRecord::Key> indexToFind(elecKey, 0);
		indexToFind = primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(), indexToFind.getBlockNumber());
	} catch (ThereIsNoNextLeafException<SecondaryIndexRecord<ChargeRecord::Key, ElectionRecord::Key> > ) {
		throw FileNextException();
	}
}

const ElectionRecord & ElectionFile::next() {
	if (lastSearch != PRIMARY_SEARCH)
		throw FileNextException();
	try {
		delete found;
		found = new ElectionRecord(dataFile->next());
		return *found;
	} catch (IndexedDataNextException e) {
		throw FileNextException();
	}

}
void ElectionFile::report() {
	districtIndex->preOrderReport();
	dateIndex->preOrderReport();
	chargeIndex->preOrderReport();
	primaryIndex->preOrderReport();
}

ElectionFile::~ElectionFile() {
	delete dateIndex;
	delete districtIndex;
	delete chargeIndex;
	delete primaryIndex;
	delete dataFile;
	delete dateSearched;
	delete districtSearched;
	delete chargeSearched;
	delete found;
}

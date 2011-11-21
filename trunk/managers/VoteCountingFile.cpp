/*
 * VoteCountingFile.cpp
 *
 *  Created on: 21/09/2011
 *      Author: juanmromera
 */

#include "VoteCountingFile.h"
#include "../utils/Time.h"

VoteCountingFile* VoteCountingFile::instance = NULL;

VoteCountingFile::VoteCountingFile() {

	electionIndex = new BPlusTree<SecondaryIndexRecord<ElectionRecord::Key, VoteCountingRecord::Key> ,
			VOTE_COUNTING_FILE__SEC_INDEX_BLOCK_SIZE> (VOTE_COUNTING_ELECTION_INDEX_PATH);
	districtIndex = new BPlusTree<SecondaryIndexRecord<DistrictRecord::Key, VoteCountingRecord::Key> ,
			VOTE_COUNTING_FILE__SEC_INDEX_BLOCK_SIZE> (VOTE_COUNTING_DISTRICT_INDEX_PATH);
	listIndex = new BPlusTree<SecondaryIndexRecord<ListRecord::Key, VoteCountingRecord::Key> ,
			VOTE_COUNTING_FILE__SEC_INDEX_BLOCK_SIZE> (VOTE_COUNTING_LIST_INDEX_PATH);
	primaryIndex
			= new BPlusTree<PrimaryIndexRecord<VoteCountingRecord::Key> , VOTE_COUNTING_FILE__PRI_INDEX_BLOCK_SIZE> (
					VOTE_COUNTING_PRIMARY_INDEX_PATH);
	VoteCountingRecord::Key highKey;
	highKey.setHighValue();
	PrimaryIndexRecord<VoteCountingRecord::Key> lastRecord(highKey, 1);
	try {
		primaryIndex->insert(lastRecord);
	} catch (LeafUnicityException) {
	}
	dataFile = new IndexedDataFile<VoteCountingRecord, VOTE_COUNTING_FILE_DATA_BLOCK_SIZE> (VOTE_COUNTING_DATA_PATH);
	lastSearch = NO_SEARCH;
	electionSearched = NULL;
	districtSearched = NULL;
	listSearched = NULL;
	found = NULL;

}
VoteCountingFile* VoteCountingFile::getInstance() {
	if (instance == NULL)
		instance = new VoteCountingFile();
	return instance;
}

void VoteCountingFile::insert(const VoteCountingRecord & record) {
	PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(record.getKey(), 0);
	PrimaryIndexRecord<VoteCountingRecord::Key>* primaryIndexFound = NULL;
	try {
		primaryIndexFound = new PrimaryIndexRecord<VoteCountingRecord::Key> (primaryIndex->search(indexToFind));
		dataFile->insert(record, primaryIndexFound->getBlockNumber());
		if (dataFile->overflow()) {
			VoteCountingRecord::Key newKey = dataFile->getNewKey();
			PrimaryIndexRecord<VoteCountingRecord::Key> newIndexRecord(newKey, primaryIndexFound->getBlockNumber());
			primaryIndex->insert(newIndexRecord);
			primaryIndexFound->setBlockNumber(dataFile->getNewBlock());
			primaryIndex->update(*primaryIndexFound);
		}
	} catch (ThereIsNoNextLeafException e) {
		/*dataFile->insert(record, e.rec.getBlockNumber());
		if (dataFile->overflow()) {
			primaryIndex->remove(e.rec);
			VoteCountingRecord::Key newKey = dataFile->getNewKey();
			PrimaryIndexRecord<VoteCountingRecord::Key> newIndexRecord(newKey, e.rec.getBlockNumber());
			primaryIndex->insert(newIndexRecord);
			indexToFind.setBlockNumber(dataFile->getNewBlock());
			primaryIndex->insert(indexToFind);
		} else {
			primaryIndex->remove(e.rec);
			VoteCountingRecord::Key newKey = dataFile->getNewKey();
			indexToFind.setBlockNumber(e.rec.getBlockNumber());
			primaryIndex->insert(indexToFind);
		}*/
	} catch (IndexedDataRecordNotFoundException e) {
		throw FileInsertException();
	} catch (IndexedDataInsertException e) {
		throw FileInsertException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<ElectionRecord::Key, VoteCountingRecord::Key>* newElection;
	SecondaryIndexRecord<DistrictRecord::Key, VoteCountingRecord::Key>* newDistrict;
	SecondaryIndexRecord<ListRecord::Key, VoteCountingRecord::Key>* newList;
	newElection = new SecondaryIndexRecord<ElectionRecord::Key, VoteCountingRecord::Key> (record.getElection(),
			record.getKey());
	newDistrict = new SecondaryIndexRecord<DistrictRecord::Key, VoteCountingRecord::Key> (record.getDistrict(),
			record.getKey());
	newList = new SecondaryIndexRecord<ListRecord::Key, VoteCountingRecord::Key> (record.getList(), record.getKey());
	electionIndex->insert(*newElection);
	districtIndex->insert(*newDistrict);
	listIndex->insert(*newList);
	delete newElection;
	delete newDistrict;
	delete newList;

}

void VoteCountingFile::remove(const VoteCountingRecord & record) {
	PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(record.getKey(), 0);
	PrimaryIndexRecord<VoteCountingRecord::Key>* primaryIndexFound = NULL;
	VoteCountingRecord * deletedRecord;
	try {
		primaryIndexFound = new PrimaryIndexRecord<VoteCountingRecord::Key> (primaryIndex->search(indexToFind));
		deletedRecord = new VoteCountingRecord(dataFile->search(record.getKey(), primaryIndexFound->getBlockNumber()));
		dataFile->remove(record, primaryIndexFound->getBlockNumber());
		if (dataFile->underflow()) {
			VoteCountingRecord::Key newKey = dataFile->getNewKey();
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
	} catch (ThereIsNoNextLeafException e) {
		throw FileRemoveException();
	} catch (IndexedDataRecordNotFoundException e) {
		throw FileRemoveException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<ElectionRecord::Key, VoteCountingRecord::Key>* deletedElection;
	SecondaryIndexRecord<DistrictRecord::Key, VoteCountingRecord::Key>* deletedDistrict;
	SecondaryIndexRecord<ListRecord::Key, VoteCountingRecord::Key>* deletedList;
	deletedElection = new SecondaryIndexRecord<ElectionRecord::Key, VoteCountingRecord::Key> (
			deletedRecord->getElection(), deletedRecord->getKey());
	deletedDistrict = new SecondaryIndexRecord<DistrictRecord::Key, VoteCountingRecord::Key> (
			deletedRecord->getDistrict(), deletedRecord->getKey());
	deletedList = new SecondaryIndexRecord<ListRecord::Key, VoteCountingRecord::Key> (deletedRecord->getList(),
			deletedRecord->getKey());
	electionIndex->remove(*deletedElection);
	districtIndex->remove(*deletedDistrict);
	listIndex->remove(*deletedList);
	delete deletedElection;
	delete deletedDistrict;
	delete deletedList;
	delete deletedRecord;

}

void VoteCountingFile::update(const VoteCountingRecord & record) {
	PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(record.getKey(), 0);
	PrimaryIndexRecord<VoteCountingRecord::Key>* primaryIndexFound = NULL;
	VoteCountingRecord * oldRecord;
	try {
		primaryIndexFound = new PrimaryIndexRecord<VoteCountingRecord::Key> (primaryIndex->search(indexToFind));
		oldRecord = new VoteCountingRecord(dataFile->search(record.getKey(), primaryIndexFound->getBlockNumber()));
		dataFile->update(record, primaryIndexFound->getBlockNumber());
		if (dataFile->overflow()) {
			VoteCountingRecord::Key newKey = dataFile->getNewKey();
			PrimaryIndexRecord<VoteCountingRecord::Key> newIndexRecord(newKey, primaryIndexFound->getBlockNumber());
			primaryIndex->insert(newIndexRecord);
			primaryIndexFound->setBlockNumber(dataFile->getNewBlock());
			primaryIndex->update(*primaryIndexFound);
		} else if (dataFile->underflow()) {
			VoteCountingRecord::Key newKey = dataFile->getNewKey();
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
	} catch (ThereIsNoNextLeafException e) {
		throw FileUpdateException();
	} catch (IndexedDataRecordNotFoundException e) {
		throw FileUpdateException();
	} catch (IndexedDataInsertException e) {
		throw FileUpdateException();
	}
	delete primaryIndexFound;
	SecondaryIndexRecord<ElectionRecord::Key, VoteCountingRecord::Key>* oldElection, *newElection;
	SecondaryIndexRecord<DistrictRecord::Key, VoteCountingRecord::Key>* oldDistrict, *newDistrict;
	SecondaryIndexRecord<ListRecord::Key, VoteCountingRecord::Key>* oldList, *newList;
	oldElection = new SecondaryIndexRecord<ElectionRecord::Key, VoteCountingRecord::Key> (oldRecord->getElection(),
			oldRecord->getKey());
	oldDistrict = new SecondaryIndexRecord<DistrictRecord::Key, VoteCountingRecord::Key> (oldRecord->getDistrict(),
			oldRecord->getKey());
	oldList = new SecondaryIndexRecord<ListRecord::Key, VoteCountingRecord::Key> (oldRecord->getList(),
			oldRecord->getKey());
	newElection = new SecondaryIndexRecord<ElectionRecord::Key, VoteCountingRecord::Key> (record.getElection(),
			record.getKey());
	newDistrict = new SecondaryIndexRecord<DistrictRecord::Key, VoteCountingRecord::Key> (record.getDistrict(),
			record.getKey());
	newList = new SecondaryIndexRecord<ListRecord::Key, VoteCountingRecord::Key> (record.getList(), record.getKey());
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

const VoteCountingRecord & VoteCountingFile::searchByDistrict(const DistrictRecord::Key & district) {
	lastSearch = DISTRICT_SEARCH;
	delete districtSearched;
	try {
		districtSearched = new DistrictRecord::Key(district);
		SecondaryIndexRecord<DistrictRecord::Key, VoteCountingRecord::Key> firstSecIndex(district);
		firstSecIndex = districtIndex->search(firstSecIndex);
		if (firstSecIndex.getAttribute() != district)
			throw FileSearchException();
		VoteCountingRecord::Key voteCountKey = firstSecIndex.getPrimary();
		PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(voteCountKey, 0);
		indexToFind = primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(), indexToFind.getBlockNumber());
	} catch (ThereIsNoNextLeafException ) {
		throw FileSearchException();
	}

}

const VoteCountingRecord & VoteCountingFile::searchByElection(const ElectionRecord::Key & election) {
	lastSearch = ELECTION_SEARCH;
	delete electionSearched;
	try {
		electionSearched = new ElectionRecord::Key(election);
		SecondaryIndexRecord<ElectionRecord::Key, VoteCountingRecord::Key> firstSecIndex(election);
		firstSecIndex = electionIndex->search(firstSecIndex);
		if (firstSecIndex.getAttribute() != election)
			throw FileSearchException();
		VoteCountingRecord::Key voteCountKey = firstSecIndex.getPrimary();
		PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(voteCountKey, 0);
		indexToFind = primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(), indexToFind.getBlockNumber());
	} catch (ThereIsNoNextLeafException ) {
		throw FileSearchException();
	}

}

const VoteCountingRecord & VoteCountingFile::searchByList(const ListRecord::Key & list) {
	lastSearch = LIST_SEARCH;
	delete listSearched;
	try {
		listSearched = new ListRecord::Key(list);
		SecondaryIndexRecord<ListRecord::Key, VoteCountingRecord::Key> firstSecIndex(list);
		firstSecIndex = listIndex->search(firstSecIndex);
		if (firstSecIndex.getAttribute() != list)
			throw FileSearchException();
		VoteCountingRecord::Key voteCountKey = firstSecIndex.getPrimary();
		PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(voteCountKey, 0);
		indexToFind = primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(), indexToFind.getBlockNumber());
	} catch (ThereIsNoNextLeafException ) {
		throw FileSearchException();
	}
}

const VoteCountingRecord & VoteCountingFile::search(const VoteCountingRecord::Key & voteCountKey) {
	lastSearch = PRIMARY_SEARCH;
	try {
		PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(voteCountKey, 0);
		indexToFind = primaryIndex->search(indexToFind);
		return dataFile->search(voteCountKey, indexToFind.getBlockNumber());
	} catch (ThereIsNoNextLeafException ) {
		throw FileSearchException();
	} catch (IndexedDataRecordNotFoundException) {
		throw FileSearchException();
	}
}

const VoteCountingRecord & VoteCountingFile::nextDistrict() {
	if (lastSearch != DISTRICT_SEARCH)
		throw FileNextException();
	try {
		SecondaryIndexRecord<DistrictRecord::Key, VoteCountingRecord::Key> firstSecIndex = districtIndex->next();
		if (firstSecIndex.getAttribute() != (*districtSearched))
			throw FileNextException();
		VoteCountingRecord::Key voteKey = firstSecIndex.getPrimary();
		PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(voteKey, 0);
		indexToFind = primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(), indexToFind.getBlockNumber());
	} catch (ThereIsNoNextLeafException) {
		throw FileNextException();
	}

}

const VoteCountingRecord & VoteCountingFile::nextElection() {
	if (lastSearch != ELECTION_SEARCH)
		throw FileNextException();
	try {
		SecondaryIndexRecord<ElectionRecord::Key, VoteCountingRecord::Key> firstSecIndex = electionIndex->next();
		if (firstSecIndex.getAttribute() != (*electionSearched))
			throw FileNextException();
		VoteCountingRecord::Key voteKey = firstSecIndex.getPrimary();
		PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(voteKey, 0);
		indexToFind = primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(), indexToFind.getBlockNumber());
	} catch (ThereIsNoNextLeafException ) {
		throw FileNextException();
	}
}
const VoteCountingRecord & VoteCountingFile::nextList() {
	if (lastSearch != LIST_SEARCH)
		throw FileNextException();
	try {
		SecondaryIndexRecord<ListRecord::Key, VoteCountingRecord::Key> firstSecIndex = listIndex->next();
		if (firstSecIndex.getAttribute() != (*listSearched))
			throw FileNextException();
		VoteCountingRecord::Key voteKey = firstSecIndex.getPrimary();
		PrimaryIndexRecord<VoteCountingRecord::Key> indexToFind(voteKey, 0);
		indexToFind = primaryIndex->search(indexToFind);
		return dataFile->search(firstSecIndex.getPrimary(), indexToFind.getBlockNumber());
	} catch (ThereIsNoNextLeafException ) {
		throw FileNextException();
	}
}
const VoteCountingRecord & VoteCountingFile::next() {
	if (lastSearch != PRIMARY_SEARCH)
		throw FileNextException();
	try {
		return dataFile->next();
	} catch (IndexedDataNextException e) {
		throw FileNextException();
	}
}

void VoteCountingFile::report() {
	listIndex->preOrderReport();
	electionIndex->preOrderReport();
	districtIndex->preOrderReport();
	primaryIndex->preOrderReport();
}
void VoteCountingFile::createReportFile() {
	std::string nameFile = "reportFileVoteCounting";
	nameFile.append(Time::getTime());

	File reportFile = File(nameFile, File::NEW);
	VoteCountingRecord::Key key = VoteCountingRecord::Key();
	this->search(key);
	while (true) {
		try {
			VoteCountingRecord record = this->next();
			reportFile << "Lista: " << record.getList().getString()
					<< " Eleccion: " << record.getElection().getString()
					<< " Distrito: " << record.getDistrict().getString()
					<< " Votos: " << record.getCount()
					<< "\n";
		} catch (FileNextException e) {
			break;
		}
	}
	std::cout << "Se ha creado el archivo " << nameFile << " con Exito" << std::endl;

}

VoteCountingFile::~VoteCountingFile() {
	delete electionIndex;
	delete districtIndex;
	delete listIndex;
	delete primaryIndex;
	delete dataFile;
	delete electionSearched;
	delete districtSearched;
	delete listSearched;
	delete found;
}


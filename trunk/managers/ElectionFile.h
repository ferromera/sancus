/*
 * ElectionFile.h
 *
 *  Created on: 19/10/2011
 *      Author: fernando
 */

#ifndef ELECTIONFILE_H_
#define ELECTIONFILE_H_

#define ELECTION_FILE_DATE_INDEX_PATH "./ElectionFileDateIndex.bin"
#define ELECTION_FILE_CHARGE_INDEX_PATH "./ElectionFileChargeIndex.bin"
#define ELECTION_FILE_DISTRICT_INDEX_PATH "./ElectionFileDistrictIndex.bin"
#define ELECTION_FILE_PRIMARY_INDEX_PATH "./ElectionFilePrimaryIndex.bin"
#define ELECTION_FILE_DATA_PATH	"./ElectionFileData.bin"

#include "FileManagerExceptions.h"
#include "../estructuras/BPlusTree.h"
#include "../estructuras/IndexedDataFile.h"
#include "../records/SecondaryIndexRecord.h"
#include "../records/PrimaryIndexRecord.h"
#include "../records/ElectionRecord.h"
#include "../estructuras/Uint32Key.h"

class ElectionFile {
	enum lastSearchEnum {DATE_SEARCH,DISTRICT_SEARCH,CHARGE_SEARCH,PRIMARY_SEARCH,NO_SEARCH};
	BPlusTree<SecondaryIndexRecord<Uint32Key,ElectionRecord::Key>,4096> * dateIndex;
	BPlusTree<SecondaryIndexRecord<ChargeRecord::Key,ElectionRecord::Key>,4096> * chargeIndex;
	BPlusTree<SecondaryIndexRecord<DistrictRecord::Key,ElectionRecord::Key>,4096> * districtIndex;

	BPlusTree<PrimaryIndexRecord<ElectionRecord::Key>,4096> * primaryIndex;
	IndexedDataFile<ElectionRecord,8192> * dataFile;
	enum lastSearchEnum lastSearch;

	Uint32Key * dateSearched;
	ChargeRecord::Key * chargeSearched;
	DistrictRecord::Key * districtSearched;
	ElectionRecord * found;

	static ElectionFile * instance;

public:
	ElectionFile();

	static ElectionFile * getInstance();

	void insert(const ElectionRecord &);
	void remove(const ElectionRecord &);
	void update(const ElectionRecord &);
	const ElectionRecord & searchByDate(unsigned int date);
	const ElectionRecord & searchByCharge(const ChargeRecord::Key & charge);
	const ElectionRecord & searchByDistrict(const DistrictRecord::Key & district);
	const ElectionRecord & search(const ElectionRecord::Key & election);
	const ElectionRecord & nextDate();
	const ElectionRecord & nextCharge();
	const ElectionRecord & nextDistrict();
	const ElectionRecord & next();
	void report();

	virtual ~ElectionFile();
};

#endif /* ELECTIONFILE_H_ */



class ChargeFile {
	enum lastSearchEnum {FATHER_SEARCH,DISTRICT_SEARCH,PRIMARY_SEARCH,NO_SEARCH};
		BPlusTree<SecondaryIndexRecord<ChargeRecord::Key,ChargeRecord::Key>,4096> * fatherIndex;
		BPlusTree<SecondaryIndexRecord<DistrictRecord::Key,ChargeRecord::Key>,4096> * districtIndex;

		BPlusTree<PrimaryIndexRecord<ChargeRecord::Key>,4096> * primaryIndex;
		IndexedDataFile<ChargeRecord,8192> * dataFile;
		enum lastSearchEnum lastSearch;

		ChargeRecord::Key * fatherSearched;
		DistrictRecord::Key * districtSearched;
		ChargeRecord * found;


		static ChargeFile * instance;

public:
	ChargeFile();
	static ChargeFile * getInstance();

	void insert(const ChargeRecord &);
	void remove(const ChargeRecord &);
	void update(const ChargeRecord &);
	const ChargeRecord & searchByFather(const ChargeRecord::Key & father);
	const ChargeRecord & searchByDistrict(const DistrictRecord::Key & district);
	const ChargeRecord & search(const ChargeRecord::Key & charge);
	const ChargeRecord & nextFather();
	const ChargeRecord & nextDistrict();
	const ChargeRecord & next();
	void report();
	virtual ~ChargeFile();
};

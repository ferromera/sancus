/*
 * ChargeFile.h
 *
 *  Created on: 19/10/2011
 *      Author: fernando
 */

#ifndef CHARGEFILE_H_
#define CHARGEFILE_H_

#define CHARGE_FILE_FATHER_INDEX_PATH "./ChargeFileFatherIndex.bin"
#define CHARGE_FILE_DISTRICT_INDEX_PATH "./ChargeFileDistrictIndex.bin"
#define CHARGE_FILE_PRIMARY_INDEX_PATH "./ChargeFilePrimaryIndex.bin"
#define CHARGE_FILE_DATA_PATH	"./ChargeFileData.bin"

#include "FileManagerExceptions.h"
#include "../estructuras/BPlusTree.h"
#include "../estructuras/IndexedDataFile.h"
#include "../records/SecondaryIndexRecord.h"
#include "../records/PrimaryIndexRecord.h"
#include "../records/ChargeRecord.h"

class ChargeFile {
	enum lastSearchEnum {
		FATHER_SEARCH, DISTRICT_SEARCH, PRIMARY_SEARCH, NO_SEARCH
	};
	BPlusTree<SecondaryIndexRecord<ChargeRecord::Key, ChargeRecord::Key> , 4096> * fatherIndex;
	BPlusTree<SecondaryIndexRecord<DistrictRecord::Key, ChargeRecord::Key> , 4096> * districtIndex;

	BPlusTree<PrimaryIndexRecord<ChargeRecord::Key> , 4096> * primaryIndex;
	IndexedDataFile<ChargeRecord, 8192> * dataFile;
	enum lastSearchEnum lastSearch;

	ChargeRecord::Key * fatherSearched;
	DistrictRecord::Key * districtSearched;
	ChargeRecord * found;

	static ChargeFile * instance;

public:
	ChargeFile();
	static ChargeFile * getInstance();
	static void deleteInstance() {
		delete instance;
		instance = NULL;
	}

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
	void createReportFile();
	virtual ~ChargeFile();
};

#endif /* CHARGEFILE_H_ */

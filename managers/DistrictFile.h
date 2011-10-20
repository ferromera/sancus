/*
 * DistrictFile.h
 *
 *  Created on: Oct 16, 2011
 *      Author: alfredo
 */

#ifndef DISTRICTFILE_H_
#define DISTRICTFILE_H_

#define DISTRICT_FILE_BPLUS

#ifdef DISTRICT_FILE_BPLUS

#include "../records/DistrictRecord.h"
#include "../estructuras/BPlusTree.h"

#define DISTRICT_FILE_DATA_PATH	"DistrictFileData.bin"
#define DISTRICT_FILE_BLOCKSIZE 1024


class DistrictFile {
private:
	BPlusTree<DistrictRecord,DISTRICT_FILE_BLOCKSIZE> * tree;
	static DistrictFile * instance;
public:
	static DistrictFile * getInstance();

	DistrictFile();

	void insert(const DistrictRecord &);
	void remove(const DistrictRecord &);
	void update(const DistrictRecord &);
	void report();
	const DistrictRecord & search(const DistrictRecord::Key & district);
	const DistrictRecord & next();

	virtual ~DistrictFile();
};

#else

#include "../records/DistrictRecord.h"
#include "../estructuras/HashTable.h"

#define DISTRICT_FILE_DATA_PATH	"DistrictFileData.bin"
#define DISTRICT_FILE_RECORDS_PER_BUCKET 50
#define DISTRICT_FILE_MAX_NUMBER_OF_RECORDS	10000

class DistrictFile {
private:
	HashTable<DistrictRecord,4096> * table;

	static DistrictFile * instance;
public:
	static DistrictFile * getInstance();

	DistrictFile();

	void insert(const DistrictRecord &);
	void remove(const DistrictRecord &);
	void update(const DistrictRecord &);
	const DistrictRecord & search(const DistrictRecord::Key & district);

	virtual ~DistrictFile();
};



#endif  // DISTRICT_FILE_BPLUS

#endif /* DISTRICTFILE_H_ */

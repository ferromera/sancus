/*
 * DistrictFile.h
 *
 *  Created on: Oct 16, 2011
 *      Author: alfredo
 */

#ifndef DISTRICTFILE_H_
#define DISTRICTFILE_H_

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

#endif /* DISTRICTFILE_H_ */

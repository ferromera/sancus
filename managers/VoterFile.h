/*
 * VoterFile.h
 *
 *  Created on: 09/10/2011
 *      Author: cecilia
 */

#ifndef VOTERFILE_H_
#define VOTERFILE_H_

#include "../records/VoterRecord.h"
#include "../estructuras/HashTable.h"
#include "FileManagerExceptions.h"

#define VOTER_FILE_DATA_PATH	 "VoterFileData.bin"
#define VOTER_FILE_RECORDS_PER_BUCKET 50
#define VOTER_FILE_MAX_NUMBER_OF_RECORDS 10000

class VoterFile {
private:
	HashTable<VoterRecord, 4096> * table;
	static VoterFile * instance;
public:
	static VoterFile * getInstance();
	static void deleteInstance() {
		delete instance;
		instance = NULL;
	}

	VoterFile();

	virtual ~VoterFile();
	void insert(const VoterRecord &);
	void remove(const VoterRecord &);
	void update(const VoterRecord &);
	void createReportFile();
	const VoterRecord & search(const VoterRecord::Key &);
};

#endif /* VOTERFILE_H_ */

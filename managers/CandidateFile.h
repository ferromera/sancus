/*
 * CandidateFile.h
 *
 *  Created on: 18/10/2011
 *      Author: cecilia
 */

#ifndef CANDIDATEFILE_H_
#define CANDIDATEFILE_H_
#include "../records/CandidateRecord.h"
#include "../estructuras/HashTable.h"

#define CANDIDATE_FILE_DATA_PATH	"CandidateFileData.bin"
#define CANDIDATE_FILE_RECORDS_PER_BUCKET 50
#define CANDIDATE_FILE_MAX_NUMBER_OF_RECORDS	10000

class CandidateFile {
private:
	HashTable<CandidateRecord,4096> * table;
	static CandidateFile * instance;
public:
	static CandidateFile * getInstance();

	CandidateFile();

	void insert(const CandidateRecord &);
	void remove(const CandidateRecord &);
	void update(const CandidateRecord &);
	const CandidateRecord & search(const CandidateRecord::Key &);

	virtual ~CandidateFile();
};


#endif /* CANDIDATEFILE_H_ */

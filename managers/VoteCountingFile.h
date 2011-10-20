/*
 * VoteCountingFile.h
 *
 *  Created on: 21/09/2011
 *      Author: juanmromera
 */

#ifndef VOTECOUNTINGFILE_H_
#define VOTECOUNTINGFILE_H_

#include <list>
#include <string>
#include "../records/VoteCountingRecord.h"
#include "FileManagerExceptions.h"
#include "../estructuras/BPlusTree.h"
#include "../estructuras/IndexedDataFile.h"
#include "../records/SecondaryIndexRecord.h"
#include "../records/PrimaryIndexRecord.h"

#define VOTE_COUNTING_ELECTION_INDEX_PATH "./VoteCountingElectionIndex.bin"
#define VOTE_COUNTING_DISTRICT_INDEX_PATH "./VoteCountingDistrictIndex.bin"
#define VOTE_COUNTING_LIST_INDEX_PATH "./VoteCountingListIndex.bin"
#define VOTE_COUNTING_PRIMARY_INDEX_PATH	"./VoteCountingPrimaryIndex.bin"
#define VOTE_COUNTING_DATA_PATH	"./VoteCountingData.bin"

#define VOTE_COUNTING_FILE__SEC_INDEX_BLOCK_SIZE 1024
#define VOTE_COUNTING_FILE__PRI_INDEX_BLOCK_SIZE 512
#define VOTE_COUNTING_FILE_DATA_BLOCK_SIZE 1024

class VoteCountingFile {
	enum lastSearchEnum {
		ELECTION_SEARCH, DISTRICT_SEARCH, LIST_SEARCH, PRIMARY_SEARCH, NO_SEARCH
	};

	BPlusTree<SecondaryIndexRecord<ElectionRecord::Key, VoteCountingRecord::Key> ,
			VOTE_COUNTING_FILE__SEC_INDEX_BLOCK_SIZE> * electionIndex;
	BPlusTree<SecondaryIndexRecord<DistrictRecord::Key, VoteCountingRecord::Key> ,
			VOTE_COUNTING_FILE__SEC_INDEX_BLOCK_SIZE> * districtIndex;
	BPlusTree<SecondaryIndexRecord<ListRecord::Key, VoteCountingRecord::Key> , VOTE_COUNTING_FILE__SEC_INDEX_BLOCK_SIZE>
			* listIndex;
	BPlusTree<PrimaryIndexRecord<VoteCountingRecord::Key> , VOTE_COUNTING_FILE__PRI_INDEX_BLOCK_SIZE> * primaryIndex;
	IndexedDataFile<VoteCountingRecord, VOTE_COUNTING_FILE_DATA_BLOCK_SIZE> * dataFile;
	enum lastSearchEnum lastSearch;

	ElectionRecord::Key * electionSearched;
	DistrictRecord::Key * districtSearched;
	ListRecord::Key * listSearched;
	VoteCountingRecord * found;

	static VoteCountingFile * instance;

public:

	static VoteCountingFile * getInstance();
	static void deleteInstance() {
		delete instance;
		instance = NULL;
	}

	VoteCountingFile();

	void insert(const VoteCountingRecord &);
	void remove(const VoteCountingRecord &);
	void update(const VoteCountingRecord &);
	const VoteCountingRecord & searchByDistrict(const DistrictRecord::Key & district);
	const VoteCountingRecord & searchByElection(const ElectionRecord::Key & election);
	const VoteCountingRecord & searchByList(const ListRecord::Key & list);
	const VoteCountingRecord & search(const VoteCountingRecord::Key & voteCounting);
	const VoteCountingRecord & nextDistrict();
	const VoteCountingRecord & nextElection();
	const VoteCountingRecord & nextList();
	const VoteCountingRecord & next();
	void report();

	virtual ~VoteCountingFile();

};

#endif /* VOTECOUNTINGFILE_h */

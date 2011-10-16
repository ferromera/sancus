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
#include "CreationMode.h"

#define VOTE_COUNTING_ELECTION_INDEX_PATH "./VoteCountingElectionIndex.bin"
#define VOTE_COUNTING_DISTRICT_INDEX_PATH "./VoteCountingDistrictIndex.bin"
#define VOTE_COUNTING_LIST_INDEX_PATH "./VoteCountingListIndex.bin"
#define VOTE_COUNTING_PRIMARY_INDEX_PATH	"./VoteCountingPrimaryIndex.bin"
#define VOTE_COUNTING_DATA_PATH	"./VoteCountingData.bin"



class VoteCountingFile {
	enum lastSearchEnum {ELECTION_SEARCH,DISTRICT_SEARCH,LIST_SEARCH,PRIMARY_SEARCH,NO_SEARCH};

	BPlusTree<SecondaryIndexRecord<ElectionRecord::Key,VoteCountingRecord::Key>,4096> * electionIndex;
	BPlusTree<SecondaryIndexRecord<DistrictRecord::Key,VoteCountingRecord::Key>,4096> * districtIndex;
	BPlusTree<SecondaryIndexRecord<ListRecord::Key,VoteCountingRecord::Key>,4096> * listIndex;
	BPlusTree<PrimaryIndexRecord<VoteCountingRecord::Key>,4096> * primaryIndex;
	IndexedDataFile<VoteCountingRecord,8192> * dataFile;
	enum lastSearchEnum lastSearch;



	static VoteCountingFile * instance;
	static VoteCountingFile * getInstance();

public:
	VoteCountingFile(int creationMode=CREATE);

	void insert(const VoteCountingRecord &);
	void remove(const VoteCountingRecord &);
	void update(const VoteCountingRecord &);
	const VoteCountingRecord & searchByDistrict(const DistrictRecord::Key & district);
	const VoteCountingRecord & searchByElection(const ElectionRecord::Key & district);
	const VoteCountingRecord & searchByList(const ListRecord::Key & district);
	const VoteCountingRecord & search(const VoteCountingRecord::Key & district);
	const VoteCountingRecord & VoteCountingFile::nextDistrict();
	const VoteCountingRecord & VoteCountingFile::nextElection();
	const VoteCountingRecord & VoteCountingFile::nextList();
	const VoteCountingRecord & VoteCountingFile::next();

	virtual ~VoteCountingFile();

};

#endif /* VOTECOUNTINGFILE_h */

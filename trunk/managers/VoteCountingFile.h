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
#include "../estructuras/VoteCountingRecord.h"

#define PATH_ELECTION_INDEX_FILE ".\ElectionIndex"
#define PATH_DISTRICT_INDEX_FILE ".\DistrictIndex"
#define PATH_POLITICAL_LIST_INDEX_FILE ".\PoliticalListIndex"
#define PATH_PRIMARY_INDEX	".\VoteCountingPrimaryIndex"
#define PATH_VOTE_COUNTNG_DATA_FILE	".\VoteCountingData"

class VoteCountingFile {
public:
	VoteCountingFile();
	virtual ~VoteCountingFile();
	void insert(const VoteCountingRecord &);
	void remove(const VoteCountingRecord &);
	std::list<VoteCountingRecord> searchByElection(uint32_t);
	std::list<VoteCountingRecord> searchByPoliticalList(std::string);
	std::list<VoteCountingRecord> searchByDistrict(std::string);
	//ElectionRecord searchByPrimaryID(uint16_t);

private:
	//BPlusTree<ElectionIndexRecord,512> *primaryIndex;
	//BPlusTree<DateIndexRecord,512>     *dateIndex;
	//BPlusTree<ChargeIndexRecord,512>   *chargeIndex;
	//BPlusTree<DistrictIndexRecord,512> *districIndex;
};

#endif /* VOTECOUNTINGFILE_h */

/*
 * FIleManagerElection.h
 *
 *  Created on: 21/09/2011
 *      Author: juanmromera
 */

#ifndef FILEMANAGERELECTION_H_
#define FILEMANAGERELECTION_H_

#include <list>
#include <string>
#include "../estructuras/ElectionRecord.h"

#define PATH_ELECTION_INDEX_FILE ".\ElectionIndex"
#define PATH_DATE_INDEX_FILE ".\DateIndex"
#define PATH_CHARGE_INDEX_FILE ".\ChargeIndex"
#define PATH_DISTRICT_INDEX_FILE ".\DateIndex"

class FIleManagerElection {
public:
	FIleManagerElection();
	virtual ~FIleManagerElection();
	void insert(const ElectionRecord &);
	void remove(const ElectionRecord &);
	std::list<ElectionRecord> searchByDate(uint32_t);
	std::list<ElectionRecord> searchByCharge(std::string);
	std::list<ElectionRecord> searchByDistrict(std::string);
	ElectionRecord searchByPrimaryID(uint16_t);

private:
	//BPlusTree<ElectionIndexRecord,512> *primaryIndex;
	//BPlusTree<DateIndexRecord,512>     *dateIndex;
	//BPlusTree<ChargeIndexRecord,512>   *chargeIndex;
	//BPlusTree<DistrictIndexRecord,512> *districIndex;
};

#endif /* FILEMANAGERELECTION_H_ */

/*
 * AdministratorFile.h
 *
 *  Created on: 18/10/2011
 *      Author: cecilia
 */

#ifndef ADMINISTRATORFILE_H_
#define ADMINISTRATORFILE_H_
#include "../records/AdministratorRecord.h"
#include "../estructuras/HashTable.h"

#define ADMINISTRATOR_FILE_DATA_PATH	"AdministratorFileData.bin"
#define ADMINISTRATOR_FILE_RECORDS_PER_BUCKET 50
#define ADMINISTRATOR_FILE_MAX_NUMBER_OF_RECORDS	10000

class AdministratorFile {
private:
	HashTable<AdministratorRecord,4096> * table;
	static AdministratorFile * instance;
public:
	static AdministratorFile * getInstance();
	static void deleteInstance(){
		delete instance;
		instance=NULL;
	}
	AdministratorFile();

	void insert(const AdministratorRecord &);
	void remove(const AdministratorRecord &);
	void update(const AdministratorRecord &);
	void createReportFile();
	const AdministratorRecord & search(const AdministratorRecord::Key &);

	virtual ~AdministratorFile();
};


#endif /* ADMINISTRATORFILE_H_ */

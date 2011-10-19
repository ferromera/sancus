/*
 * AdmministratorFile.cpp
 *
 *  Created on: 18/10/2011
 *      Author: cecilia
 */

#include "AdministratorFile.h"
#include "FileManagerExceptions.h"

AdministratorFile* AdministratorFile::instance = NULL;

AdministratorFile * AdministratorFile::getInstance(){
	if(instance==NULL)
		instance= new AdministratorFile();
	return instance;
}

AdministratorFile::AdministratorFile() {
	try
	{
		this->table = new HashTable<AdministratorRecord, 4096> (ADMINISTRATOR_FILE_DATA_PATH);
	}catch(OpenFileException &e)
	{
		this->table = new HashTable<AdministratorRecord, 4096> (ADMINISTRATOR_FILE_DATA_PATH,
				ADMINISTRATOR_FILE_RECORDS_PER_BUCKET,
				ADMINISTRATOR_FILE_MAX_NUMBER_OF_RECORDS);
	}
}

void AdministratorFile::insert(const AdministratorRecord & Administrator) {
	try {
		table->insert(Administrator);
	} catch (RehashCountException & ex) {
		throw FileInsertException();
	}catch(UniqueViolationException & e)
	{
		throw FileInsertException();
	}

}

void AdministratorFile::remove(const AdministratorRecord & Administrator) {
	try {
		table->remove(Administrator);
	} catch (RecordNotFoundException & ex) {
		throw FileRemoveException();
	}
}

void AdministratorFile::update(const AdministratorRecord & Administrator) {
	try {
		table->update(Administrator);
	} catch (RecordNotFoundException & ex) {
		throw FileUpdateException();
	}
}

const AdministratorRecord & AdministratorFile::search(
		const AdministratorRecord::Key & AdministratorKey) {
	try {
		return table->get(AdministratorKey);
	} catch (RecordNotFoundException & ex) {
		throw FileSearchException();
	}
}

AdministratorFile::~AdministratorFile() {
	delete (table);
}

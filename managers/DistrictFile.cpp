/*
 * DistrictFile.cpp
 *
 *  Created on: Oct 16, 2011
 *      Author: alfredo
 */

#include "DistrictFile.h"
#include "FileManagerExceptions.h"

DistrictFile* DistrictFile::instance = NULL;

DistrictFile * DistrictFile::getInstance(){
	if(instance==NULL)
		instance= new DistrictFile();
	return instance;
}

DistrictFile::DistrictFile() {
	this->table = new HashTable<DistrictRecord, 4096> (DISTRICT_FILE_DATA_PATH,
			DISTRICT_FILE_RECORDS_PER_BUCKET,
			DISTRICT_FILE_MAX_NUMBER_OF_RECORDS);
}

void DistrictFile::insert(const DistrictRecord & district) {
	try {
		table->insert(district);
	} catch (RehashCountException & ex) {
		throw FileInsertException();
	}catch(UniqueViolationException & e)
	{
		throw FileInsertException();
	}

}

void DistrictFile::remove(const DistrictRecord & district) {
	try {
		table->remove(district);
	} catch (RecordNotFoundException & ex) {
		throw FileRemoveException();
	}
}

void DistrictFile::update(const DistrictRecord & district) {
	try {
		table->update(district);
	} catch (RecordNotFoundException & ex) {
		throw FileUpdateException();
	}
}

const DistrictRecord & DistrictFile::search(
		const DistrictRecord::Key & districtKey) {
	try {
		return table->get(districtKey);
	} catch (RecordNotFoundException & ex) {
		throw FileSearchException();
	}
}

DistrictFile::~DistrictFile() {
	delete (table);
}

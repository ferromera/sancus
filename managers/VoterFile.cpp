/*
 * VoterFile.cpp
 *
 *  Created on: 17/10/2011
 *      Author: cecilia
 */

#include "VoterFile.h"

VoterFile* VoterFile::instance = NULL;

VoterFile * VoterFile::getInstance(){
	if(instance == NULL)
		instance= new VoterFile();
	return instance;
}

VoterFile::VoterFile()
{
	try
	{
		this->table = new HashTable<VoterRecord, 4096> (VOTER_FILE_DATA_PATH);
	}catch(OpenFileException & e)
	{
		this-> table = new HashTable<VoterRecord, 4096> (VOTER_FILE_DATA_PATH,
					VOTER_FILE_RECORDS_PER_BUCKET, VOTER_FILE_MAX_NUMBER_OF_RECORDS);
	}
}


void VoterFile::insert(const VoterRecord & vr){
	try {
			table->insert(vr);
	} catch (RehashCountException & ex) {
			throw FileInsertException();
	}
	catch (UniqueViolationException &e)
	{
		throw FileInsertException();
	}

}

void VoterFile::remove(const VoterRecord & vr) {
	try {
		table->remove(vr);
	} catch (RehashCountException & ex){
		throw FileRemoveException();
	}
}

const VoterRecord & VoterFile::search(const VoterRecord::Key & vr) {
	try {
		return table->get(vr);
	} catch (RecordNotFoundException & ex) {
		throw FileSearchException();
	}
}

void VoterFile::update(const VoterRecord & vr) {
	try {
		table->update(vr);
	} catch (RecordNotFoundException & ex) {
		throw FileUpdateException();
	}
}

VoterFile::~VoterFile() {
	delete (table);
}

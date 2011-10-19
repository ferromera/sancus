/*
 * CandidateFile.cpp
 *
 *  Created on: 18/10/2011
 *      Author: cecilia
 */

#include "CandidateFile.h"
#include "FileManagerExceptions.h"

CandidateFile* CandidateFile::instance = NULL;

CandidateFile * CandidateFile::getInstance(){
	if(instance==NULL)
		instance= new CandidateFile();
	return instance;
}

CandidateFile::CandidateFile() {
	try
	{
	this->table = new HashTable<CandidateRecord, 4096> (CANDIDATE_FILE_DATA_PATH);
	}catch(OpenFileException &e)
	{
		this->table = new HashTable<CandidateRecord, 4096> (CANDIDATE_FILE_DATA_PATH,
				CANDIDATE_FILE_RECORDS_PER_BUCKET,
				CANDIDATE_FILE_MAX_NUMBER_OF_RECORDS);
	}
}

void CandidateFile::insert(const CandidateRecord & candidate) {
	try {
		table->insert(candidate);
	} catch (RehashCountException & ex) {
		throw FileInsertException();
	}catch(UniqueViolationException & e)
	{
		throw FileInsertException();
	}

}

void CandidateFile::remove(const CandidateRecord & candidate) {
	try {
		table->remove(candidate);
	} catch (RecordNotFoundException & ex) {
		throw FileRemoveException();
	}
}

void CandidateFile::update(const CandidateRecord & candidate) {
	try {
		table->update(candidate);
	} catch (RecordNotFoundException & ex) {
		throw FileUpdateException();
	}
}

const CandidateRecord & CandidateFile::search(
		const CandidateRecord::Key & CandidateKey) {
	try {
		return table->get(CandidateKey);
	} catch (RecordNotFoundException & ex) {
		throw FileSearchException();
	}
}

CandidateFile::~CandidateFile() {
	delete (table);
}

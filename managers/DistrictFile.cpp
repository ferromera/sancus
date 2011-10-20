/*
 * DistrictFile.cpp
 *
 *  Created on: Oct 16, 2011
 *      Author: alfredo
 */

#include "DistrictFile.h"
#include "FileManagerExceptions.h"

#ifdef DISTRICT_FILE_BPLUS

DistrictFile* DistrictFile::instance = NULL;

DistrictFile * DistrictFile::getInstance() {
	if (instance == NULL)
		instance = new DistrictFile();
	return instance;
}

DistrictFile::DistrictFile() {
	tree = new BPlusTree<DistrictRecord, DISTRICT_FILE_BLOCKSIZE> (DISTRICT_FILE_DATA_PATH);
}

void DistrictFile::insert(const DistrictRecord & district) {
	try {
		tree->insert(district);
	} catch (LeafUnicityException & ex) {
		throw FileInsertException();
	} catch (BPTreeRecordSizeException & e) {
		throw FileInsertException();
	}

}

void DistrictFile::remove(const DistrictRecord & district) {
	try {
		tree->remove(district);
	} catch (LeafRecordNotFoundException & ex) {
		throw FileRemoveException();
	}
}

void DistrictFile::update(const DistrictRecord & district) {
	try {
		tree->update(district);
	} catch (LeafRecordNotFoundException & ex) {
		throw FileUpdateException();
	}
}

// Retorna el registro con la clave districtKey o, si no se encuentra,
// el mayor inmediato.
const DistrictRecord & DistrictFile::search(const DistrictRecord::Key & districtKey) {
	try {
		return tree->search(districtKey);
	} catch (ThereIsNoNextLeafException<DistrictRecord> & ex) {
		throw FileSearchException();
	}
}
// Debe ser llamado despues de un search o next, retorna el mayor inmediato al ultimo
// retornado por search o next.
const DistrictRecord & DistrictFile::next() {
	try {
		return tree->next();
	} catch (ThereIsNoNextLeafException<DistrictRecord> & ex) {
		throw FileNextException();
	}catch(BPlusTreeNextException & e){
		throw FileNextException();
	}
}
void DistrictFile::report(){
	tree->preOrderReport();
}
DistrictFile::~DistrictFile() {
	delete tree;
}

#else
#include "../estructuras/IndexedDataFileExceptions.h"

DistrictFile* DistrictFile::instance = NULL;

DistrictFile * DistrictFile::getInstance() {
	if(instance==NULL)
	instance= new DistrictFile();
	return instance;
}

DistrictFile::DistrictFile() {
	//TODO: AGREGAR VALIDACION DE SI EXISTE EL FILE O NO.
	try
	{
		this->table = new HashTable<DistrictRecord, 4096> (DISTRICT_FILE_DATA_PATH);
	} catch(OpenFileException e)
	{
		this->table = new HashTable<DistrictRecord, 4096> (DISTRICT_FILE_DATA_PATH,
				DISTRICT_FILE_RECORDS_PER_BUCKET,
				DISTRICT_FILE_MAX_NUMBER_OF_RECORDS);
	}
}

void DistrictFile::insert(const DistrictRecord & district) {
	try {
		table->insert(district);
	} catch (RehashCountException & ex) {
		throw FileInsertException();
	} catch(UniqueViolationException & e)
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

#endif

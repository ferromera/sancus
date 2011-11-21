/*
 * DistrictFile.cpp
 *
 *  Created on: Oct 16, 2011
 *      Author: alfredo
 */

#include "DistrictFile.h"
#include "../utils/Time.h"
#include "FileManagerExceptions.h"

#ifdef DISTRICT_FILE_BPLUS

DistrictFile* DistrictFile::instance = NULL;

DistrictFile * DistrictFile::getInstance() {
	if (instance == NULL
		)
		instance = new DistrictFile();
	return instance;
}

DistrictFile::DistrictFile() {
	tree = new BPlusTree<DistrictRecord, DISTRICT_FILE_BLOCKSIZE>(
			DISTRICT_FILE_DATA_PATH);
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
const DistrictRecord & DistrictFile::search(
		const DistrictRecord::Key & districtKey) {
	try {
		return tree->search(districtKey);
	} catch (ThereIsNoNextLeafException & ex) {
		throw FileSearchException();
	}
}
// Debe ser llamado despues de un search o next, retorna el mayor inmediato al ultimo
// retornado por search o next.
const DistrictRecord & DistrictFile::next() {
	try {
		return tree->next();
	} catch (ThereIsNoNextLeafException & ex) {
		throw FileNextException();
	} catch (BPlusTreeNextException & e) {
		throw FileNextException();
	}
}
void DistrictFile::report() {
	tree->preOrderReport();
}
void DistrictFile::createReportFile() {
	std::string nameFile = "reportFileDistrict";
	nameFile.append(Time::getTime());

	File reportFile = File(nameFile, File::NEW);
	DistrictRecord::Key key = DistrictRecord::Key();
	this->search(key);
	while (true) {
		try {
			DistrictRecord record = this->next();
			reportFile << "Nombre de Distrito: " << record.getDistrictName()
					<< " Distrito Padre: " << record.getFatherName() << "\n";
		} catch (FileNextException e) {
			break;
		}
	}
	std::cout << "Se ha creado el archivo " << nameFile << " con Exito" << std::endl;

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

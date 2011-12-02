/*
 * AdmministratorFile.cpp
 *
 *  Created on: 18/10/2011
 *      Author: cecilia
 */

#include "AdministratorFile.h"
#include "FileManagerExceptions.h"
#include "../utils/Time.h"
#include "../estructuras/RsaSecurity.h"

AdministratorFile* AdministratorFile::instance = NULL;

AdministratorFile * AdministratorFile::getInstance(){
	if(instance==NULL)
		instance= new AdministratorFile();
	return instance;
}

AdministratorFile::AdministratorFile() {
	SecurityStrategy *security = new RsaSecurity(10);
	try
	{
		this->table = new HashTable<AdministratorRecord, 4096> (ADMINISTRATOR_FILE_DATA_PATH,security);
	}catch(OpenFileException &e)
	{
		this->table = new HashTable<AdministratorRecord, 4096> (ADMINISTRATOR_FILE_DATA_PATH,
				ADMINISTRATOR_FILE_RECORDS_PER_BUCKET,
				ADMINISTRATOR_FILE_MAX_NUMBER_OF_RECORDS,security);
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
void AdministratorFile::createReportFile()
{
	std::string nameFile = "reportFileAdministrator";
	nameFile.append(Time::getTime());
	File reportFile = File(nameFile, File::NEW);
	this->table->positionateAtBegining();
	while (true) {
		try {
			AdministratorRecord record = this->table->next();
			reportFile << "Usuario : " << record.getUser()
					<< " Contraseña: " << record.getPassword() << "\n";
		} catch (RecordNotFoundException e) {
			break;
		}
	}
	std::cout << "Se ha creado el archivo " << nameFile << " con Exito" << std::endl;

}

AdministratorFile::~AdministratorFile() {
	delete (table);
}

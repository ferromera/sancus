/*
 * VoterFile.cpp
 *
 *  Created on: 17/10/2011
 *      Author: cecilia
 */

#include "VoterFile.h"
#include "../utils/Time.h"
#include "../estructuras/RsaSecurity.h"

VoterFile* VoterFile::instance = NULL;

VoterFile * VoterFile::getInstance(){
	if(instance == NULL)
		instance= new VoterFile();
	return instance;
}

VoterFile::VoterFile()
{	SecurityStrategy *security = new DummySecurity(); //new RsaSecurity(10);
	try
	{
		this->table = new HashTable<VoterRecord, 4096> (VOTER_FILE_DATA_PATH,security);
	}catch(OpenFileException & e)
	{
		this-> table = new HashTable<VoterRecord, 4096> (VOTER_FILE_DATA_PATH,
					VOTER_FILE_RECORDS_PER_BUCKET, VOTER_FILE_MAX_NUMBER_OF_RECORDS,security);
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
void VoterFile::createReportFile()
{
	std::string nameFile = "reportFileVoter";
	nameFile.append(Time::getTime());
	File reportFile = File(nameFile, File::NEW);
	this->table->positionateAtBegining();
	while (true) {
		try {
			VoterRecord record = this->table->next();
			reportFile << "DNI : " << record.getDni()
					<< " Nombre: " << record.getName()
					<< " Distrito: " << record.getDistrict().getKey()
					<< " Direccion: " << record.getAddress()
					<< " Contraseña " << record.getUserKey()
					<< " Elecciones hechas: ";

			std::list<ElectionRecord::Key> list = record.getElectionList();
			std::list<ElectionRecord::Key>::iterator it = list.begin();
			for(;it != list.end();it++)
			{
				reportFile << it->getString() << " - ";
			}
			reportFile<< "\n";
		} catch (RecordNotFoundException e) {
			break;
		}
	}
	std::cout << "Se ha creado el archivo " << nameFile << " con Exito" << std::endl;
}

VoterFile::~VoterFile() {
	delete (table);
}

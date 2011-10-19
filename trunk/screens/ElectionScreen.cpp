/*
 * ElectionScreen.cpp
 *
 *  Created on: 14/10/2011
 *      Author: juanmromera
 */

#include "ElectionScreen.h"
#include "../core/VoteApp.h"
//#include "../records/VoterRecord.h"
//#include "../records/ElectionRecord.h"

ElectionScreen::ElectionScreen() {

}
void ElectionScreen::draw()
{
	int electionToVote;
	system("clear");
	std::cout<<"PANTALLA DE ELECCIONES PENDIENTES"<<std::endl;
	VoteApp* app = VoteApp::getInstance();
/*
	//VoterRecord user = app->getUserLogin();

	std::list<DistrictRecord::Key> keysOfDistrict;
	std::list<ElectionRecord> keysOfElection;
	DistrictRecord distRecord;
	DistrictFile* dFile = DistrictFile::getInstance();
	ElectionFile* electionFile = ElectionFile::getInstance();
	try
	{
		distRecord = dFile->search(user->getDistrict());
	}
	catch(FileSearchException &e)
	{
	}
	while(distRecord.hasFather())
	{
		keysOfDistrict.push_back(distRecord.getKey());
			try
			{
				distRecord = dFile->search(distRecord->getFather());
			}
			catch(FileSearchException &e)
			{
			}
	}
	keysOfDistrict.push_back(distRecord.getKey());

	std::list<DistrictRecord::Key>::iterator itDistrict = keysOfDistrict.begin();
	while(itDistrict != keysOfElection.end())
	{
		ElectionRecord record;
			try
			{
				record = electionFile->searchByDistrict(*itDistrict);
			}
			catch(FileSearchException &e)
			{
				break;
			}
			if(isAValidElection(record)
				keysOfElection.push_back(record);
			while(true)
			{
				try
				{
					record = electionFile->nextByDistrict();
				}
				catch(FileNextException &e)
				{
					break;
				}
				if(isAValidElection(record)
					keysOfElection.push_back(record);
			}
			itDistrict++;
	}

	std::list<ElectionRecord>::iterator itElect = keysOfElection.begin();

	for(int i = 0 ; i < keysOfElection.size();i++,itElect++)
	{
		ElectionRecord record = *itElect;
		std::cout<<number<<" - Cargo: "<<record.getCharge()<< " Fecha: " << record.getDate() <<std::endl;
	}
	std::cout<< "Eliga una eleccion a votar y presione ENTER"<<std::endl;
	while(true)
	{
		electionToVote = IstreamUtils::getUint();
		if(electionToVote <= keysOfElection.size)
		{
			std::list<ElectionRecord>::iterator itElection = getElectionNumber(electionToVote - 1);
			app->setChooseElection(*iterator);
			app->setActualScreen(LIST_SCREEN);
			return;
		}
		else
		{
			std::cout<<"Numero de eleccion Incorrecto eliga nuevamente"<<std::endl;
		}
	}
	*/
}
/*
std::list<ElectionRecord>::iterator ElectionScreen::getElectionNumber(unsigned int electionNumber)
{
	std::list<ElectionRecord>::iterator itList = elections.begin();
	int a = elections.size();
	int i = 0;
	while(i < electionNumber)
	{
		itList++;
		i++;
	}


	return itList;
}

bool ElectionScreen::isAValidElection(ElectionRecord election)
{
	VoteApp* app = VoteApp::getInstance();
	VoterRecord user = app->getUserLogin();
	std::list<ElectionRecord::Key> elList = user.getElections();
	std::list<ElectionRecord::Key>::iterator itList =  elList.begin();
	while(itList != elList.end())
	{
		if(*itList == election.getKey())
			return false;

		itList++;
	}
	return true;
}
*/

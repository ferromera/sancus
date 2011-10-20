/*
 * ElectionScreen.cpp
 *
 *  Created on: 14/10/2011
 *      Author: juanmromera
 */

#include "ElectionScreen.h"
#include "../core/VoteApp.h"

ElectionScreen::ElectionScreen() {

}
void ElectionScreen::draw()
{
	int electionToVote;
	system("clear");
	VoteApp* app = VoteApp::getInstance();
	VoterRecord user = app->getUserLogin();
	std::cout<<"Bienvenido "<< user.getName() <<" al sistema de voto electronico."<<std::endl;
	std::cout<<"Ciudad " <<user.getDistrict().getString()<< " Domicilio" <<user.getAddress()<<std::endl;
	std::cout<<"Abajo observara la elecciones en las cual debe votar"<<std::endl;

	std::list<DistrictRecord::Key> keysOfDistrict;
	std::list<ElectionRecord> keysOfElection;
	DistrictRecord distRecord;
	DistrictFile* dFile = DistrictFile::getInstance();
	DistrictRecord::Key dKey = DistrictRecord::Key(user.getDistrict().getString());
	ElectionFile* electionFile = ElectionFile::getInstance();
	try
	{
		distRecord = dFile->search(dKey);
		//std::cout<<"Distrito "<< distRecord.getKey().getKey()<<std::endl;
	}
	catch(FileSearchException &e)
	{
		std::cout<<"Distrito "<<std::endl;
	}
	while(distRecord.hasFather())
	{
		keysOfDistrict.push_back(distRecord.getKey());
			try
			{
				distRecord = dFile->search(distRecord.getFather());
			}
			catch(FileSearchException &e)
			{
			}
	}
	//keysOfDistrict.push_back(distRecord.getKey());
	//std::cout<<"Distrito "<< distRecord.getDistrictName()<<std::endl;

	std::list<DistrictRecord::Key>::iterator itDistrict = keysOfDistrict.begin();
	while(itDistrict != keysOfDistrict.end())
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
			if(isAValidElection(record))
				keysOfElection.push_back(record);
			while(true)
			{
				try
				{
					record = electionFile->nextDistrict();
				}
				catch(FileNextException &e)
				{
					break;
				}
				if(isAValidElection(record))
					keysOfElection.push_back(record);
			}
			itDistrict++;
	}

	std::list<ElectionRecord>::iterator itElect = keysOfElection.begin();

	for(unsigned int i = 0 ; i < keysOfElection.size();i++,itElect++)
	{
		int number = i +1;
		ElectionRecord record = *itElect;
		std::cout<< number<<" - Cargo: "<<record.getCharge().getCharge() << " Fecha: " << record.getDate() <<std::endl;
	}
	std::cout<< "Eliga una eleccion a votar y presione ENTER"<<std::endl;
	while(true)
	{
		electionToVote = IstreamUtils::getUint();
		if(electionToVote <= keysOfElection.size())
		{
			std::list<ElectionRecord>::iterator itElection = getElectionNumber(electionToVote - 1);
			app->setChooseElection(*itElection);
			app->setActualScreen(LIST_SCREEN);
			return;
		}
		else
		{
			std::cout<<"Numero de eleccion Incorrecto eliga nuevamente"<<std::endl;
		}
	}
}

std::list<ElectionRecord>::iterator ElectionScreen::getElectionNumber(unsigned int electionNumber)
{
	std::list<ElectionRecord>::iterator itList = elections.begin();
	unsigned int i = 0;
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
	std::list<ElectionRecord::Key> elList = user.getElectionList();
	std::list<ElectionRecord::Key>::iterator itList =  elList.begin();
	while(itList != elList.end())
	{
		if(*itList == election.getKey())
			return false;

		itList++;
	}
	return true;
}


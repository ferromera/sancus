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
	unsigned int electionToVote;
	system("clear");
	VoteApp* app = VoteApp::getInstance();
	VoterRecord user = app->getUserLogin();
	std::cout<<"Bienvenido "<< user.getName() <<" al sistema de voto electronico."<<std::endl;
	std::cout<<"Ciudad: " <<user.getDistrict().getString()<< ", Domicilio: " <<user.getAddress()<<std::endl;
	std::cout<<"Abajo observara la elecciones en las cual debe votar"<<std::endl;
	std::cout<<""<<std::endl;
	std::list<DistrictRecord::Key> keysOfDistrict;
	DistrictRecord distRecord;
	DistrictFile* dFile = DistrictFile::getInstance();
	DistrictRecord::Key dKey = DistrictRecord::Key(user.getDistrict().getString());
	ElectionFile* electionFile = ElectionFile::getInstance();

	try
	{
		distRecord = dFile->search(dKey);
		if(distRecord.getKey()!=dKey)
			throw FileSearchException();

	}
	catch(FileSearchException &e)
	{
		std::cout<<"Error! : Distrito no encontrado"<<std::endl;
	}
	while(distRecord.hasFather())
	{
		keysOfDistrict.push_back(distRecord.getKey());
			try
			{
				DistrictRecord::Key fatherKey=distRecord.getFather();
				distRecord = dFile->search(fatherKey);
				if(distRecord.getKey()!=fatherKey)
					throw FileSearchException();
			}
			catch(FileSearchException &e)
			{
				std::cout<<"Error! : Distrito padre no encontrado"<<std::endl;
			}
	}
	keysOfDistrict.push_back(distRecord.getKey());
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
				itDistrict++;
				continue;
			}
			if(isAValidElection(record))
				elections.push_back(record);
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
					elections.push_back(record);
			}
			itDistrict++;
	}
	std::list<ElectionRecord>::iterator itElect = elections.begin();

	for(unsigned int i = 0 ; i < elections.size();i++,itElect++)
	{
		int number = i +1;
		ElectionRecord record = *itElect;
		std::cout<< number<<" - Cargo: "<<record.getCharge().getCharge() << " Fecha: " << record.getDate() << " Distrito " << record.getDistrict().getKey() << std::endl;
	}
	std::cout<<""<<std::endl;
	std::cout<<"Elija una eleccion a votar y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	while(true)
	{
		electionToVote = IstreamUtils::getUint();
		if(electionToVote <= elections.size())
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


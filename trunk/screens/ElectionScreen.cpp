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

	std::list<ElectionRecord::Key> keysOfElection = user.getElectionList();
	std::list<ElectionRecord::Key>::iterator itElect = keysOfElection.begin();
	ElectionFile* electionFile = ElectionFile::getInstance();

	for(int i = 0 ; i < keysOfElection.size();i++,itElect++)
	{
		int number = i+1;
		ElectionRecord record = ElectionRecord((*itElect));
		try
		{
			record = electionFile->search(record);
		}
		catch(FileSearchException e)
		{
			break;
		}
		elections.push_back(record);
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
*/

/*
 * ListScreen.cpp
 *
 *  Created on: 14/10/2011
 *      Author: juanmromera
 */

#include "ListScreen.h"
#include "../core/VoteApp.h"

ListScreen::ListScreen() {
	// TODO Auto-generated constructor stub

}
void  ListScreen::draw(){
	VoteApp* app = VoteApp::getInstance();
	/*
	ListFile* listFile = ListFile::getInstance();
	VoteCountingFile* voteFile = VoteCountingFile::getInstance();
	ElectionRecord electionRecord = app->getChooseElection();
	VoterRecord voterRecord = app->getUserLogin();
	char opcion;
	system("clear");
	std::cout<<"/////////////////////////////////////////"<<std::endl;
	std::cout<<"/////////////////////////////////////////"<<std::endl;
	std::cout<<"/////////////////////////////////////////"<<std::endl;
	std::cout<<"             ELIGA SU LISTA			          "<<std::endl;
	std::cout<<"/////////////////////////////////////////"<<std::endl;
	std::cout<<"/////////////////////////////////////////"<<std::endl;
	std::cout<<"/////////////////////////////////////////"<<std::endl;

	try
	{
		ListRecord listRecord = listFile->searchByElection(electionRecord->getKey());
	}catch(FileSearchException)
	{

	}
	while(true)
	{
		showList(listRecord);
		bool opcionInvalida = true;
		while(opcionInvalida)
		{
			std::cout<<" Presione V para votar o S para seguir, luego apriete ENTER " << listRec.getName() << std::endl;
			std::cin>>opcion;
			if(opcion == 'V')
			{
				VoteConuntingRecord::Key voteKey = VoteConuntingRecord::Key(listRecord.getKey(),voterRecord.getVoterDistrict(),electionRecord.getKey());
				voteFile->addVote(voteKey);

				std::list<ElectionRecord::Key> keysOfElection = voterRecord.getElections();
				std::list<ElectionRecord::Key>::iterator itElect = keysOfElection.begin();
				std::list<ElectionRecord::Key> newListElections;

				for(int i = 0; i < keysOfElection.size();i++,itElect++)
				{
					if((*itElect)!= electionRecord.getKey())
						newListElections.push_back(*itElect);
				}

				voterRecord.setElections(newListElections);
				app->setUserLogin(voterRecord);
				app->setActualScreen(ELECTION_SCREEN);
				return;
			}
			else if (opcion == 'S')
			{
				listRecord = listFile->next();
				if(listRecord.getElection() != electionRecord.getKey())
				{
					try
					{
						ListRecord listRecord = listFile->searchByElection(electionRecord->getKey());
					}catch(FileSearchException)
					{

					}
				}
				opcionInvalida = false;
			}
			else
			{
				std::cout<<"OPCION INVALIDA " << std::endl;
			}
		}
	}
	*/

}
/*
void ListRecord::showList(ListRecord listRec)
{
	std::cout<<" LISTA " << listRec.getName() << std::endl;
}*/


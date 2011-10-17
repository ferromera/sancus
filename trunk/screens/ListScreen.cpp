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
	std::cout<<"PANTALLA DE LISTAS"<<std::endl;
	try
	{
		ListRecord listRecord = listFile->searchByElection(electionRecord->getKey());
	}catch(FileSearchException)
	{
		std::cout<<"Error no existe la lista en el archivo de Listas"<<std::endl;
	}
	while(true)
	{
		showList(listRecord);
		bool opcionInvalida = true;
		while(opcionInvalida)
		{
			std::cout<<" Presione V para votar o S para seguir, luego apriete ENTER " << listRec.getName() << std::endl;
			opcion = IstreamUtils::getChar();
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
				try
				{
					listRecord = listFile->nextByElection();
				}catch(FileNextException e)
				{
					try
					{
						listRecord = listFile->searchByElection(electionRecord->getKey());
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
	}*/
}
/*
void ListRecord::showList(ListRecord listRec)
{
	std::cout<<" LISTA " << listRec.getName() << std::endl;
	std::cout<<""<<std::endl;
	ElectionRecord electionRecord = app->getChooseElection();
	//Busco cargos
	ChargeFile* cFile = ChargeFile::getInstance();
	CandidateFile* candFile = CandidateFile::getInstance();
	std::list<ChargeRecord::Key> listaCargos;
	listaCargos.push_back(electionRecord.getCharge());
	ChargeRecord chRecord;
	while(true)
	{
		try
		{
			chRecord = cFile->searchByFatherCharge(electionRecord.getCharge());
		}catch(FileSearchEception e)
		{
			break;
		}
		listaCargos.push_back(chRecord.getCharge());
	}
	std::list<ChargeRecord::Key>::iterator itListaCargos = listaCargos.begin();

	for(int i = 0 ; i<listaCargos.size(); i++,itListaCargos++)
	{
		CandidateRecord::Key candKey = CandidateRecord(listRec.getKey(),(*itListaCargos));
		CandidateRecord candRecord;
		try
		{
			candRecord = candFile->search(candKey);
		}catch(FileSearchEception e)
		{
			continue;
		}
		std::cout<<"Cargo: "<< candRecord.getCharge() << " Candidato: " candRecord.getVoter().getName() << " " << candRecord.getVoter().getLastName()<<std::endl;
	}


}
*/

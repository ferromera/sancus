/*
 * ListScreen.cpp
 *
 *  Created on: 14/10/2011
 *      Author: juanmromera
 */

#include "ListScreen.h"

ListScreen::ListScreen() {
	// TODO Auto-generated constructor stub

}
void ListScreen::draw() {
	VoteApp* app = VoteApp::getInstance();

	ListFile* listFile = ListFile::getInstance();
	VoteCountingFile* voteFile = VoteCountingFile::getInstance();
	ElectionRecord electionRecord = app->getChooseElection();
	VoterRecord voterRecord = app->getUserLogin();
	ListRecord listRecord;
	char opcion;
	system("clear");
	std::cout << "PANTALLA DE LISTAS" << std::endl;
	std::cout <<  "" << std::endl;
	try {
		listRecord = listFile->searchByElection(electionRecord.getKey());
	} catch (FileSearchException) {
		std::cout << "Error no existe la lista en el archivo de Listas" << std::endl;
	}
	while (true) {
		showList(listRecord);
		bool opcionInvalida = true;
		while (opcionInvalida) {
			std::cout << "Presione V para votar o S para seguir, luego apriete ENTER " << std::endl;
			std::cout << "" << std::endl;
			opcion = IstreamUtils::getChar();
			if (opcion == 'V') {
				std::list<DistrictRecord::Key> districtList = getDistrictListOf(voterRecord.getDistrict());
				VoteCountingRecord::Key voteKey;

				std::list<DistrictRecord::Key>::iterator itDistrict = districtList.begin();
				for (; (*itDistrict) != electionRecord.getDistrict(); itDistrict++) {
					voteKey = VoteCountingRecord::Key(listRecord.getKey(), *itDistrict, electionRecord.getKey());
					VoteCountingRecord voteCounting = voteFile->search(voteKey);
					voteCounting.setCount(voteCounting.getCount() + 1);
					voteFile->update(voteCounting);
				}
				voteKey = VoteCountingRecord::Key(listRecord.getKey(), *itDistrict, electionRecord.getKey());
				VoteCountingRecord voteCounting = voteFile->search(voteKey);
				voteCounting.setCount(voteCounting.getCount() + 1);
				voteFile->update(voteCounting);

				voterRecord.addElection(electionRecord.getKey());
				VoterFile * voterFile = VoterFile::getInstance();
				voterFile->update(voterRecord);
				app->setUserLogin(voterRecord);
				app->setActualScreen(ELECTION_SCREEN);
				return;
			} else if (opcion == 'S') {
				try {
					listRecord = listFile->nextElection();
				} catch (FileNextException e) {
					try {
						listRecord = listFile->searchByElection(electionRecord.getKey());
					} catch (FileSearchException) {
						std::cout << "Error no existe la lista en el archivo de Listas" << std::endl;
					}
				}
				opcionInvalida = false;
			} else {
				std::cout << "OPCION INVALIDA " << std::endl;
			}
		}
	}
}
std::list<DistrictRecord::Key> ListScreen::getDistrictListOf(const DistrictRecord::Key & district) {
	std::list<DistrictRecord::Key> returnedList;
	returnedList.push_back(district);
	DistrictRecord disRec("");
	DistrictFile * disFile = DistrictFile::getInstance();
	try {
		disRec = disFile->search(district);
		if (disRec.getKey() != district)
			throw FileSearchException();
	} catch (FileSearchException) {
		std::cout << "Error no se encontro el distrito" << std::endl;
	}
	while (disRec.hasFather()) {
		try {
			DistrictRecord::Key fatherKey = disRec.getFather();
			disRec = disFile->search(fatherKey);
			if (disRec.getKey() != fatherKey)
				throw FileSearchException();
			returnedList.push_back(fatherKey);
		} catch (FileSearchException) {
			std::cout << "Error no se encontro el distrito" << std::endl;
		}
	}
	return returnedList;
}

void ListScreen::showList(ListRecord listRec) {
	system("clear");
	std::cout << "PANTALLA DE LISTAS" << std::endl;
	std::cout <<  "" << std::endl;
	std::cout << "Lista " << listRec.getName() << std::endl;
	std::cout << "" << std::endl;
	VoteApp * app = VoteApp::getInstance();
	ElectionRecord electionRecord = app->getChooseElection();
	//Busco cargos
	ChargeFile* cFile = ChargeFile::getInstance();
	CandidateFile* candFile = CandidateFile::getInstance();
	std::list<ChargeRecord::Key> listaCargos;
	listaCargos.push_back(electionRecord.getCharge());
	ChargeRecord chRecord;

	try {
		ChargeRecord::Key fatherKey = electionRecord.getCharge();
		chRecord = cFile->searchByFather(fatherKey);
		if (chRecord.getKey() != fatherKey)
			throw FileSearchException();
		listaCargos.push_back(chRecord.getKey());
		while (true) {
			try {
				chRecord = cFile->nextFather();
				listaCargos.push_back(chRecord.getKey());
			} catch (FileNextException) {
				break;
			}
		}

	} catch (FileSearchException e) {

	}
	std::list<ChargeRecord::Key>::iterator itListaCargos = listaCargos.begin();

	for (unsigned int i = 0; i < listaCargos.size(); i++, itListaCargos++) {
		CandidateRecord::Key candKey = CandidateRecord::Key(listRec.getKey(), (*itListaCargos));
		CandidateRecord candRecord(candKey);
		try {
			candRecord = candFile->search(candKey);
			if (candRecord.getKey() != candKey)
				throw FileSearchException();
		} catch (FileSearchException e) {
			continue;
		}
		VoterFile * voterFile = VoterFile::getInstance();
		VoterRecord candVoter = voterFile->search(candRecord.getVoter());

		std::cout << "Cargo: " << candRecord.getCharge().getString() << " Candidato: " << candVoter.getName()
				<< std::endl;
	}
	std::cout << "" << std::endl;

}

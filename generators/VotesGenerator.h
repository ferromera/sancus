/*
 * VotesGenerator.h
 *
 *  Created on: Oct 19, 2011
 *      Author: alfredo
 */

#ifndef VOTESGENERATOR_H_
#define VOTESGENERATOR_H_

#define DOCUMENTO_BASE  20000000

#include <iostream>
#include <list>
#include "../managers/VoterFile.h"
#include "../managers/DistrictFile.h"
#include "../managers/ElectionFile.h"
#include "../managers/ListFile.h"
#include "../estructuras/MathUtils.h"
#include "../managers/VoteCountingFile.h"

using namespace std;

class VotesGenerator {
private:
	VoterFile * voters;
	DistrictFile * districts;
	ElectionFile * elections;
	ListFile * lists;
	VoteCountingFile * voteCountings;
public:
	VotesGenerator() {
		voters = VoterFile::getInstance();
		districts = DistrictFile::getInstance();
		elections = ElectionFile::getInstance();
		lists = ListFile::getInstance();
		voteCountings = VoteCountingFile::getInstance();
	}

	void vote(unsigned int documento) {
		VoterRecord::Key * key = new VoterRecord::Key(documento);
		VoterRecord voter = voters->search(*key);

		//cout << voter.getName() << endl;

		DistrictRecord::Key localidad = voter.getDistrict();
		DistrictRecord::Key provincia = districts->search(localidad).getFather();
		DistrictRecord::Key pais = districts->search(provincia).getFather();

		DistrictRecord::Key distritosMunicipales[] = { localidad };
		DistrictRecord::Key distritosProvinciales[] = { provincia, localidad };
		DistrictRecord::Key distritosNacionales[] = { pais, provincia, localidad };

		ElectionRecord eleccionMunicipal = elections->searchByDistrict(localidad);
		ElectionRecord eleccionProvincial = elections->searchByDistrict(provincia);
		ElectionRecord eleccionNacional = elections->searchByDistrict(pais);

		pick(voter, eleccionMunicipal.getKey(), distritosMunicipales,1);
		pick(voter, eleccionProvincial.getKey(),distritosProvinciales, 2);
		pick(voter, eleccionMunicipal.getKey(),distritosNacionales, 3);

	}
private:
	void pick(VoterRecord voter, ElectionRecord::Key eleccion, DistrictRecord::Key distritos[], unsigned int numberOfDistricts) {
		std::list<ListRecord> listas;

		listas.push_back(lists->searchByElection(eleccion));

		while (true) {
			try {
				listas.push_back(lists->nextElection());
			} catch (FileNextException) {
				break;
			}
		}

		std::list<ListRecord>::iterator it;

		unsigned int choice = MathUtils::randomNumber(1, listas.size() - 1);

		cout<<"CHOICE " << choice<<endl;

		unsigned int i = 0;
		for (it = listas.begin(); i < choice; it++, i++) {
		}

		ListRecord winner = *it;

		for (unsigned int i = 0; i < numberOfDistricts; i++) {
			updateVoteCountingRecordFor(winner.getKey(), distritos[i], eleccion);
		}

		cout << "Votante " << voter.getName() << " voto a la lista: " << winner.getKey().getString() << endl;
	}

	void updateVoteCountingRecordFor(ListRecord::Key winner, DistrictRecord::Key district, ElectionRecord::Key election) {
		VoteCountingRecord winnerRecord(winner, district, election, 0);
		winnerRecord = voteCountings->search(winnerRecord.getKey());
		winnerRecord.setCount(winnerRecord.getCount() + 1);
		voteCountings->update(winnerRecord);
	}
};

#endif /* VOTESGENERATOR_H_ */

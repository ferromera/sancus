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
	VotesGenerator(){
		voters = VoterFile::getInstance();
		districts = DistrictFile::getInstance();
		elections = ElectionFile::getInstance();
		lists = ListFile::getInstance();
		voteCountings = VoteCountingFile::getInstance();
	}

	void vote(unsigned int documento) {
		VoterRecord::Key * key = new VoterRecord::Key(documento);
		VoterRecord voter = voters->search(*key);

		cout<<voter.getName()<<endl;

		DistrictRecord::Key localidad = voter.getDistrict();

		DistrictRecord::Key provincia = districts->search(localidad).getFather();

		DistrictRecord::Key pais = districts->search(provincia).getFather();

		ElectionRecord eleccionMunicipal = elections->searchByDistrict(localidad);
		ElectionRecord eleccionProvincial = elections->searchByDistrict(provincia);
		ElectionRecord eleccionNacional = elections->searchByDistrict(pais);

		std::list<ListRecord> listas;

		listas.push_back(lists->searchByElection(eleccionMunicipal.getKey()));

		while(true){
			try{
				listas.push_back(lists->nextElection());
			}catch(FileNextException){
				break;
			}
		}

		std::list<ListRecord>::iterator it;

		for(it=listas.begin(); it!=listas.end(); it++){
			cout<<it->getKey().getString()<<endl;
		}

		unsigned int choice = MathUtils::randomNumber(0,listas.size()-1);

		unsigned int i = 0;
		for(it=listas.begin();i<choice;it++, i++){}

		ListRecord winner = *it;

		VoteCountingRecord winnerRecord(winner.getKey(),localidad,eleccionMunicipal.getKey(),0);

		winnerRecord = voteCountings->search(winnerRecord.getKey());

		winnerRecord.setCount(winnerRecord.getCount()+1);

		voteCountings->update(winnerRecord);

		cout<<"Votante "<<voter.getName()<< " voto a la lista: "<<winner.getKey().getString()<<endl;

		lists->searchByElection(eleccionProvincial.getKey());
		lists->searchByElection(eleccionNacional.getKey());

	}
};

#endif /* VOTESGENERATOR_H_ */

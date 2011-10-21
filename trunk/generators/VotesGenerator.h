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
#include "../managers/VoterFile.h"
#include "../managers/DistrictFile.h"
#include "../managers/ElectionFile.h"
#include "../managers/ListFile.h"

using namespace std;

class VotesGenerator {
private:
	VoterFile * voters;
	DistrictFile * districts;
	ElectionFile * elections;
	ListFile * lists;
public:
	VotesGenerator(){
		voters = VoterFile::getInstance();
		districts = DistrictFile::getInstance();
		elections = ElectionFile::getInstance();
		lists = ListFile::getInstance();
	}

	void vote(unsigned int documento) {
		VoterRecord::Key * key = new VoterRecord::Key(documento);
		VoterRecord voter = voters->search(*key);

		cout<<voter.getName()<<endl;

		DistrictRecord::Key localidad = voter.getDistrict();
		DistrictRecord::Key provincia;
		DistrictRecord::Key pais;

		DistrictRecord loc = districts->search(localidad);

		cout<<"localidad: "<<loc.getDistrictName()<<endl;



		ElectionRecord elecccionMunicipal = elections->searchByDistrict(localidad);
		ElectionRecord elecccionProvincial = elections->searchByDistrict(provincia);
		ElectionRecord elecccionNacional = elections->searchByDistrict(pais);

		lists->searchByElection(elecccionMunicipal.getKey());
		lists->searchByElection(elecccionProvincial.getKey());
		lists->searchByElection(elecccionNacional.getKey());

	}
};

#endif /* VOTESGENERATOR_H_ */

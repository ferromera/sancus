/*
 * VotesGenerator.h
 *
 *  Created on: Oct 19, 2011
 *      Author: alfredo
 */

#ifndef VOTESGENERATOR_H_
#define VOTESGENERATOR_H_

#define DOCUMENTO_BASE  20000000

#include "../managers/VoterFile.h"
#include "../managers/DistrictFile.h"
#include "../managers/ElectionFile.h"
#include "../managers/ListFile.h"

class VotesGenerator {
private:
	VoterFile * voters;
	DistrictFile * districts;
	ElectionFile * elections;
	ListFile * lists;

	void VotesGenerator(){
		voters = VoterFile::getInstance();
		districts = DistrictFile::getInstance();
		elections = ElectionFile::getInstance();
		lists = ListFile::getInstance();
	}

	void vote(unsigned int documento) {
		VoterRecord::Key key = new VoterRecord::Key(documento);
		VoterRecord voter = voters->search(key);

		DistrictRecord::Key localidad = voter.getDistrict();
		DistrictRecord::Key provincia;
		DistrictRecord::Key pais;

		ElectionRecord elecccionMunicipal = elections->searchByDistrict(localidad);
		ElectionRecord elecccionProvincial = elections->searchByDistrict(provincia);
		ElectionRecord elecccionNacional = elections->searchByDistrict(pais);

		lists->searchByElection(elecccionMunicipal);
		lists->searchByElection(elecccionProvincial);
		lists->searchByElection(elecccionNacional);

	}
};

#endif /* VOTESGENERATOR_H_ */

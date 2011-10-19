/*
 * DistrictGenerator.h
 *
 * @author Alfredo Scoppa
 * @since OCt 18, 2011
 */

#ifndef DISTRICTGENERATOR_H_
#define DISTRICTGENERATOR_H_

#include <iostream>
#include "../managers/DistrictFile.h"

using namespace std;

class DistrictGenerator {
private:
	DistrictFile * districts;

	const string argentina = "Argentina";
	const string caba = "CABA";
	const string buenosAires = "Buenos Aires";
	const string santaFe = "Santa Fe";
	const string rosario = "Rosario";
	const string tigre = "Tigre";
	const string sanFernando = "San Fernando";
	const string palermo = "Palermo";
	const string retiro = "Retiro";

public:
	void DistrictGenerator() {
		districts = new DistrictFile();
	}

	void loadDistricts() {
		DistrictRecord * argentinaRecord = new DistrictRecord(argentina);
		DistrictRecord * cabaRecord = new DistrictRecord(caba, argentina);
		DistrictRecord * retiroRecord = new DistrictRecord(retiro, caba);
		DistrictRecord * palermoRecord = new DistrictRecord(palermo, caba);
		DistrictRecord * buenosAiresRecord = new DistrictRecord(buenosAires,argentina);
		DistrictRecord * tigreRecord = new DistrictRecord(tigre, buenosAires);
		DistrictRecord * sanFernandoRecord = new DistrictRecord(sanFernando,buenosAires);
		DistrictRecord * santaFeRecord = new DistrictRecord(santaFe, argentina);
		DistrictRecord * rosarioRecord = new DistrictRecord(rosario, santaFe);

		districts->insert(*argentinaRecord);
		districts->insert(*cabaRecord);
		districts->insert(*retiroRecord);
		districts->insert(*palermoRecord);
		districts->insert(*tigreRecord);
		districts->insert(*sanFernandoRecord);
		districts->insert(*santaFeRecord);
		districts->insert(*rosarioRecord);
	}
};

#endif /* DISTRICTGENERATOR_H_ */

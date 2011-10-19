/*
 * DistrictGenerator.h
 *
 * @author Alfredo Scoppa
 * @since OCt 18, 2011
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <iostream>
#include "../managers/DistrictFile.h"
#include "../managers/VoterFile.h"
#include "../records/ChargeRecord.h"

using namespace std;

class Generator {
private:
	DistrictFile * districts;
	VoterFile * voters;

	ChargeGenerator * chargesGenerator;

	const string ARGENTINA = "Argentina";
	const unsigned int NUMERO_DE_PROVINCIAS = 23;
	const unsigned int NUMERO_DE_MUNICIPIOS = 50;
	const unsigned int NUMERO_DE_VOTANTES = 10000;
	const string NOMBRE_BASE_PROVINCIAS = "Provincia";
	const string NOMBRE_BASE_MUNICIPIOS = "Municipio";
	const string PRESIDENTE = "Presidente";
	const string VICEPRESIDENTE = "Vicepresidente";
	const string SENADOR = "Senador";
	const string DIPUTADO = "Diputado";
	const string GOBERNADOR = "Gobernador";
	const string VICEGOBERNADOR = "Vicegobernador";
	const string LEGISLADOR = "Legislador";
	const string INTENDENTE = "Intendente";
	const string VICEINTENDENTE = "ViceIntendente";
	const string CONSEJAL = "Consejal";

	unsigned int date;
public:

	void Generator(unsigned int date) {
		this->date = date;
		this->chargesGenerator = new ChargeGenerator(date);
		this->districts = new DistrictFile();
		this->voters = new VoterFile();
	}

	void loadDistricts() {
		DistrictRecord * argentinaRecord = new DistrictRecord(ARGENTINA);
		DistrictRecord * provinciaRecord;
		DistrictRecord * municipioRecord;

		districts->insert(*argentinaRecord);

		string chargesNacionales [] = { PRESIDENTE, VICEPRESIDENTE, SENADOR, DIPUTADO };

		loadCharges(chargesNacionales,4,argentinaRecord);

		delete (argentinaRecord);

		for (unsigned int i = 0; i < NUMERO_DE_PROVINCIAS; i++) {
			string provincia = NOMBRE_BASE_PROVINCIAS + i;
			provinciaRecord = new DistrictRecord(provincia, argentina);

			districts->insert(*provinciaRecord);
			string chargesProvinciales [] = { GOBERNADOR, VICEGOBERNADOR, LEGISLADOR };
			loadCharges(chargesProvinciales,3,provinciaRecord);

			delete (provinciaRecord);

			for (unsigned int j = 0; j < NUMERO_DE_MUNICIPIOS; j++) {
				string municipio = NOMBRE_BASE_MUNICIPIOS + j + "-" + provincia;
				municipioRecord = new DistrictRecord(retiro, provincia);
				string chargesMunicipales [] = { INTENDENTE, VICEINTENDENTE, CONSEJAL };
				loadCharges(chargesMunicipales,3,municipioRecord);

				delete (municipio);
			}
		}
	}

	void loadCharges(String charges[], unsigned int numberOfCharges, DistrictRecord * district) {

		ChargeRecord * fatherRecord = new ChargeRecord(charges[0],district->getDistrictName());
		chargeFile->insert(*fatherRecord);

		loadElection(fatherRecord, district);

		ChargeRecord * childRecord;

		for(unsigned int i = 1; i<numberOfCharges; i++){
			childRecord = new ChargeRecord(charges[i],
					district->getDistrictName(), charges[0],
					district->getFatherName());

			chargeFile->insert(*childRecord);

			loadElection(childRecord, district);

			delete(childRecord);
		}

		delete(fatherRecord);
	}


	void loadElection(ChargeRecord * charge,DistrictRecord * district) {
		ElectionRecord * election = new ElectionRecord(this->date, charge->getKey(),district->getKey());

		//elections->insert(*election);

		delete (election);
	}

	void loadVoters(DistrictRecord * district){
		//VoterRecord * voter = new VoterRecord()
	}

};

#endif /* GENERATOR_H_ */

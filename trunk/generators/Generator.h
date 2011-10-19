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
#include "../managers/ListFile.h"
#include "../managers/VoteCountingFile.h"
#include "../managers/CandidateFile.h"
#include "../records/ChargeRecord.h"
#include "../utils/StringUtils.h"

using namespace std;

class Generator {
private:
	DistrictFile * districts;
	VoterFile * voters;
	//ListFile * lists;
	VoteCountingFile * voteCountings;
	CandidateFile * candidates;

	const string ARGENTINA = "Argentina";

	const unsigned int NUMERO_DE_PROVINCIAS = 23;
	const unsigned int NUMERO_DE_MUNICIPIOS = 50;
	const unsigned int NUMERO_DE_VOTANTES = 10000;
	const unsigned int NUMERO_DE_LISTAS_POR_ELECCION = 10000;
	const unsigned int DOCUMENTO_BASE = 20000000;

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
	const string LISTA_NOMBRE_BASE = "Lista";

	unsigned int date;
	unsigned int currentDNI;

public:

	void Generator(unsigned int date) {
		this->date = date;
		this->currentDNI = DOCUMENTO_BASE;

		this->districts = DistrictFile::getInstance();
		this->voters = VoterFile::getInstance();
		//this->lists = ListFile::getInstance();
		this->voteCountings = VoteCountingFile::getInstance();
		this->candidates = CandidateFile::getInstance();
	}

	void generate() {
		loadDistricts();
	}

	void loadDistricts() {
		DistrictRecord * argentinaRecord = new DistrictRecord(ARGENTINA);
		DistrictRecord * provinciaRecord;
		DistrictRecord * municipioRecord;

		districts->insert(*argentinaRecord);

		string chargesNacionales[] = { PRESIDENTE, VICEPRESIDENTE, SENADOR, DIPUTADO };

		loadCharges(chargesNacionales, 4, argentinaRecord);

		delete (argentinaRecord);

		for (unsigned int i = 0; i < NUMERO_DE_PROVINCIAS; i++) {
			string provincia = NOMBRE_BASE_PROVINCIAS + i;
			provinciaRecord = new DistrictRecord(provincia, argentina);

			districts->insert(*provinciaRecord);
			string chargesProvinciales[] = { GOBERNADOR, VICEGOBERNADOR, LEGISLADOR };
			loadCharges(chargesProvinciales, 3, provinciaRecord);

			delete (provinciaRecord);

			for (unsigned int j = 0; j < NUMERO_DE_MUNICIPIOS; j++) {
				string municipio = NOMBRE_BASE_MUNICIPIOS + j + "-" + provincia;
				municipioRecord = new DistrictRecord(retiro, provincia);
				string chargesMunicipales[] = { INTENDENTE, VICEINTENDENTE, CONSEJAL };

				loadVoters(municipioRecord);

				loadCharges(chargesMunicipales, 3, municipioRecord);

				delete (municipioRecord);
			}
		}
	}

	void loadCharges(String charges[], unsigned int numberOfCharges, DistrictRecord * district) {

		ChargeRecord * fatherRecord = new ChargeRecord(charges[0], district->getDistrictName());
		chargeFile->insert(*fatherRecord);

		loadElection(fatherRecord, district);

		ChargeRecord * childRecord;

		for (unsigned int i = 1; i < numberOfCharges; i++) {
			childRecord = new ChargeRecord(charges[i], district->getDistrictName(), charges[0],
							district->getFatherName());

			chargeFile->insert(*childRecord);

			loadElection(childRecord, district);

			delete (childRecord);
		}

		delete (fatherRecord);
	}

	void loadElection(ChargeRecord * charge, DistrictRecord * district) {
		ElectionRecord * election = new ElectionRecord(this->date, charge->getKey(),
						district->getKey());

		//elections->insert(*election);

		loadList(election);

		delete (election);
	}

	void loadList(ElectionRecord * election) {
		ListRecord * listRecord;

		for (unsigned int i = 0; i < NUMERO_DE_LISTAS_POR_ELECCION; i++) {
			string list = LISTA_NOMBRE_BASE + i;
			listRecord = new ListRecord(election->getKey(), LISTA_NOMBRE_BASE);
			//lists->insert(listRecord);

			loadVoteCountings(listRecord);

			delete (listRecord);
		}
	}

	void loadVoteCountings(ListRecord * list) {
		VoteCountingRecord * voteCountingRecord = new VoteCountingRecord(list->getKey(),
						list->getElection(), list->getElection().getCharge().getDistrict());

		voteCountings->insert(voteCountingRecord);

		delete (voteCountingRecord);
	}

	void loadCandidates(ListRecord * list) {
		unsigned int dni = currentDNI - 1;
		VoterRecord::Key * voterKey = new VoterRecord::Key(dni);

		CandidateRecord * candidateRecord = new CandidateRecord(list->getKey(voterKey),
						list->getElection().getCharge(), NULL);
	}

	void loadVoters(DistrictRecord * district) {

		for (unsigned int i = 0; i < NUMERO_DE_VOTANTES; i++) {
			string voterName = "";
			string address = "";
			unsigned int dni = currentDNI++;
			string voterKey = StringUtils::intToString(dni);
			list<ElectionRecord::Key> & elections;

			VoterRecord * voterRecord = new VoterRecord(voterName, dni, address, voterKey,
							district->getKey(), elections);

			voters->insert(*voterRecord);

			delete (voter);
		}
	}

};

#endif /* GENERATOR_H_ */
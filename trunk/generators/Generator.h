/*
 * Generator.h
 *
 * Esta herramienta genera los archivos necesarios para verificar el comportamiento del sistema
 * durante una eleccion en una fecha dada
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
#include "../managers/ChargeFile.h"
#include "../utils/StringUtils.h"

using namespace std;

#define ARGENTINA "Argentina"

#define NUMERO_DE_PROVINCIAS  23
#define NUMERO_DE_MUNICIPIOS  50
#define NUMERO_DE_VOTANTES  10000
#define NUMERO_DE_LISTAS_POR_ELECCION  10000
#define DOCUMENTO_BASE  20000000

#define NOMBRE_BASE_PROVINCIAS  "Provincia"
#define  NOMBRE_BASE_MUNICIPIOS  "Municipio"
#define  NOMBRE_BASE_LISTA "Lista"
#define  NOMBRE_BASE_VOTANTE  "Votante"

#define  PRESIDENTE  "Presidente"
#define  VICEPRESIDENTE "Vicepresidente"
#define  SENADOR  "Senador"
#define  DIPUTADO  "Diputado"
#define  GOBERNADOR  "Gobernador"
#define  VICEGOBERNADOR  "Vicegobernador"
#define  LEGISLADOR "Legislador"
#define  INTENDENTE "Intendente"
#define  VICEINTENDENTE  "ViceIntendente"
#define  CONSEJAL "Consejal"

class Generator {
private:
	DistrictFile * districts;
	VoterFile * voters;
	ListFile * lists;
	VoteCountingFile * voteCountings;
	CandidateFile * candidates;
	ChargeFile * charges;

	unsigned int date;
	unsigned int currentDNI;

public:
	/**
	 * @date en formato AAAAMMDD
	 */
	void GenerateElectionFor(unsigned int date) {
		this->date = date;
		this->currentDNI = DOCUMENTO_BASE;

		this->districts = DistrictFile::getInstance();
		this->voters = VoterFile::getInstance();
		this->lists = ListFile::getInstance();
		this->voteCountings = VoteCountingFile::getInstance();
		this->candidates = CandidateFile::getInstance();
		this->charges = ChargeFile::getInstance();
	}

	void generate() {
		loadDistricts();
	}

private:
	void loadDistricts() {
		DistrictRecord * argentinaRecord = new DistrictRecord(ARGENTINA);
		DistrictRecord * provinciaRecord;
		DistrictRecord * municipioRecord;

		districts->insert(*argentinaRecord);

		string chargesNacionales[] = { PRESIDENTE, VICEPRESIDENTE, SENADOR, DIPUTADO };

		loadCharges(chargesNacionales, 4, argentinaRecord);

		delete (argentinaRecord);

		for (unsigned int i = 0; i < NUMERO_DE_PROVINCIAS; i++) {
			string provincia = NOMBRE_BASE_PROVINCIAS + StringUtils::intToString(i);
			provinciaRecord = new DistrictRecord(provincia, ARGENTINA);

			districts->insert(*provinciaRecord);
			string chargesProvinciales[] = { GOBERNADOR, VICEGOBERNADOR, LEGISLADOR };
			loadCharges(chargesProvinciales, 3, provinciaRecord);

			delete (provinciaRecord);

			for (unsigned int j = 0; j < NUMERO_DE_MUNICIPIOS; j++) {
				string municipio = NOMBRE_BASE_MUNICIPIOS + StringUtils::intToString(j) + "-"
								+ provincia;
				municipioRecord = new DistrictRecord(municipio, provincia);
				string chargesMunicipales[] = { INTENDENTE, VICEINTENDENTE, CONSEJAL };

				loadVoters(municipioRecord);

				loadCharges(chargesMunicipales, 3, municipioRecord);

				delete (municipioRecord);
			}
		}
	}

	void loadCharges(string chargesArray[], unsigned int numberOfCharges, DistrictRecord * district) {

		ChargeRecord * fatherRecord =
						new ChargeRecord(chargesArray[0], district->getDistrictName());
		charges->insert(*fatherRecord);

		ElectionRecord * election = loadElection(fatherRecord, district);

		ChargeRecord * childRecord;

		for (unsigned int i = 1; i < numberOfCharges; i++) {
			childRecord = new ChargeRecord(chargesArray[i], district->getDistrictName(),
							chargesArray[0], district->getDistrictName());

			charges->insert(*childRecord);

			delete (childRecord);
		}

		delete (fatherRecord);
		delete (election);
	}

	ElectionRecord * loadElection(ChargeRecord * charge, DistrictRecord * district) {
		ElectionRecord * election = new ElectionRecord(this->date, charge->getKey(),
						district->getKey());

		//elections->insert(*election);

		loadList(election);

		return election;
	}

	void loadList(ElectionRecord * election) {
		ListRecord * listRecord;

		for (unsigned int i = 0; i < NUMERO_DE_LISTAS_POR_ELECCION; i++) {
			string list = NOMBRE_BASE_LISTA + StringUtils::intToString(i);
			listRecord = new ListRecord(election->getKey(), NOMBRE_BASE_LISTA);

			lists->insert(*listRecord);

			loadVoteCountings(listRecord);

			delete (listRecord);
		}
	}

	void loadVoteCountings(ListRecord * list) {
		VoteCountingRecord * voteCountingRecord = new VoteCountingRecord(list->getKey(),
						list->getElection().getCharge().getDistrict(), list->getElection(), 0);

		voteCountings->insert(*voteCountingRecord);

		std::list<DistrictRecord> disList;
		disList.push_back(districts->search(DistrictRecord::Key()));
		while(disList.back().hasFather()){
		disList.push_back(districts->search(disList.back().getFather()));
		}


		while(true){

		}

		delete (voteCountingRecord);
	}

	void loadCandidates(ListRecord * list) {
		unsigned int dni = currentDNI - 1;
		VoterRecord::Key * voterKey = new VoterRecord::Key(dni);

		CandidateRecord * candidateRecord = new CandidateRecord(list->getKey(),
						list->getElection().getCharge(), voterKey->getKey());

		candidates->insert(*candidateRecord);

		delete (candidateRecord);
	}

	void loadVoters(DistrictRecord * district) {

		for (unsigned int i = 0; i < NUMERO_DE_VOTANTES; i++) {
			string address = "";
			unsigned int dni = currentDNI++;
			string voterName = NOMBRE_BASE_VOTANTE + StringUtils::intToString(dni);
			string voterKey = "password";
			list<ElectionRecord::Key> elections;

			VoterRecord * voterRecord = new VoterRecord(voterName, dni, address, voterKey,
							district->getKey(), elections);

			voters->insert(*voterRecord);

			delete (voterRecord);
		}
	}

};
#endif /* GENERATOR_H_ */

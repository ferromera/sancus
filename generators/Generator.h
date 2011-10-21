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
#include "../estructuras/MathUtils.h"
#include "../managers/DistrictFile.h"
#include "../managers/VoterFile.h"
#include "../managers/ListFile.h"
#include "../managers/VoteCountingFile.h"
#include "../managers/CandidateFile.h"
#include "../managers/ChargeFile.h"
#include "../managers/ElectionFile.h"
#include "../utils/StringUtils.h"
#include "../utils/Logger.h"

using namespace std;

#define ARGENTINA "Argentina"

#define NUMERO_DE_PROVINCIAS  2
#define NUMERO_DE_MUNICIPIOS  2
#define NUMERO_DE_VOTANTES_POR_MUNICIPIO  10
#define NUMERO_DE_LISTAS_POR_ELECCION  10
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
	ElectionFile * elections;

	unsigned int date;
	unsigned int currentDNI;

public:
	/**
	 * @date en formato AAAAMMDD
	 */
	Generator(unsigned int date) {
		this->date = date;
		this->currentDNI = DOCUMENTO_BASE;

		this->districts = DistrictFile::getInstance();
		this->voters = VoterFile::getInstance();
		this->lists = ListFile::getInstance();
		this->voteCountings = VoteCountingFile::getInstance();
		this->candidates = CandidateFile::getInstance();
		this->charges = ChargeFile::getInstance();
		this->elections = ElectionFile::getInstance();
	}

	void GenerateElection() {
		Logger::getInstance()->info("SE CARGAN LOS DISTRITOS");
		Logger::getInstance()->info("*************************************************************************");

		loadDistricts();

		Logger::getInstance()->info("SE TERMINO DE CARGAR LOS DISTRITOS");
		Logger::getInstance()->info("*************************************************************************");

		Logger::getInstance()->info("SE CARGAN LOS CARGOS");
		Logger::getInstance()->info("*************************************************************************");

		//loadCharges();

		Logger::getInstance()->info("*************************************************************************");

	}

	//GENERA DISTRITOS Y VOTANTES
	void loadDistricts() {
		DistrictRecord * argentinaRecord = new DistrictRecord(ARGENTINA);
		DistrictRecord * provinciaRecord;
		DistrictRecord * municipioRecord;

		districts->insert(*argentinaRecord);

		delete (argentinaRecord);

		for (unsigned int i = 0; i < NUMERO_DE_PROVINCIAS; i++) {
			string provincia = NOMBRE_BASE_PROVINCIAS + StringUtils::intToString(i);
			provinciaRecord = new DistrictRecord(provincia, ARGENTINA);

			districts->insert(*provinciaRecord);

			delete (provinciaRecord);

			for (unsigned int j = 0; j < NUMERO_DE_MUNICIPIOS; j++) {
				string municipio = NOMBRE_BASE_MUNICIPIOS + StringUtils::intToString(j) + "-" + provincia;
				municipioRecord = new DistrictRecord(municipio, provincia);

				loadVoters(municipioRecord);

				districts->insert(*municipioRecord);

				delete (municipioRecord);
			}
		}
	}

	void loadCharges() {
		string chargesNacionales[] = { PRESIDENTE, VICEPRESIDENTE, SENADOR, DIPUTADO };
		string chargesProvinciales[] = { GOBERNADOR, VICEGOBERNADOR, LEGISLADOR };
		string chargesMunicipales[] = { INTENDENTE, VICEINTENDENTE, CONSEJAL };

		std::list<DistrictRecord> disList;
		disList.push_back(districts->search(DistrictRecord::Key()));
		while (disList.back().hasFather()) {
			disList.push_back(districts->search(disList.back().getFather()));
		}

		switch (disList.size()) {
		case 1:
			loadChargeForDistrict(chargesNacionales, 4, disList.front());
			break;
		case 2:
			loadChargeForDistrict(chargesProvinciales, 3, disList.front());
			break;

		case 3:
			loadChargeForDistrict(chargesMunicipales, 3, disList.front());
			break;
		}

		while (true) {
			DistrictRecord::Key lastSearched = disList.front().getKey();
			disList.clear();
			districts->search(lastSearched);
			try {
				disList.push_back(districts->next());
				while (disList.back().hasFather()) {
					disList.push_back(districts->search(disList.back().getFather()));
				}

				switch (disList.size()) {
				case 1:
					loadChargeForDistrict(chargesNacionales, 4, disList.front());
					break;
				case 2:
					loadChargeForDistrict(chargesProvinciales, 3, disList.front());
					break;

				case 3:
					loadChargeForDistrict(chargesMunicipales, 3, disList.front());
					break;
				}
			} catch (FileNextException) {
				break;
			}
		}

	}

	//CARGA elecciones, listas ,cargos, conteo, candidatos para un distrito
	void loadChargeForDistrict(string chargesArray[], unsigned int numberOfCharges, DistrictRecord & district) {
		Logger::getInstance()->info("SE CARGAN LOS CARGOS PARA EL DISTRITO "+ district.getKey().getString());
		Logger::getInstance()->info("*************************************************************************");

		ChargeRecord * fatherRecord = new ChargeRecord(chargesArray[0], district.getDistrictName());
		charges->insert(*fatherRecord);

		ElectionRecord * election = loadElection(fatherRecord, district);

		loadCandidates(election, fatherRecord);

		ChargeRecord * childRecord;

		for (unsigned int i = 1; i < numberOfCharges; i++) {
			childRecord = new ChargeRecord(chargesArray[i], district.getDistrictName(), chargesArray[0],
							district.getDistrictName());

			charges->insert(*childRecord);

			Logger::getInstance()->info("SE CARGAN LOS CANDIDATOS PARA EL CARGO "+ chargesArray[i]);
			Logger::getInstance()->info("*************************************************************************");

			loadCandidates(election, childRecord);

			delete (childRecord);
		}

		delete (fatherRecord);
		delete (election);
	}

	ElectionRecord * loadElection(ChargeRecord * charge, DistrictRecord & district) {
		Logger::getInstance()->info("SE CARGA LA ELECCION PARA EL CARGO "+ charge->getKey().getString() + " Y DISTRITO: "+ district.getDistrictName());
		Logger::getInstance()->info("*************************************************************************");

		ElectionRecord * election = new ElectionRecord(this->date, charge->getKey(), district.getKey());

		elections->insert(*election);

		loadList(election);

		return election;
	}

	void loadList(ElectionRecord * election) {
		ListRecord * listRecord;

		Logger::getInstance()->info("SE CARGAN LISTAS PARA LA ELECCION ");
		Logger::getInstance()->info("*************************************************************************");

		for (unsigned int i = 0; i < NUMERO_DE_LISTAS_POR_ELECCION; i++) {
			string list = NOMBRE_BASE_LISTA + election->getCharge().getString() + StringUtils::intToString(i);
			listRecord = new ListRecord(election->getKey(), list);

			lists->insert(*listRecord);

			loadVoteCountings(listRecord);

			delete (listRecord);
		}
	}

	void loadVoteCountings(ListRecord * list) {
		VoteCountingRecord * voteCountingRecord;

		DistrictRecord::Key elecDistrict = list->getElection().getCharge().getDistrict();
		std::list<DistrictRecord> disList;
		disList.push_back(districts->search(DistrictRecord::Key()));
		while (disList.back().hasFather()) {
			disList.push_back(districts->search(disList.back().getFather()));
		}
		std::list<DistrictRecord>::iterator itList;
		for (itList = disList.begin(); itList != disList.end(); itList++) {
			if (itList->getKey() == elecDistrict) {
				voteCountingRecord = new VoteCountingRecord(list->getKey(), disList.front().getKey(),
								list->getElection(), 0);
				voteCountings->insert(*voteCountingRecord);
				break;
			}
		}
		while (true) {
			DistrictRecord::Key lastSearched = disList.front().getKey();
			disList.clear();
			districts->search(lastSearched);
			try {
				disList.push_back(districts->next());
				while (disList.back().hasFather()) {
					disList.push_back(districts->search(disList.back().getFather()));
				}
				for (itList = disList.begin(); itList != disList.end(); itList++) {
					if (itList->getKey() == elecDistrict) {
						delete voteCountingRecord;
						voteCountingRecord = new VoteCountingRecord(list->getKey(), disList.front().getKey(),
										list->getElection(), 0);
						voteCountings->insert(*voteCountingRecord);
						break;
					}
				}
			} catch (FileNextException) {
				break;
			}
		}

		delete voteCountingRecord;
	}

	void loadCandidates(ElectionRecord * election, ChargeRecord * charge) {

		try {
			ListRecord listRecord = lists->searchByElection(election->getKey());

			currentDNI -= MathUtils::randomNumber(1, 10);

			unsigned int dni = currentDNI;

			VoterRecord::Key * voterKey = new VoterRecord::Key(dni);

			CandidateRecord * candidateRecord = new CandidateRecord(listRecord.getKey(), charge->getKey(),
							voterKey->getKey());

			candidates->insert(*candidateRecord);

			delete voterKey;
			delete (candidateRecord);

			while (true) {
				try {
					listRecord = lists->nextElection();

					currentDNI -= MathUtils::randomNumber(1, 10);

					unsigned int dni = currentDNI;

					VoterRecord::Key * voterKey = new VoterRecord::Key(dni);

					CandidateRecord * candidateRecord = new CandidateRecord(listRecord.getKey(), charge->getKey(),
									voterKey->getKey());

					candidates->insert(*candidateRecord);

					delete voterKey;
					delete candidateRecord;

				} catch (FileNextException) {
					break;
				}
			}

		} catch (FileSearchException) {
			return;
		}

	}

	void loadVoters(DistrictRecord * district) {

		for (unsigned int i = 0; i < NUMERO_DE_VOTANTES_POR_MUNICIPIO; i++) {
			string address = "";
			unsigned int dni = currentDNI++;
			string voterName = NOMBRE_BASE_VOTANTE + StringUtils::intToString(dni);
			string voterKey = "password";
			list<ElectionRecord::Key> elections;

			VoterRecord * voterRecord = new VoterRecord(voterName, dni, address, voterKey, district->getKey(),
							elections);

			voters->insert(*voterRecord);

			delete (voterRecord);
		}
	}

};
#endif /* GENERATOR_H_ */

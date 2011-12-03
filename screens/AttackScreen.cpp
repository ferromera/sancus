/*
 * AttackScreen.cpp
 *
 *  Created on: 02/12/2011
 *      Author: juanmromera
 */

#include "AttackScreen.h"
#include "../core/VoteApp.h"
#include "../managers/DistrictFile.h"
#include "../managers/FileManagerExceptions.h"
#include "../managers/VoterFile.h"
#include "../records/VoterRecord.h"
#include "../records/ElectionRecord.h"
#include "../records/CandidateRecord.h"
#include "../records/VoteCountingRecord.h"
#include "../managers/ElectionFile.h"
#include "../managers/AdministratorFile.h"
#include "../records/AdministratorRecord.h"
#include "../estructuras/SecurityStrategy.h"
#include "../estructuras/RsaSecurity.h"
#include "../cryptanalysis/RSABreaker.h"
#include "../cryptanalysis/Kasiski.h"

AttackScreen::AttackScreen() {
}
void AttackScreen::draw() {
	int opcion;
	VoteApp * app = VoteApp::getInstance();
	system("clear");
	std::cout << "PANTALLA DE ATAQUES A ARCHIVOS" << std::endl;
	std::cout << "Elija el archivo que quiere atacar y apriete ENTER" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "1- Archivo de Administrador" << std::endl;
	std::cout << "2- Archivo de Votantes" << std::endl;
	std::cout << "3- Reporte de resultados por lista" << std::endl;
	std::cout << "4- Reporte de resultados por eleccion" << std::endl;
	std::cout << "5- Reporte de resultados por distrito" << std::endl;
	std::cout << "6- Otro archivo RSA" << std::endl;
	std::cout << "7- Volver a pantalla anterior" << std::endl;
	std::cout << "" << std::endl;
	while (true) {
		opcion = IstreamUtils::getUint();
		switch (opcion) {
		case 1:
			attackAdminFile();
			return;
		case 2:
			attackVoterFile();
			return;
		case 3:
			attackreportFilebyList();
			return;
		case 4:
			attackreportFilebyElection();
			return;
		case 5:
			attackreportFileByDistrict();
			return;
		case 6:
			attackRSAFile();
			return;
		case 7:
			app->setActualScreen(CRYPT_SCREEN);
			return;
		default:
			std::cout << "Opcion Incorrecta, elija nuevamente" << std::endl;
		}
	}
}
void AttackScreen::attackAdminFile() {
	RsaSecurity* security = new RsaSecurity(10);
	RsaPublicKey publicKey = security->getPublicKey();
	RSABreaker::attack2(ADMINISTRATOR_FILE_DATA_PATH, publicKey.n, publicKey.e);
	std::cout << "Presione enter para volver a la pantalla de ataques."<< std::endl;
	IstreamUtils::getString();
}
void AttackScreen::attackVoterFile() {
	RsaSecurity* security = new RsaSecurity(10);
	RsaPublicKey publicKey = security->getPublicKey();
	RSABreaker::attack2(VOTER_FILE_DATA_PATH, publicKey.n, publicKey.e);
	std::cout << "Presione enter para volver a la pantalla de ataques."<< std::endl;
	IstreamUtils::getString();
}
void AttackScreen::attackRSAFile() {
	std::string path;
	system("clear");
	std::cout << "Ingrese la ruta del archivo RSA." << std::endl;
	std::cout << "" << std::endl;
	path = IstreamUtils::getString();
	std::cout << "" << std::endl;

	RsaSecurity* security = new RsaSecurity(10);
	RsaPublicKey publicKey = security->getPublicKey();
	RSABreaker::attack2(path, publicKey.n, publicKey.e);
	std::cout << "Presione enter para volver a la pantalla de ataques."<< std::endl;
	IstreamUtils::getString();
}
void AttackScreen::attackreportFileByDistrict() {
	std::string nameReporte;
	system("clear");
	std::cout << "Ingrese la ruta del archivo de reporte por distrito:" << std::endl;
	std::cout << "" << std::endl;
	nameReporte = IstreamUtils::getString();
	std::cout << "" << std::endl;
	Kasiski::attackDistrict(nameReporte);
	std::cout << "Presione enter para volver a la pantalla de ataques."<< std::endl;
	IstreamUtils::getString();
}
void AttackScreen::attackreportFilebyList() {
	std::string nameReporte;
	system("clear");
	std::cout << "Ingrese la ruta del archivo de reporte por lista:" << std::endl;
	std::cout << "" << std::endl;
	nameReporte = IstreamUtils::getString();
	std::cout << "" << std::endl;
	Kasiski::attackList(nameReporte);
	std::cout << "Presione enter para volver a la pantalla de ataques."<< std::endl;
	IstreamUtils::getString();
}
void AttackScreen::attackreportFilebyElection() {
	std::string nameReporte;
	system("clear");
	std::cout << "Ingrese la ruta del archivo de reporte por eleccion:" << std::endl;
	std::cout << "" << std::endl;
	nameReporte = IstreamUtils::getString();
	std::cout << "" << std::endl;
	Kasiski::attackElection(nameReporte);
	std::cout << "Presione enter para volver a la pantalla de ataques."<< std::endl;
	IstreamUtils::getString();
}

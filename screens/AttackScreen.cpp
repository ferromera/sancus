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
void AttackScreen::draw(){
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
	std::cout << "6- Volver a pantalla anterior" << std::endl;
	std::cout << "" << std::endl;
	while (true) {
		opcion = IstreamUtils::getUint();
		switch (opcion) {
		case 1:
			attackAdminFile();
			break;
		case 2:
			attackVoterFile();
			break;
		case 3:
			attackreportFilebyList();
			break;
		case 4:
			attackreportFilebyElection();
			break;
		case 5:
			attackreportFileByDistrict();
			break;
		case 6:
			app->setActualScreen(ADM_SCREEN);
			return;
		default:
			std::cout << "Opcion Incorrecta, elija nuevamente" << std::endl;
		}
	}
}
void AttackScreen::attackAdminFile()
{
	RsaSecurity* security = new RsaSecurity(10);
	RsaPublicKey publicKey = security->getPublicKey();
	RSABreaker::attack2(ADMINISTRATOR_FILE_DATA_PATH,publicKey.n,publicKey.e);
}
void AttackScreen::attackVoterFile()
{
	RsaSecurity* security = new RsaSecurity(10);
	RsaPublicKey publicKey = security->getPublicKey();
	RSABreaker::attack2(VOTER_FILE_DATA_PATH,publicKey.n,publicKey.e);
}
void AttackScreen::attackreportFileByDistrict()
{
	std::string nameReporte;
	system("clear");
	std::cout
			<< "Ingrese algun reporte del tipo 'resultByDistrictXXXXXXXXXXXXXX' para romper"
			<< std::endl;
	std::cout << "" << std::endl;
	nameReporte = IstreamUtils::getString();
	std::cout << "" << std::endl;
	Kasiski::attackDistrict(nameReporte);
}
void AttackScreen::attackreportFilebyList()
{
	std::string nameReporte;
	system("clear");
	std::cout
			<< "Ingrese algun reporte del tipo 'resultByListXXXXXXXXXXXXXX' para romper"
			<< std::endl;
	std::cout << "" << std::endl;
	nameReporte = IstreamUtils::getString();
	std::cout << "" << std::endl;
	Kasiski::attackList(nameReporte);
}
void AttackScreen::attackreportFilebyElection()
{
	std::string nameReporte;
	system("clear");
	std::cout
			<< "Ingrese algun reporte del tipo 'resultByElectionXXXXXXXXXXXXXX' para romper"
			<< std::endl;
	std::cout << "" << std::endl;
	nameReporte = IstreamUtils::getString();
	std::cout << "" << std::endl;
	Kasiski::attackElection(nameReporte);
}

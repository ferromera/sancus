/*
 * DataFileScreen.cpp
 *
 *  Created on: 20/11/2011
 *      Author: juanmromera
 */

#include "DataFileScreen.h"
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

DataFileScreen::DataFileScreen() {
}
void DataFileScreen::draw() {
	int opcion;
	VoteApp * app = VoteApp::getInstance();
	system("clear");
	std::cout << "PANTALLA DE CREACION DE ARCHIVOS DE REPORTES" << std::endl;
	std::cout << "Elija el archivo que quiere crear y apriete ENTER" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "1- Archivo de Distritos" << std::endl;
	std::cout << "2- Archivo de Votantes" << std::endl;
	std::cout << "3- Archivo de Elecciones" << std::endl;
	std::cout << "4- Archivo de Candidatos" << std::endl;
	std::cout << "5- Archivo de Listas" << std::endl;
	std::cout << "6- Archivo de Cargos" << std::endl;
	std::cout << "7- Archivo de Conteo" << std::endl;
	std::cout << "8- Archivo de Administrador" << std::endl;
	std::cout << "9- Volver a pantalla anterior" << std::endl;
	std::cout << "" << std::endl;
	while (true) {
		opcion = IstreamUtils::getUint();
		switch (opcion) {
		case 1:
			createReportFileByDistrict();
			break;
		case 2:
			createReportFileByVoter();
			break;
		case 3:
			createReportFileByElection();
			break;
		case 4:
			createReportFileByCandidate();
			break;
		case 5:
			createReportFileByList();
			break;
		case 6:
			createReportFileByCharge();
			break;
		case 7:
			createReportFileByCounting();
			break;
		case 8:
			createReportFileByAdministrator();
			break;
		case 9:
			app->setActualScreen(ADM_SCREEN);
			return;
		default:
			std::cout << "Opcion Incorrecta, elija nuevamente" << std::endl;
		}
	}
}
void DataFileScreen::createReportFileByDistrict(){

	DistrictFile *file = DistrictFile::getInstance();
	file->createReportFile();
}
void DataFileScreen::createReportFileByVoter()
{
	VoterFile *file = VoterFile::getInstance();
	file->createReportFile();
}
void DataFileScreen::createReportFileByElection()
{
	ElectionFile *file = ElectionFile::getInstance();
	file->createReportFile();
}
void DataFileScreen::createReportFileByList(){
	ListFile *file = ListFile::getInstance();
	file->createReportFile();
}
void DataFileScreen::createReportFileByCandidate(){
	CandidateFile *file = CandidateFile::getInstance();
	file->createReportFile();
}
void DataFileScreen::createReportFileByCharge(){
	ChargeFile *file = ChargeFile::getInstance();
	file->createReportFile();
}
void DataFileScreen::createReportFileByCounting()
{
	VoteCountingFile *file = VoteCountingFile::getInstance();
	file->createReportFile();
}
void DataFileScreen::createReportFileByAdministrator()
{
	AdministratorFile *file = AdministratorFile::getInstance();
	file->createReportFile();
}


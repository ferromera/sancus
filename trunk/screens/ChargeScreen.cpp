/*
 * ChargeScreen.cpp
 *
 *  Created on: 16/10/2011
 *      Author: juanmromera
 */

#include "ChargeScreen.h"
#include "../core/VoteApp.h"
#include <cstdio>

ChargeScreen::ChargeScreen() {
}
void ChargeScreen::draw()
{
	int opcion;
	VoteApp* app = VoteApp::getInstance();
	system("clear");
	std::cout<<"PANTALLA DE ALTA"<<std::endl;
	std::cout<<"Eliga el archivo donde quiera ingresar un alta y apriete ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	std::cout<<"1- Archivo de Distritos"<<std::endl;
	std::cout<<"2- Archivo de Votantes"<<std::endl;
	std::cout<<"3- Archivo de Elecciones"<<std::endl;
	std::cout<<"4- Archivo de Candidatos"<<std::endl;
	std::cout<<"5- Archivo de Listas"<<std::endl;
	std::cout<<"6- Archivo de Cargos"<<std::endl;
	std::cout<<"7- Archivo de Conteo"<<std::endl;
	std::cout<<"8- Volver a pantalla anterior"<<std::endl;
	std::cout<<""<<std::endl;
	while(true)
	{
		opcion = IstreamUtils::getUint();
		switch(opcion)
		{
				case 1:	while(chargeDistrict() == 'S'){};return;
				case 2: while(chargeVoter() == 'S'){};return;
				//case 3: app->setActualScreen(BAJA_SCREEN);return;
				//case 4: app->setActualScreen(CONSULTA_SCREEN);return;
				default:std::cout<<"Opcion Incorrecta, eliga nuevamente"<<std::endl;
		}
	}
}
char ChargeScreen::chargeDistrict()
{
	std::string district;
	std::string father;
	//DistrictRecord record;
	system("clear");

	std::cout<<"Alta de un registro del Archivo de Distritos"<<std::endl;
	std::cout<<""<<std::endl;
	std::cout<<"Ingrese el Nombre de un Distrito y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	district = IstreamUtils::getString();
	std::cout<<""<<std::endl;
	std::cout<<"Ingrese el Nombre del padre del Distrito anterior, si no tiene padre no ingrese nada y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	father = IstreamUtils::getString();
	std::cout<<""<<std::endl;

//	if(father.compare("") == 0)
//		record = DistrictRecord(district);
//	else
//		record = DistrictRecord(district,father);

//	DistrictFile* dFile = DistrictFile::getInstance();
//	try
//	{
//		dFile->insert(record);
//	}
//	catch(DuplicateRecordException)
//	{
//		std::cout<<"No se pude Insertar el Registro por que ya existe en el Archivo"<<std::endl;
//		std::cout<<""<<std::endl;
//		std::cout<<"Quiere agregar otro registro al archivo de Distritos S/N"<<std::endl;
//		return doQuestion();
//	}
	//std::cout<<"Se inserto correctamente el siguiente Registo distrito: ";
	//std::cout<<record.getKey().getString()<<std::endl;
	std::cout<<""<<std::endl;
	std::cout<<"Quiere agregar otro registro al archivo de Distritos S/N"<<std::endl;
	return doQuestion();
}
char ChargeScreen::chargeVoter(){
	uint32_t dni;
	std::string name;
	std::string apellido;
	std::string distrito;
	std::string domicilio;
	std::string clave;
	system("clear");
	std::cout<<"Alta de un registro del Archivo de Votantes"<<std::endl;
	std::cout<<""<<std::endl;
	std::cout<<"Ingrese el DNI y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	dni = IstreamUtils::getUint();
	std::cout<<""<<std::endl;
	std::cout<<"Ingrese el Nombre y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	name = IstreamUtils::getString();
	std::cout<<""<<std::endl;
	std::cout<<"Ingrese el Apellido y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	apellido = IstreamUtils::getString();
	std::cout<<""<<std::endl;
	std::cout<<"Ingrese el Distrito y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	distrito = IstreamUtils::getString();
	std::cout<<""<<std::endl;
	std::cout<<"Ingrese el Domicilio y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	domicilio = IstreamUtils::getString();
	std::cout<<""<<std::endl;
	std::cout<<"Ingrese la Contraseña y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	clave = IstreamUtils::getString();
	/*
	VoterRecord record;
	// Busco si existe el Distrito
	std::list<DistrictRecord> listaDeDistritos;
	std::list<ElectionRecord::Key> listaDeElecciones;
	DistrictFile* dFile = DistrictFile::getInstance();
	ElectionFile* eFile = Election::getInstance();
	DistrictRecord dRecord = DistrictRecord(distrito);void
	try
	{
		DistrictRecord dRecordReturned = dFile->search(dRecord.getKey());
	}
	catch(FileSearchException e)
	{
		std::cout<<"No se pude Insertar el Registro por que no existe el Distrito elegido"<<std::endl;
		std::cout<<""<<std::endl;
		std::cout<<"Quiere agregar otro registro al archivo de Votantes S/N"<<std::endl;
		return doQuestion();
	}
		//Busco los distrvoiditos siguientes
		while(dRecordReturned.hasFather())
		{
			listaDeDistritos.push_back(dRecordReturned);
			dRecordReturned = dFile->search(dRecordReturned.getFather());
		}
		listaDeDistritos.push_back(dRecordReturned);
		//Busco elecciones
		std::list<DistrictRecord>::iterator itDist = listaDeDistritos.begin();
		for(int i = 0; i < listaDeDistritos.size();i++,itDist++)
		{
			try
			{
				ElectionRecord recElection = eFile->searchByDistrict((*itDist).getKey());
				while(true)
				{
					listaDeElecciones.push_back(recElection.getKey());
					try
					{
						recElection = eFile->nextDistrict();
					}
					catch(FileNextException e)
					{
						break;
					}
				}
			}
			catch(FileSearchException e)
			{
				break;
			}
		}
		record = VoterRecord(name,dni,domicilio,clave,listaDeDistritos.front().getKey(),listaDeElecciones);
		VoterFile* vFile = VoterFile::getInstance();
		try
		{
			vFile->insert(record);
		}catch(DuplicateRecordException)
		{
			std::cout<<"Error el Registro ya existe"<<std::endl;
			std::cout<<""<<std::endl;
			std::cout<<"Quiere agregar otro registro al archivo de Votantes S/N"<<std::endl;
			return doQuestion();
		}void

	std::cout<<"Se inserto correctamente el siguiente Registo votante: ";
	std::cout<<record.getKey().getString()<<std::endl;
	*/
	std::cout<<""<<std::endl;
	std::cout<<"Quiere agregar otro registro al archivo de Votantes S/N"<<std::endl;
	return doQuestion();
}
void ChargeScreen::chargeElection(){}
void ChargeScreen::chargeList(){}
void ChargeScreen::chargeCandidate(){}
void ChargeScreen::chargeCharge(){}
void ChargeScreen::chargeCounting(){}
char ChargeScreen::doQuestion()
{
	while(true)
	{
		char resp = IstreamUtils::getChar();
		switch(resp)
		{
			case 'S': return resp;
			case 'N': return resp;
			default : std::cout<<"Respuesta no valida use S/N"<<std::endl;
		}
	}
}


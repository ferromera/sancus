/*
 * ResultScreen.cpp
 *
 *  Created on: 17/10/2011
 *      Author: Romera 2
 */

#include "ResultScreen.h"
#include "../core/VoteApp.h"

ResultScreen::ResultScreen() {
}
void ResultScreen::draw()
{
	int opcion;
	VoteApp* app = VoteApp::getInstance();
	system("clear");
	std::cout<<"PANTALLA DE Resultados"<<std::endl;
	std::cout<<"Eliga el criterio por el cual quiere ver el resultado y apriete ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	std::cout<<"1- Por Distrito"<<std::endl;
	std::cout<<"2- Por Lista"<<std::endl;
	std::cout<<"3- Por Eleccion"<<std::endl;
	std::cout<<"4- Volver a pantalla anterior"<<std::endl;
	std::cout<<""<<std::endl;
	while(true)
	{
		opcion = IstreamUtils::getUint();
		switch(opcion)
		{
				case 1:	while(resultByDistrict() == 'S'){};return;
				case 2: while(resultByList() == 'S'){};return;
				case 3: while(resultByElection() == 'S'){};return;
				case 4: app->setActualScreen(ADM_SCREEN);return;
				default:std::cout<<"Opcion Incorrecta, eliga nuevamente"<<std::endl;
		}
	}
}
char ResultScreen::resultByDistrict()
{
	std::string distrito;
	unsigned int fecha;
	system("clear");
	std::cout<<"Ingrese la fecha de las elecciones que quiere consultar (AAAAMMDD)"<<std::endl;
		std::cout<<""<<std::endl;
		fecha = IstreamUtils::getUint();
		std::cout<<""<<std::endl;
	std::cout<<"Ingrese el Distrito para ver los resultados de las elecciones"<<std::endl;
	std::cout<<""<<std::endl;
	distrito = IstreamUtils::getString();
	std::cout<<""<<std::endl;

	VoteCountingFile* vFile = VoteCountingFile::getInstance();
	DistrictRecord::Key districtKey = DistrictRecord::Key(distrito);
	VoteCountingRecord record;
	try
	{
		record = vFile->searchByDistrict(districtKey);
	}catch(FileSearchException e)
	{
		std::cout<<"No se encuentran resultados para el distrito ingresado"<<std::endl;
		std::cout<<""<<std::endl;
		std::cout<<"Quiere realizar otra busqueda S/N"<<std::endl;
		return doQuestion();
	}
	std::cout<<"Resultados del distrito: "<<distrito<<" para elecciones de la fecha: "<<fecha<<endl;
	unsigned int showedResults=0;
	while(true)
	{
		if(record.getElection().getDate() == fecha){
			std::cout<<"Lista: "  << record.getList().getName() << ", Cargo: "<< record.getElection().getCharge().getCharge()<<" de "<<
			record.getElection().getCharge().getDistrict().getString()<< ", Votos: "<<  record.getCount() <<std::endl;
			showedResults++;
		}
		if(showedResults== 5){
			showedResults=0;
			std::cout<<endl<<"Presione ENTER para continuar o Q para salir"<<endl;
			if(IstreamUtils::getString()[0]=='Q'){
				std::cout<<""<<std::endl;
				std::cout<<"Quiere realizar otra busqueda S/N"<<std::endl;
				return doQuestion();
			}else{
				system("clear");
			}
		}
		try
		{
			record = vFile->nextDistrict();
		}catch(FileNextException e)
		{
			break;
		}
	}

	std::cout<<""<<std::endl;
	std::cout<<"Quiere realizar otra busqueda S/N"<<std::endl;
	return doQuestion();
}
char ResultScreen::resultByList()
{
	int fecha;
	std::string name;
	std::string cargo;
	std::string distrito;
	system("clear");
	std::cout<<"Ingrese el nombre de la lista y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	name = IstreamUtils::getString();
	std::cout<<"Ingrese la fecha de la Eleccion a la que la Lista se presentara con el siguiente formato YYYYMMDD y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	bool fechaIncorrecta = true;
	while(fechaIncorrecta)
	{
		fecha = IstreamUtils::getUint();
		std::cout<<""<<std::endl;
		if(fecha < Time::getCurrentDate())
		{
			std::cout<<"La fecha es menor a la actual ingrese una fecha posterior al "<< Time::getCurrentDate() <<std::endl;
			std::cout<<""<<std::endl;
		}
		else
			fechaIncorrecta = false;
	}
	std::cout<<"Ingrese el Nombre del Cargo a votarse en la eleccion a la cual la Lista se presentara y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	cargo = IstreamUtils::getString();
	std::cout<<""<<std::endl;
	std::cout<<"Ingrese el Distrito donde se elegira al Cargo anteriormente seleccionado y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	distrito = IstreamUtils::getString();
	std::cout<<""<<std::endl;
	/*
	VoteCountingFile* vFile = VoteCountingFile::getInstance();
	ElectionRecord::Key electionKey = ElectionRecord::Key(fecha,cargo,distrito);
	ListRecord::Key listKey = ListRecord::Key(name,electionKey);
	VoteCountingRecord record;
	try
	{
		record = vFile->searchByList(listKey);
	}catch(FileSearchException e)
	{
		std::cout<<"No se encuentran resultados para la Lista ingresado"<<std::endl;
		std::cout<<""<<std::endl;
		std::cout<<"Quiere realizar otra busqueda S/N"<<std::endl;
		return doQuestion();
	}
	while(true)
	{
		std::cout<<"Eleccion: "<< record.getElection() << "Distrito: "  << record.getDistrict() << " Votos: "  record.getVotes() <<std:endl;
		try
		{
			record = vFile->nexthByList();
		}catch(FileSearchException e)
		{
			break;
		}
	}*/
	std::cout<<""<<std::endl;
	std::cout<<"Quiere realizar otra busqueda S/N"<<std::endl;
	return doQuestion();

}
char ResultScreen::resultByElection()
{
	int fecha;
	std::string cargo;
	std::string distrito;
	system("clear");
	std::cout<<"Ingrese la fecha de la Eleccion a consultar con el siguiente formato YYYYMMDD y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	bool fechaIncorrecta = true;
	while(fechaIncorrecta)
	{
		fecha = IstreamUtils::getUint();
		std::cout<<""<<std::endl;
		if(fecha < Time::getCurrentDate())
		{
			std::cout<<"La fecha es menor a la actual ingrese una fecha posterior al "<< Time::getCurrentDate() <<std::endl;
			std::cout<<""<<std::endl;
		}
		else
			fechaIncorrecta = false;
	}
	std::cout<<"Ingrese el Nombre del Cargo de la eleccion a consultar y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	cargo = IstreamUtils::getString();
	std::cout<<""<<std::endl;
	std::cout<<"Ingrese el Distrito  de la eleccion a consultar y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	distrito = IstreamUtils::getString();
	std::cout<<""<<std::endl;
	/*
	VoteCountingFile* vFile = VoteCountingFile::getInstance();
	ElectionRecord::Key electionKey = ElectionRecord::Key(fecha,cargo,distrito);
	VoteCountingRecord record;
	try
	{
		record = vFile->searchByElection(electionKey);
	}catch(FileSearchException e)
	{
		std::cout<<"No se encuentran resultados para la Eleccion ingresado"<<std::endl;
		std::cout<<""<<std::endl;
		std::cout<<"Quiere realizar otra busqueda S/N"<<std::endl;
		return doQuestion();
	}
	while(true)
	{
		std::cout<<"Lista: "<< record.getList() << "Distrito: "  << record.getDistrict() << " Votos: "  record.getVotes() <<std:endl;
		try
		{
			record = vFile->nexthByElection();
		}catch(FileSearchException e)
		{
			break;
		}
	}*/

	std::cout<<""<<std::endl;
	std::cout<<"Quiere realizar otra busqueda S/N"<<std::endl;
	return doQuestion();
}
char ResultScreen::doQuestion()
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
ResultScreen::~ResultScreen() {
}

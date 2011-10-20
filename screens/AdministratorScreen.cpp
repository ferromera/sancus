/*
 * AdministratorScreen.cpp
 *
 *  Created on: 16/10/2011
 *      Author: juanmromera
 */

#include "AdministratorScreen.h"
#include "../core/VoteApp.h"

AdministratorScreen::AdministratorScreen() {

}
void AdministratorScreen::draw()
{
	int opcion;
	VoteApp* app = VoteApp::getInstance();
	system("clear");
	std::cout<<"PANTALLA DE ADMINISTRADOR"<<std::endl;
	std::cout<<"Eliga una opcion y apriete ENTER"<<std::endl;
	std::cout<<""<<std::endl;
	std::cout<<"1- Alta de Datos"<<std::endl;
	std::cout<<"2- Modificación de Datos"<<std::endl;
	std::cout<<"3- Baja de Datos"<<std::endl;
	std::cout<<"4- Consulta de Datos"<<std::endl;
	std::cout<<"5- Volver a pantalla principal"<<std::endl;
	std::cout<<"6- Salir de VoteApp"<<std::endl;
	std::cout<<""<<std::endl;
	while(true)
	{
		opcion = IstreamUtils::getUint();
		switch(opcion)
		{
				case 1:	app->setActualScreen(ALTA_SCREEN);return;
				case 2: app->setActualScreen(MOD_SCREEN);return;
				case 3: app->setActualScreen(BAJA_SCREEN);return;
				case 4: app->setActualScreen(CONSULTA_SCREEN);return;
				case 5: app->setActualScreen(LOGIN_SCREEN);return;
				case 6: app->setQuit(true);return;
				default:std::cout<<"Opcion Incorrecta, eliga nuevamente"<<std::endl;
		}
	}
}


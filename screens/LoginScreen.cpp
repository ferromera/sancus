/*
 * LoginScreen.cpp
 *
 *  Created on: 14/10/2011
 *      Author: juanmromera
 */

#include "LoginScreen.h"
#define MODE_DEBUG true

LoginScreen::LoginScreen() {
}

void  LoginScreen::draw()
{
	system("clear");

	VoteApp* app = VoteApp::getInstance();
	uint32_t dni;
	std::string clave;



	std::cout<<"/////////////////////////////////////////"<<std::endl;
	std::cout<<"       SISTEMA DE VOTO ELECTRONICO       "<<std::endl;
	std::cout<<"/////////////////////////////////////////"<<std::endl;
	std::cout<<"            GRUPO KAMMERICH				 "<<std::endl;
	std::cout<<"         FACULTAD DE INGENIERIA 		 "<<std::endl;
	std::cout<<"      UNIVERSIDAD DE BUENOS AIRES        "<<std::endl;
	std::cout<<"/////////////////////////////////////////"<<std::endl;

	std::cout<<""<<std::endl;
	std::cout<<""<<std::endl;


	bool incorrectUser = true;
	bool userAdmin = false;
	std::cout<<"Por favor ingrese su usuario (DNI) y presione ENTER"<<std::endl;
	std::cout<<""<<std::endl;

	while(incorrectUser)
	{
		dni = IstreamUtils::getUint();
		std::cout<<""<<std::endl;

		if(MODE_DEBUG && dni == 0)
		{
			incorrectUser = false;
			userAdmin = true;
			break;
		}
		if(!validateDNI(dni))
		{
			if(validateAdmin(dni))
			{
				incorrectUser = false;
				userAdmin = true;
			}
			else
			{
				std::cout<<"Usuario Inexistente ingrese nuevamente"<<std::endl;
				std::cout<<""<<std::endl;
			}

		}
		else
		{
			incorrectUser = false;
		}
	}
	bool incorrectPassword = true;
	std::cout<<""<<std::endl;

	while(incorrectPassword)
	{
		std::cout<<"Por favor ingrese contraseña y presione enter"<<std::endl;
		std::cout<<""<<std::endl;
		clave =  IstreamUtils::getString();
		std::cout<<""<<std::endl;

		if(MODE_DEBUG && clave.compare("0") == 0)
		{
			incorrectPassword = false;
			break;
		}

		if( (!userAdmin && clave.compare(user.getUserKey()) != 0)
				|| (userAdmin && clave.compare(adminUser.getPassword()) != 0))
		{
			std::cout<<""<<std::endl;
			std::cout<<"Contraseña incorrecta"<<std::endl;
			std::cout<<""<<std::endl;
			std::cout<<"¿Quiere ingresar la contraseña nuevamente? S/N"<<std::endl;
			std::cout<<""<<std::endl;

			bool nC = true;

			while(nC)
			{
				char resp = IstreamUtils::getChar();
				switch(resp)
				{
					case 'S': nC = false;break;
					case 'N': app->setActualScreen(LOGIN_SCREEN);return;
					default : std::cout<<"Respuesta no valida use S/N"<<std::endl;
				}
			}
		}
		else
		{	if(!userAdmin)
				app->setUserLogin(user);
			incorrectPassword = false;
		}
	}
	if(userAdmin)
		app->setActualScreen(ADM_SCREEN);
	else
		app->setActualScreen(ELECTION_SCREEN);
	return;
}

bool LoginScreen::validateDNI(uint32_t dni)
{
	VoterFile* voteFile = VoterFile::getInstance();
	VoterRecord::Key voterKey = VoterRecord::Key(dni);
	try{
		 user  = voteFile->search(voterKey);
	}catch(FileSearchException e)
	{
		return false;
	}
	return true;
}

bool LoginScreen::validateAdmin(uint32_t user)
{
	AdministratorFile* adminFile = AdministratorFile::getInstance();
	AdministratorRecord::Key adminKey = AdministratorRecord::Key(user);
	try{
		adminUser = adminFile->search(adminKey);
	}catch(FileSearchException e)
	{
		return false;
	}
	return true;
}


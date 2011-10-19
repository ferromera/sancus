/*
 * LoginScreen.cpp
 *
 *  Created on: 14/10/2011
 *      Author: juanmromera
 */

#include "LoginScreen.h"
#include "../core/VoteApp.h"
#include "../utils/OstreamUtils.h"

LoginScreen::LoginScreen() {
}

void  LoginScreen::draw()
{
	system("clear");
	//VoterFile* voteFile = VoterFile::getInstance();
	VoteApp* app = VoteApp::getInstance();
	uint32_t dni;
	std::string clave;
	char respuesta;

	std::cout<<"/////////////////////////////////////////"<<std::endl;
	std::cout<<"/////////////////////////////////////////"<<std::endl;
	std::cout<<"/////////////////////////////////////////"<<std::endl;
	std::cout<<"           VOTO ELECTRONICO              "<<std::endl;
	std::cout<<"/////////////////////////////////////////"<<std::endl;
	std::cout<<"/////////////////////////////////////////"<<std::endl;
	std::cout<<"/////////////////////////////////////////"<<std::endl;
	std::cout<<""<<std::endl;
	std::cout<<""<<std::endl;
	std::cout<<""<<std::endl;


	bool incorrectUser = true;
	bool userAdmin = false;
	std::cout<<"Porfavor ingrese su usuario (DNI) y presione enter"<<std::endl;
	std::cout<<""<<std::endl;

	while(incorrectUser)
	{
		dni = IstreamUtils::getUint();
		std::cout<<""<<std::endl;

		if(true)//if(!validateDNI(dni))
		{
			if(true)//if(validateAdmin(dni))
			{
				incorrectUser = false;
				userAdmin = true;
			}
			else
			{
				std::cout<<""<<std::endl;
				std::cout<<"Usuario Inexistente ingrese nuevamente"<<std::endl;
			}

		}
		else
		{
			incorrectUser = false;
		}
	}
	bool incorrectPassword = true;

	std::cout<<""<<std::endl;

	std::cout<<""<<std::endl;

	while(incorrectPassword)
	{
		std::cout<<"Porfavor ingrese contraseña y presione enter"<<std::endl;
		std::cout<<""<<std::endl;
		clave =  IstreamUtils::getString();
		std::cout<<""<<std::endl;
		if(false)
		/*if( (!userAdmin && clave.compare(record->getPassword()) != 0)
				|| (userAdmin && clave.compare(adminUser.getPassword != 0)) )*/
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
		{	//if(!userAdmin)
			//app->setUserLogin(user);
			incorrectPassword = false;
		}
	}
	if(userAdmin)
		app->setActualScreen(ADM_SCREEN);
	else
		app->setActualScreen(ELECTION_SCREEN);
	return;
}
/*
bool LoginScreen::validateDNI(uint32_t dni)
{
	VoterFile* voteFile = VoterFile::getInstance();
	user = new VoterRecord(dni);
	try{
		*user  = voteFile.search(*record);
		if(user->getDNI() == dni)
			return true;
		else
			return false;
	}catch(FileSearchException e)
	{
		return false;
	}
}
*/
/*
bool LoginScreen::validateAdmin(uint32_t user)
{
	AdminFile* adminFile = AdminFile::getInstance();
	AdminRecord::Key adminKey = AdminRecord(user);
	try{
		adminUser = adminFile.search(adminKey);
	}catch(FileSearchException e)
	{
		return false;
	}
	return true;
}
*/

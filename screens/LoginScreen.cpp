/*
 * LoginScreen.cpp
 *
 *  Created on: 14/10/2011
 *      Author: juanmromera
 */

#include "LoginScreen.h"
#include "../core/VoteApp.h"

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
	std::cout<<"Porfavor ingrese su usuario (DNI) y presione enter"<<std::endl;
	std::cout<<""<<std::endl;

	while(incorrectUser)
	{
		dni = IstreamUtils::getUint();
		std::cout<<""<<std::endl;

		if(false)//if(!validateDNI(dni))
		{
			std::cout<<""<<std::endl;
			std::cout<<"Usuario Inexistente ingrese nuevamente"<<std::endl;
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
		if(true)//if(clave.compare(record->getPassword()) != 0)
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
		{
			//app->setUserLogin(user);
			incorrectPassword = false;
		}
	}
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

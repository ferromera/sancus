/*
 * CryptographyScreen.cpp
 *
 *  Created on: 03/12/2011
 *      Author: fernando
 */

#include "CryptographyScreen.h"
#include "../core/VoteApp.h"

CryptographyScreen::CryptographyScreen() {
}
void CryptographyScreen::draw(){
	int opcion;
	VoteApp * app = VoteApp::getInstance();
	system("clear");
	std::cout << "PANTALLA DE CRIPTOGRAFIA" << std::endl;
	std::cout << "Elija la opcion que prefiera y apriete ENTER" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "1- Encriptar" << std::endl;
	std::cout << "2- Desencriptar" << std::endl;
	std::cout << "3- Atacar" << std::endl;
	std::cout << "4- Volver a pantalla anterior" << std::endl;
	std::cout << "" << std::endl;
	while (true) {
		opcion = IstreamUtils::getUint();
		switch (opcion) {
		case 1:
			app->setActualScreen(ENC_SCREEN);
			return;
		case 2:
			app->setActualScreen(DESC_SCREEN);
			return;
		case 3:
			app->setActualScreen(ATTACK_SCREEN);
			return;
		case 4:
			app->setActualScreen(ADM_SCREEN);
			return;
		default:
			std::cout << "Opcion Incorrecta, elija nuevamente" << std::endl;
		}
	}
}

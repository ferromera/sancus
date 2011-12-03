/*
 * EncryptScreen.cpp
 *
 *  Created on: 03/12/2011
 *      Author: fernando
 */

#include "EncryptScreen.h"
#include "../core/VoteApp.h"
#include "../screens/ResultScreen.h"
#include "../estructuras/RsaSecurity.h"
#include "../estructuras/VigenereSecurity.h"

EncryptScreen::EncryptScreen() {
}
void EncryptScreen::draw() {
	int opcion;
	VoteApp * app = VoteApp::getInstance();
	system("clear");
	std::cout << "PANTALLA DE ENCRIPTACION DE ARCHIVOS" << std::endl;
	std::cout << "Elija el metodo que desea usar para encriptar y apriete ENTER" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "1- Encriptar con Vigenere" << std::endl;
	std::cout << "2- Encriptar con RSA" << std::endl;
	std::cout << "3- Volver a pantalla anterior" << std::endl;
	std::cout << "" << std::endl;
	while (true) {
		opcion = IstreamUtils::getUint();
		switch (opcion) {
		case 1:
			encryptByVigenere();
			return;
		case 2:
			encryptByRsa();
			return;
		case 3:
			app->setActualScreen(CRYPT_SCREEN);
			return;
		default:
			std::cout << "Opcion Incorrecta, elija nuevamente" << std::endl;
		}
	}

}
void EncryptScreen::encryptByVigenere() {
	std::cout << "Seleccione la clave a usar:" << std::endl;
	std::cout << "1- Clave del sistema" << std::endl;
	std::cout << "2- Clave personalizada" << std::endl;
	int opcion;
	while (true) {
		opcion = IstreamUtils::getUint();
		switch (opcion) {
		case 1:
			encryptByVigenereWithSystemKey();
			return;
		case 2:
			encryptByVigenereWithCustomKey();
			return;
		default:
			std::cout << "Opcion Incorrecta, elija nuevamente" << std::endl;
		}
	}

}
void EncryptScreen::encryptByVigenereWithSystemKey() {
	File *readFile = NULL;
	string path;
	VigenereSecurity * vSec = new VigenereSecurity(VIGENEREKEYSIZE);
	bool repeat = false;
	while (true) {
		repeat=false;
		std::cout << "Ingrese la ruta del archivo a encriptar: " << std::endl;
		path = IstreamUtils::getString();
		try {
			readFile = new File(path, File::BIN);
		} catch (OpenFileException e) {
			std::cout << "Ruta Invalida, desea ingresar otra ruta (S/N)" << std::endl;
			switch (IstreamUtils::getChar()) {
			case 'S':repeat=true;
				break;
			case 'N':
				delete vSec;
				return;
			default:
				std::cout << "Opcion Incorrecta, elija nuevamente" << std::endl;
				continue;
			}
		}
		if(repeat)
			continue;
		readFile->seek(0,File::END);
		unsigned int size=readFile->tell();
		readFile->seek(0,File::BEG);
		unsigned char * readBuffer=new unsigned char [size];
		readFile->read(readBuffer,size);
		std::string encryptedPath = path;
		std::string::iterator it = encryptedPath.end();
		for (; (*it) != '.' && (*it) != '/' && it!=encryptedPath.begin(); it--)
				;
			if (*it == '.')
				encryptedPath.erase(it, encryptedPath.end());
			encryptedPath.append("_encrypted.bin");
		File * outputFile=new File(encryptedPath,File::NEW|File::BIN,vSec);
		outputFile->write(readBuffer,size);
		delete vSec;
		delete readBuffer;
		delete outputFile;
		delete readFile;
		std::cout << "Se encripto el archivo en: "<<encryptedPath << std::endl;
					std::cout << "Presione enter para volver a la pantalla de encriptacion."<< std::endl;
					IstreamUtils::getString();
		return;
	}
}
void EncryptScreen::encryptByVigenereWithCustomKey(){
	File *readFile = NULL;
		string path;
		VigenereSecurity * vSec = new VigenereSecurity(VIGENEREKEYSIZE);
		bool repeat = false;
		std::cout << "Ingrese la clave: " << std::endl;
		string key = IstreamUtils::getString();
		vSec->setKey(key);

		while (true) {
			repeat=false;
			std::cout << "Ingrese la ruta del archivo a encriptar: " << std::endl;
			path = IstreamUtils::getString();
			try {
				readFile = new File(path, File::BIN);
			} catch (OpenFileException e) {
				std::cout << "Ruta Invalida, desea ingresar otra ruta (S/N)" << std::endl;
				switch (IstreamUtils::getChar()) {
				case 'S':repeat=true;
					break;
				case 'N':
					delete vSec;
					return;
				default:
					std::cout << "Opcion Incorrecta, elija nuevamente" << std::endl;
					continue;
				}
			}
			if(repeat)
				continue;
			readFile->seek(0,File::END);
			unsigned int size=readFile->tell();
			readFile->seek(0,File::BEG);
			unsigned char * readBuffer=new unsigned char [size];
			readFile->read(readBuffer,size);
			std::string encryptedPath = path;
			std::string::iterator it = encryptedPath.end();
			for (; (*it) != '.' && (*it) != '/' && it!=encryptedPath.begin(); it--)
					;
				if (*it == '.')
					encryptedPath.erase(it, encryptedPath.end());
				encryptedPath.append("_encrypted.bin");
			File * outputFile=new File(encryptedPath,File::NEW|File::BIN,vSec);
			outputFile->write(readBuffer,size);
			delete vSec;
			delete readBuffer;
			delete outputFile;
			delete readFile;
			std::cout << "Se encripto el archivo en: "<<encryptedPath << std::endl;
						std::cout << "Presione enter para volver a la pantalla de encriptacion."<< std::endl;
						IstreamUtils::getString();
			return;
		}
}

void EncryptScreen::encryptByRsa() {
	File *readFile = NULL;
		string path;
		RsaSecurity * rsaSec = new RsaSecurity(10);
		bool repeat = false;
		while (true) {
			repeat=false;
			std::cout << "Ingrese la ruta del archivo a encriptar: " << std::endl;
			path = IstreamUtils::getString();
			try {
				readFile = new File(path, File::BIN);
			} catch (OpenFileException e) {
				std::cout << "Ruta Invalida, desea ingresar otra ruta (S/N)" << std::endl;
				switch (IstreamUtils::getChar()) {
				case 'S':repeat=true;
					break;
				case 'N':
					delete rsaSec;
					return;
				default:
					std::cout << "Opcion Incorrecta, elija nuevamente" << std::endl;
					continue;
				}
			}
			if(repeat)
				continue;
			readFile->seek(0,File::END);
			unsigned int size=readFile->tell();
			readFile->seek(0,File::BEG);
			unsigned char * readBuffer=new unsigned char [size];
			readFile->read(readBuffer,size);
			std::string encryptedPath = path;
			std::string::iterator it = encryptedPath.end();
			for (; (*it) != '.' && (*it) != '/' && it!=encryptedPath.begin(); it--)
					;
				if (*it == '.')
					encryptedPath.erase(it, encryptedPath.end());
				encryptedPath.append("_encrypted.bin");
			File * outputFile=new File(encryptedPath,File::NEW|File::BIN,rsaSec);
			outputFile->write(readBuffer,size);
			delete rsaSec;
			delete readBuffer;
			delete outputFile;
			delete readFile;
			std::cout << "Se encripto el archivo en: "<<encryptedPath << std::endl;
			std::cout << "Presione enter para volver a la pantalla de encriptacion."<< std::endl;
			IstreamUtils::getString();
			return;
		}
}

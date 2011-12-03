/*
 * DecryptScreen.cpp
 *
 *  Created on: 03/12/2011
 *      Author: fernando
 */

#include "DecryptScreen.h"
#include "../core/VoteApp.h"
#include "../estructuras/RsaSecurity.h"
#include "../estructuras/VigenereSecurity.h"
#include "ResultScreen.h"

DecryptScreen::DecryptScreen() {
}
void DecryptScreen::draw() {
	int opcion;
	VoteApp * app = VoteApp::getInstance();
	system("clear");
	std::cout << "PANTALLA DE DESENCRIPTACION DE ARCHIVOS" << std::endl;
	std::cout << "Elija el metodo que desea usar para desencriptar y apriete ENTER" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "1- Desencriptar con Vigenere" << std::endl;
	std::cout << "2- Desencriptar con RSA" << std::endl;
	std::cout << "3- Volver a pantalla anterior" << std::endl;
	std::cout << "" << std::endl;
	while (true) {
		opcion = IstreamUtils::getUint();
		switch (opcion) {
		case 1:
			decryptByVigenere();
			return;
		case 2:
			decryptByRsa();
			return;
		case 3:
			app->setActualScreen(CRYPT_SCREEN);
			return;
		default:
			std::cout << "Opcion Incorrecta, elija nuevamente" << std::endl;
		}
	}

}
void DecryptScreen::decryptByVigenere() {
	std::cout << "Seleccione la clave a usar:" << std::endl;
	std::cout << "1- Clave del sistema" << std::endl;
	std::cout << "2- Clave personalizada" << std::endl;
	int opcion;
	while (true) {
		opcion = IstreamUtils::getUint();
		switch (opcion) {
		case 1:
			decryptByVigenereWithSystemKey();
			return;
		case 2:
			decryptByVigenereWithCustomKey();
			return;
		default:
			std::cout << "Opcion Incorrecta, elija nuevamente" << std::endl;
		}
	}

	IstreamUtils::getString();
}
void DecryptScreen::decryptByVigenereWithSystemKey() {
	File *readFile = NULL;
	string path;
	VigenereSecurity * vSec = new VigenereSecurity(VIGENEREKEYSIZE);
	bool repeat = false;
	while (true) {
		repeat=false;
		std::cout << "Ingrese la ruta del archivo a desencriptar: " << std::endl;
		path = IstreamUtils::getString();
		try {
			readFile = new File(path, File::BIN, vSec);
		} catch (OpenFileException e) {
			std::cout << "Ruta Invalida, desea ingresar otra ruta (S/N)" << std::endl;
			switch (IstreamUtils::getChar()) {
			case 'S':
				repeat = true;
				break;
			case 'N':
				delete vSec;
				return;
			default:
				std::cout << "Opcion Incorrecta, elija nuevamente" << std::endl;
				continue;
			}
		}
		if (repeat)
			continue;
		readFile->seek(0, File::END);
		unsigned int size = readFile->tell();
		readFile->seek(0, File::BEG);
		unsigned char * readBuffer = new unsigned char[size];
		readFile->read(readBuffer, size);
		std::string decryptedPath = path;
		std::string::iterator it = decryptedPath.end();
		for (; (*it) != '.' && (*it) != '/' && it != decryptedPath.begin(); it--)
			;
		if (*it == '.')
			decryptedPath.erase(it, decryptedPath.end());
		decryptedPath.append("_decrypted.bin");
		File * outputFile = new File(decryptedPath, File::NEW | File::BIN);
		outputFile->write(readBuffer, size);
		delete vSec;
		delete readBuffer;
		delete outputFile;
		delete readFile;
		std::cout << "Se desencripto el archivo en: " << decryptedPath << std::endl;
		std::cout << "Presione enter para volver a la pantalla de desencriptacion." << std::endl;
		IstreamUtils::getString();
		return;
	}
}
void DecryptScreen::decryptByVigenereWithCustomKey() {
	File *readFile = NULL;
	string path;
	VigenereSecurity * vSec = new VigenereSecurity(VIGENEREKEYSIZE);
	bool repeat = false;
	std::cout << "Ingrese la clave: " << std::endl;
	string key = IstreamUtils::getString();
	vSec->setKey(key);

	while (true) {
		repeat=false;
		std::cout << "Ingrese la ruta del archivo a desencriptar: " << std::endl;
		path = IstreamUtils::getString();
		try {
			readFile = new File(path, File::BIN, vSec);
		} catch (OpenFileException e) {
			std::cout << "Ruta Invalida, desea ingresar otra ruta (S/N)" << std::endl;
			switch (IstreamUtils::getChar()) {
			case 'S':
				repeat = true;
				break;
			case 'N':
				delete vSec;
				return;
			default:
				std::cout << "Opcion Incorrecta, elija nuevamente" << std::endl;
				continue;
			}
		}
		if (repeat)
			continue;
		readFile->seek(0, File::END);
		unsigned int size = readFile->tell();
		readFile->seek(0, File::BEG);
		unsigned char * readBuffer = new unsigned char[size];
		readFile->read(readBuffer, size);
		std::string decryptedPath = path;
		std::string::iterator it = decryptedPath.end();
		for (; (*it) != '.' && (*it) != '/' && it != decryptedPath.begin(); it--)
			;
		if (*it == '.')
			decryptedPath.erase(it, decryptedPath.end());
		decryptedPath.append("_decrypted.bin");
		File * outputFile = new File(decryptedPath, File::NEW | File::BIN);
		outputFile->write(readBuffer, size);
		delete vSec;
		delete readBuffer;
		delete outputFile;
		delete readFile;
		std::cout << "Se desencripto el archivo en: " << decryptedPath << std::endl;
		std::cout << "Presione enter para volver a la pantalla de desencriptacion." << std::endl;
		IstreamUtils::getString();
		return;
	}
}
void DecryptScreen::decryptByRsa() {
	File *readFile = NULL;
		string path;
		RsaSecurity * rsaSec = new RsaSecurity(10);
		bool repeat = false;
		while (true) {
			repeat=false;
			std::cout << "Ingrese la ruta del archivo a desencriptar: " << std::endl;
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

			std::string decryptedPath = path;
			std::string::iterator it = decryptedPath.end();
			for (; (*it) != '.' && (*it) != '/' && it!=decryptedPath.begin(); it--)
					;
				if (*it == '.')
					decryptedPath.erase(it, decryptedPath.end());
				decryptedPath.append("_decrypted.bin");
			File * outputFile=new File(decryptedPath,File::NEW|File::BIN);
			unsigned char* decryptedBuffer=rsaSec->decrypt(readBuffer,rsaSec->getPlainBytes(size));
			outputFile->write(decryptedBuffer,rsaSec->getPlainBytes(size));
			delete decryptedBuffer;
			delete rsaSec;
			delete readBuffer;
			delete outputFile;
			delete readFile;
			std::cout << "Se desencripto el archivo en: "<<decryptedPath << std::endl;
			std::cout << "Presione enter para volver a la pantalla de desencriptacion."<< std::endl;
			IstreamUtils::getString();
			return;
		}
}


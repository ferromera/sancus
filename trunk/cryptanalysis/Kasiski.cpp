/*
 * Kasiski.cpp
 *
 *  Created on: 18/11/2011
 *      Author: fernando
 */

#include "Kasiski.h"
#include <string>
#include <list>
#include <iostream>
#include "../estructuras/File.h"
#include "../utils/IstreamUtils.h"
#define DISTRICT_REPORT_HEADER "Resultados del distrito: "
#define LIST_REPORT_HEADER     "Resultados de la lista: "
#define ELECTION_REPORT_HEADER "Resultados de la eleccion: "

Kasiski::Kasiski() {
	// TODO Auto-generated constructor stub

}

void Kasiski::attackElection(std::string encryptedFilePath) {
	File file(encryptedFilePath, File::BIN);
	file.seek(0, File::END);
	unsigned int fileSize = file.tell();
	file.seek(0, File::BEG);
	unsigned int bufferSize = KASISKI_BUFFER_MAX;
	if (fileSize < bufferSize)
		bufferSize = fileSize;
	unsigned char * buffer = new unsigned char[bufferSize];
	file.read(buffer, bufferSize);

	unsigned int keyLength = calculateKeyLength((char*)buffer, bufferSize);
	unsigned char *key = getKey(buffer, keyLength, ELECTION_REPORT_HEADER);
	unsigned char *decryptedBuffer = decrypt(key, keyLength, buffer, bufferSize);
	writeBreakedFile(encryptedFilePath,decryptedBuffer, bufferSize);
	delete buffer;
	delete key;
	delete decryptedBuffer;

}


void Kasiski::attackDistrict(std::string encryptedFilePath) {
	File file(encryptedFilePath, File::BIN);
	file.seek(0, File::END);
	unsigned int fileSize = file.tell();
	file.seek(0, File::BEG);
	unsigned int bufferSize = KASISKI_BUFFER_MAX;
	if (fileSize < bufferSize)
		bufferSize = fileSize;
	 unsigned char * buffer = new  unsigned char[bufferSize];
	file.read(buffer, bufferSize);

	unsigned int keyLength = calculateKeyLength((char*)buffer, bufferSize);
	unsigned char *key = getKey(buffer, keyLength, DISTRICT_REPORT_HEADER);
	unsigned char *decryptedBuffer = decrypt(key, keyLength, buffer, bufferSize);
	writeBreakedFile(encryptedFilePath,decryptedBuffer, bufferSize);
	delete buffer;
	delete key;
	delete decryptedBuffer;

}
void Kasiski::attackList(std::string encryptedFilePath) {
	File file(encryptedFilePath, File::BIN);
	file.seek(0, File::END);
	unsigned int fileSize = file.tell();
	file.seek(0, File::BEG);
	unsigned int bufferSize = KASISKI_BUFFER_MAX;
	if (fileSize < bufferSize)
		bufferSize = fileSize;
	unsigned char * buffer = new unsigned char[bufferSize];
	file.read(buffer, bufferSize);

	unsigned int keyLength = calculateKeyLength((char*)buffer, bufferSize);
	unsigned char *key = getKey(buffer, keyLength, LIST_REPORT_HEADER);
	unsigned char *decryptedBuffer = decrypt(key, keyLength, buffer, bufferSize);
	writeBreakedFile(encryptedFilePath,decryptedBuffer,bufferSize);
	delete buffer;
	delete key;
	delete decryptedBuffer;

}

unsigned char * Kasiski::decrypt(unsigned char *key,unsigned int keyLength,unsigned char* buffer,unsigned int bufferSize){
	unsigned char * decryptedBuffer=new unsigned char[bufferSize];
	unsigned int i,j;
	for(i=0,j=0;i<bufferSize;i++,j=(j+1)%keyLength)
		decryptedBuffer[i]=buffer[i]-key[j];
	return decryptedBuffer;
}
void Kasiski::writeBreakedFile(std::string &encryptedFilePath,unsigned char* decryptedBuffer, unsigned int bufferSize) {
	std::string decryptedFilePath = encryptedFilePath;
	std::string::iterator it = decryptedFilePath.end();
	for (; (*it) != '.' && (*it) != '/' && it!=decryptedFilePath.begin(); it--)
		;
	if (*it == '.')
		decryptedFilePath.erase(it, decryptedFilePath.end());
	decryptedFilePath.append("_breaked.txt");
	File breakedFile(decryptedFilePath, File::NEW | File::BIN);
	breakedFile.write(decryptedBuffer, bufferSize);
	std::cout << "El archivo fue desencriptado en : " << decryptedFilePath << std::endl;
	IstreamUtils::getString();
}
unsigned char * Kasiski::getKey(unsigned char * buffer, unsigned int keyLength, const std::string & header) {
	unsigned char *key = new unsigned char[keyLength];
	if (keyLength > header.size()) {
		std::cout << "El tamanio de la clave es muy grande para poder romper el archivo" << std::endl;
		IstreamUtils::getString();
		delete key;
		return NULL;
	}
	for (unsigned int i = 0; i < keyLength; i++) {
		key[i] = buffer[i] - header[i];
	}
	return key;
}

unsigned int Kasiski::calculateKeyLength(char * buffer, unsigned int size) {
	char * str1 = buffer;
	char * str2 = buffer + KASISKI_STR_SIZE;
	std::list<unsigned int> distances;
	unsigned int initialPos = 0;
	for (; initialPos + 2 * KASISKI_STR_SIZE -1< size; initialPos++, str1++) {
		str2 = buffer + initialPos + KASISKI_STR_SIZE;
		for (unsigned int i = initialPos + KASISKI_STR_SIZE; i + KASISKI_STR_SIZE-1 < size; i++, str2++) {
			if (areEqual(str1, str2))
				distances.push_back(i - initialPos);
		}
	}
	std::list<unsigned int>::iterator it = distances.begin();
	unsigned int *factorCount = new unsigned int[KASISKI_BUFFER_MAX];
	for (unsigned int i = 0; i < KASISKI_BUFFER_MAX; i++)
		factorCount[i] = 0;
	for (; it != distances.end(); it++) {
		addFactors(*it, factorCount);
	}
	unsigned int maxCount = 0;
	unsigned int keyLength = 0;
	for (unsigned int i = 0; i < KASISKI_BUFFER_MAX; i++) {
		if (factorCount[i] > maxCount) {
			maxCount = factorCount[i];
			keyLength = i;
		}
	}
	delete factorCount;
	return keyLength;
}

bool Kasiski::areEqual(char * str1, char*str2) {
	bool equal = true;
	for (unsigned int i = 0; i < KASISKI_STR_SIZE; i++)
		if (str1[i] != str2[i])
			equal = false;
	return equal;
}
void Kasiski::addFactors(unsigned int distance, unsigned int * factorCount) {
	unsigned int greaterDivisor;
	for (unsigned int i = KASISKI_STR_SIZE; i < distance; i++) {
		if ((distance % i) == 0) {
			greaterDivisor = distance / i;
			if (greaterDivisor < i)
				break;
			factorCount[i]++;
			factorCount[greaterDivisor]++;
		}
	}
}
Kasiski::~Kasiski() {
	// TODO Auto-generated destructor stub
}

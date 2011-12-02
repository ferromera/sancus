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

Kasiski::Kasiski() {
	// TODO Auto-generated constructor stub

}

void Kasiski::attack(std::string encryptedFilePath) {
	File file(encryptedFilePath, File::BIN);
	file.seek(0, File::END);
	int fileSize = file.tell();
	file.seek(0, File::BEG);
	int bufferSize = KASISKI_BUFFER_MAX;
	if (fileSize < bufferSize)
		bufferSize = fileSize;
	char * buffer = new char[bufferSize];
	file.read(buffer, bufferSize);
	int keyLength = calculateKeyLength(buffer, bufferSize);
	std::cout<<keyLength<<std::endl;
}
unsigned int Kasiski::calculateKeyLength(char * buffer, unsigned int size) {
	char * str1 = buffer;
	char * str2 = buffer + KASISKI_STR_SIZE;
	std::list<unsigned int> distances;
	unsigned int initialPos = 0;
	for (; initialPos + 2 * KASISKI_STR_SIZE < size; initialPos++, str1++) {
		str2=buffer+initialPos + KASISKI_STR_SIZE;
		for (unsigned int i = initialPos + KASISKI_STR_SIZE; i + KASISKI_STR_SIZE < size; i++, str2++) {
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

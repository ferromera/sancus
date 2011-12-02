/*
 * Kasiski.h
 *
 *  Created on: 18/11/2011
 *      Author: fernando
 */

#ifndef KASISKI_H_
#define KASISKI_H_
#include <string>

#define KASISKI_BUFFER_MAX 1048576
#define KASISKI_STR_SIZE 5

class Kasiski {
	static void addFactors(unsigned int,unsigned int*);
	static unsigned int calculateKeyLength(char *,unsigned int);
	static bool areEqual(char * ,char*);
	static void writeBreakedFile(std:: string &encryptedFilePath,unsigned char* decryptedBuffer, unsigned int bufferSize);
	static unsigned char * getKey(unsigned char * buffer, unsigned int keyLength, const std::string & header);
	static unsigned char * decrypt(unsigned char *key,unsigned int keyLength,unsigned char* buffer,unsigned int bufferSize);
public:
	Kasiski();
	static void attackDistrict(std::string encryptedFilePath);
	static void attackList(std::string encryptedFilePath);
	static void attackElection(std::string encryptedFilePath);
	virtual ~Kasiski();
};

#endif /* KASISKI_H_ */

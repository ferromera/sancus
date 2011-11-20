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
#define KASISKI_STR_SIZE 3

class Kasiski {
	static void addFactors(unsigned int,unsigned int*);
	static unsigned int calculateKeyLength(char *,unsigned int);
	static bool areEqual(char * ,char*);
public:
	Kasiski();
	static void attack(std::string encryptedFilePath);
	virtual ~Kasiski();
};

#endif /* KASISKI_H_ */

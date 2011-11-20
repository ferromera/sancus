/*
 * VigenereSecurity.h
 *
 *  Created on: Nov 15, 2011
 *      Author: alfredo
 */

#ifndef VIGENERE_SECURITY_H_
#define VIGENERE_SECURITY_H_

#include "SecurityStrategy.h"

class VigenereSecurity : public SecurityStrategy {
private:
	char * key;

	void generateAndStoreRandomKey(unsigned keySize);

public:
	VigenereSecurity(unsigned int  keySize): SecurityStrategy(keySize){
		key = new char[keySize];

		generateAndStoreRandomKey(keySize);
	}

	void encrypt(void * buffer, size_t bytes){
		unsigned int j=0;

		for(unsigned int i = 0; i< bytes; i++){
			if(j>this->keySize){
				j=0;
			}

			j++;

			buffer[i] = (key[j] + buffer[i]  % 256);
		}
	}
	
	void decrypt(void * buffer, size_t bytes){
	
	}
};
#endif /* VIGENERE_SECURITY_H_ */

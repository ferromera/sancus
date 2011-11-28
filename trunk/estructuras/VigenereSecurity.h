/*
 * VigenereSecurity.h
 *
 *  Created on: Nov 15, 2011
 *      Author: alfredo
 */

#ifndef VIGENERE_SECURITY_H_
#define VIGENERE_SECURITY_H_
#define SYMBOLS 256

#include "SecurityStrategy.h"

class VigenereSecurity : public SecurityStrategy {
private:
	unsigned char * key;

	void generateAndStoreRandomKey(unsigned int keySize)
	{
		this->key = (unsigned char*)"AAAA";
//		int r;
//		//for (int i = 0; i < keySize; i++){
//			key[i] = MathUtils::randomNumber(0,256);
//		}

	}

public:
	VigenereSecurity(unsigned int  keySize): SecurityStrategy(keySize){
		key = new unsigned char[keySize];
		generateAndStoreRandomKey(keySize);
	}

	void encrypt(unsigned char * &buffer, size_t bytes){
		unsigned int j=0;
		unsigned char* message = (unsigned char*)buffer;

		for(unsigned int i = 0; i< bytes; i++){
			if(j>this->keySize){
				j=0;
			}

			message[i] = ((key[j] + message[i])  % SYMBOLS);
			j++;
		}
	}
	
	void decrypt(unsigned char * &buffer, size_t bytes){
		unsigned int j=0;
		unsigned char* message = (unsigned char*)buffer;

		for(unsigned int i = 0; i< bytes; i++){
			if(j>this->keySize){
				j=0;
			}
			if (message[i] - key[j] < 0){
				message[i] = (message[i] - key[j]) + SYMBOLS;
			}
			else {
				message[i] = message[i] - key[j];
			}
			j++;

		}
	}

	~VigenereSecurity()
	{
		delete this->key;
	}
};
#endif /* VIGENERE_SECURITY_H_ */

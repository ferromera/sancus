/*
 * VigenereSecurity.h
 *
 *  Created on: Nov 15, 2011
 *      Author: alfredo
 */

#ifndef VIGENERE_SECURITY_H_
#define VIGENERE_SECURITY_H_
#define SYMBOLS 256

#include <cstdio>
#include <cstdlib>

#define VIGENERE_SECURITY_KEYPATH	"VigenereKey.bin"

#include "SecurityStrategy.h"
#include "MathUtils.h"
#include "File.h"

class VigenereSecurity : public SecurityStrategy {
private:
	unsigned char * key;

	void generateAndStoreRandomKey(unsigned int keySize)
	{
		File* keyFile;
		try {
			keyFile = new File(VIGENERE_SECURITY_KEYPATH, File::BIN | File::IO);
			keyFile->read(this->key, keySize);
			delete keyFile;
		}
		catch (OpenFileException) {
			keyFile = new File(VIGENERE_SECURITY_KEYPATH, File::BIN | File::NEW);
			for (unsigned int i = 0; i < keySize; i++){
				this->key[i] = MathUtils::randomNumber(0,256);
			}
			keyFile->write(this->key, keySize);
			delete keyFile;
		}
	}

public:
	VigenereSecurity(unsigned int  keySize): SecurityStrategy(keySize){
		key = new unsigned char[keySize];
		generateAndStoreRandomKey(keySize);
	}

	unsigned char * encrypt(unsigned char * buffer, size_t bytes){
		unsigned int j=0;
		unsigned char* message = (unsigned char*)buffer;

		for(unsigned int i = 0; i< bytes; i++){
			if(j>this->keySize){
				j=0;
			}

			message[i] = ((key[j] + message[i])  % SYMBOLS);
			j++;
		}
		return buffer;
	}
	
	unsigned char * decrypt(unsigned char * buffer, size_t bytes){
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
		return buffer;
	}

	~VigenereSecurity()
	{
		delete this->key;
	}
};
#endif /* VIGENERE_SECURITY_H_ */

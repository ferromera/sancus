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
				this->key[i] = MathUtils::randomNumber(0,255);
			}
			keyFile->write(this->key, keySize);
			delete keyFile;
		}
	}

public:
	VigenereSecurity(unsigned int  keySize): SecurityStrategy(keySize){
		srand((unsigned) time(0));
		key = new unsigned char[keySize];
		generateAndStoreRandomKey(keySize);

	}
	void setKey(string newKey){
		delete key;
		key=new unsigned char[newKey.size()];
		for(unsigned int i=0;i<newKey.size();i++)
			key[i]=newKey[i];

	}

	unsigned char * encrypt(unsigned char * buffer, size_t bytes){
		unsigned int j=0;
		unsigned char* message = (unsigned char*)buffer;
		unsigned char* encryptedMessage= new unsigned char[bytes];

		for(unsigned int i = 0; i< bytes; i++){
			if(j==this->keySize){
				j=0;
			}

			encryptedMessage[i] = ((key[j] + message[i])  % SYMBOLS);
			j++;
		}
		return encryptedMessage;
	}
	
	unsigned char * decrypt(unsigned char * buffer, size_t bytes){
		unsigned int j=0;
		unsigned char* message = (unsigned char*)buffer;
		unsigned char* decryptedMessage= new unsigned char[bytes];

		for(unsigned int i = 0; i< bytes; i++){
			if(j==this->keySize){
				j=0;
			}

			decryptedMessage[i] = message[i] - key[j];

			j++;

		}
		return decryptedMessage;
	}

	~VigenereSecurity()
	{
		delete this->key;
	}
};
#endif /* VIGENERE_SECURITY_H_ */

/*
 * VigenereSecurity.h
 *
 *  Created on: Nov 15, 2011
 *      Author: alfredo
 */

#ifndef VIGENERE_SECURITY_H_
#define VIGENERE_SECURITY_H_

class VigenereSecurity : public SecurityStrategy {

public:
	VigenereSecurity(unsigned int  keySize): SecurityStrategy(keySize){

	}

	void encrypt(void * buffer, size_t bytes){

	}
	
	void decrypt(void * buffer, size_t bytes){
	
	}
};
#endif /* VIGENERE_SECURITY_H_ */

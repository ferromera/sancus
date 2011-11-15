/*
 * RsaSecurity.h
 *
 *  Created on: Nov 15, 2011
 *      Author: alfredo
 */

#ifndef RSA_SECURITY_H_
#define RSA_SECURITY_H_

#include "SecurityStrategy.h"

class RsaSecurity : public SecurityStrategy {

public:
	RsaSecurity(unsigned int  keySize): SecurityStrategy(keySize){
        
	}

	void encrypt(void * buffer, size_t bytes){
        
	}
	
	void decrypt(void * buffer, size_t bytes){
	
	}
};
#endif /* RSA_SECURITY_H_ */

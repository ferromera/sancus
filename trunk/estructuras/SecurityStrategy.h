/*
 * SecurityStrategy
 *
 *  Created on: Nov 15, 2011
 *      Author: alfredo
 */

#ifndef SECURITY_STRATEGY_H_
#define SECURITY_STRATEGY_H_

#include <cstdlib>
using namespace std;

class SecurityStrategy {
protected:
	unsigned int keySize;
public:
	SecurityStrategy(unsigned int keySize) {
		this->keySize = keySize;
	}

	virtual unsigned char *  encrypt(unsigned char * buffer, size_t bytes) = 0;
	virtual unsigned char *  decrypt(unsigned char * buffer, size_t bytes) = 0;
	virtual unsigned int getEncryptedBytes(size_t bytes){
		return bytes;
	}
};

#endif /* SECURITY_STRATEGY_H_ */


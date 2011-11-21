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

	virtual void encrypt(void * buffer, size_t bytes) = 0;
	virtual void decrypt(void * buffer, size_t bytes) = 0;
};

#endif /* SECURITY_STRATEGY_H_ */


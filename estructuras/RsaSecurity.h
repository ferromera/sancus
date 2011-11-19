/*
 * RsaSecurity.h
 *
 *  Created on: Nov 15, 2011
 *      Author: alfredo
 */

#ifndef RSA_SECURITY_H_
#define RSA_SECURITY_H_

#include "SecurityStrategy.h"
#include "MathUtils.h"

class RsaSecurity: public SecurityStrategy {
private:
	unsigned int p;
	unsigned int q;
	unsigned int x;

public:
	RsaSecurity(unsigned int keySize) : SecurityStrategy(keySize) {
		while (MathUtils::isPrime(p)) {
			p = MathUtils::generateRandomIntegersOf(this->keySize);
		}

		while (MathUtils::isPrime(q) || q == p) {
			q = MathUtils::generateRandomIntegersOf(this->keySize);
		}

		this->x = p * q;
	}

	void encrypt(void * buffer, size_t bytes) {

	}

	void decrypt(void * buffer, size_t bytes) {

	}
};
#endif /* RSA_SECURITY_H_ */

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

struct RsaPublicKey{
	unsigned int e;
	unsigned int n;
};

struct RsaPrivateKey{
	unsigned int d;
	unsigned int n;
};

class RsaSecurity: public SecurityStrategy {
private:
	RsaPublicKey publicKey;
	RsaPrivateKey privateKey;

public:
	RsaSecurity(unsigned int keySize) : SecurityStrategy(keySize) {
		unsigned int p;
		unsigned int q;
		unsigned int d;
		unsigned int x;
		unsigned int e;
		unsigned int n;

		//obtener p y q
		while (MathUtils::isPrime(p)) {
			p = MathUtils::generateRandomIntegersOf(this->keySize);
		}

		while (MathUtils::isPrime(q) || q == p) {
			q = MathUtils::generateRandomIntegersOf(this->keySize);
		}

		//calcular n
		n = p * q;

		//calcular fi(n)
		x = (p-1) * (q-1);

		//obtener d
		d = 0;
		while(d < p && d < q){
			d = rand();

			if(!MathUtils::isPrime(d)){
				MathUtils::nextPrime(d);
			}
		}

		//obtener e
	    e = (MathUtils::euclidesExtendido(x,d)).e;

		publicKey.e = e;
		publicKey.n = n;
		privateKey.n = n;
		privateKey.d = d;
	}

	void encrypt(void * buffer, size_t bytes) {

	}

	void decrypt(void * buffer, size_t bytes) {

	}

	RsaPublicKey getPublicKey(){
		return this->publicKey;
	}
};
#endif /* RSA_SECURITY_H_ */

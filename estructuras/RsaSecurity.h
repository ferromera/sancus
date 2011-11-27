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
#include <algorithm>
#include "../records/StudentRecord.h"

struct RsaPublicKey {
	unsigned int e;
	unsigned int n;
};

struct RsaPrivateKey {
	unsigned int d;
	unsigned int n;
};

class RsaSecurity: public SecurityStrategy {
private:
	RsaPublicKey publicKey;
	RsaPrivateKey privateKey;

public:
	RsaSecurity(unsigned int keySize) :
		SecurityStrategy(keySize) {
		unsigned int p = 0;
		unsigned int q = 0;
		unsigned int d = 0;
		unsigned int x = 0;
		unsigned int n = 0;
		EuclidesResult result;
		result.e = 0;

		//obtener p y q
		while (MathUtils::isPrime(p) || p == 0) {
			p = MathUtils::generateRandomIntegersOf(this->keySize);
		}

		while (MathUtils::isPrime(q) || q == p || q == 0) {
			q = MathUtils::generateRandomIntegersOf(this->keySize);
		}

		//calcular n
		n = p * q;

		//calcular fi(n)
		x = (p - 1) * (q - 1);

		//si e < 0 hay que tomar otro d
		while (result.e <= 0) {
			//obtener d
			d = 0;
			while (d < max(p, q)) {
				d = MathUtils::generateRandomIntegersOf(this->keySize + 1);

				if (!MathUtils::isPrime(d)) {
					MathUtils::nextPrime(d);
				}
			}

			//obtener e
			result = MathUtils::euclidesExtendido(x, d);
		}

		publicKey.e = result.e;
		publicKey.n = n;
		privateKey.n = n;
		privateKey.d = d;
	}

	void encrypt(void * m, size_t bytes) {
		int * f = (int *) m;

		for (unsigned int i = 0; i < bytes; i++) {
			f[i] = MathUtils::powMod(f[i], publicKey.e, publicKey.n);
		}
	}

	void decrypt(void * c, size_t bytes) {
		int * f = (int *) c;

		for (unsigned int i = 0; i < bytes; i++) {
			f[i] = MathUtils::powMod(f[i], privateKey.d, privateKey.n);
		}
	}

	RsaPublicKey getPublicKey() {
		return this->publicKey;
	}
};
#endif /* RSA_SECURITY_H_ */

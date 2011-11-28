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
			p = MathUtils::generateRandomIntegersOf(this->keySize / 2);
		}



		while (MathUtils::isPrime(q) || q == p || q == 0) {
			q = MathUtils::generateRandomIntegersOf((this->keySize+1) / 2);
		}

		//calcular n
		n = p * q;

		//calcular fi(n)
		x = (p - 1) * (q - 1);

		cout << "fi de n: " << x << endl;

		//si e < 0 hay que tomar otro d
		while (result.d != 1) {

			//obtener d
			d = 0;

			while (d < max(p, q)) {
				d = MathUtils::generateRandomIntegersOf(this->keySize + 1);

				//if (!MathUtils::isPrime(d)) {
				//MathUtils::nextPrime(d);
				//}
			}

			//obtener e
			result = MathUtils::euclidesExtendido(x, d);

			if (result.e < 0) {
				result.e = result.e % x;
			}

		}

		publicKey.e = result.e;
		publicKey.n = n;
		privateKey.n = n;
		privateKey.d = d;
	}

	void encrypt(unsigned char * &m, size_t bytes) {

		unsigned char * c_toChar = new unsigned char[bytes * 4];
		unsigned int c_i;
		unsigned char char_c_i;
		cout << "exponente e:" << publicKey.e << endl;
		cout << "n " << publicKey.n << endl;

		for (unsigned int i = 0; i < bytes; i++) {
			c_i = MathUtils::powMod(m[i], publicKey.e, publicKey.n);
			cout<<c_i<<endl;
			for (int j = 0; j<4; j++) {
				char_c_i = (unsigned char) (c_i & 255);
				cout<<"char c-i: "<<(unsigned int)char_c_i<<endl;
				c_toChar[4*i+j]=char_c_i;
				c_i = c_i >> 8;
			}
		}

		delete (m);
		m = c_toChar;
	}

	void decrypt(unsigned char * &c, size_t bytes) {
		unsigned char * plain=new unsigned char[bytes];
		unsigned int * &f = (unsigned int *&) c;

		cout << "MENSAJE " << endl;
		for (unsigned int i = 0; i < bytes; i++) {
			cout<<"ENCRIPTADO ES: "<<f[i]<<endl;
			plain[i] = (unsigned char)MathUtils::powMod(f[i], privateKey.d, privateKey.n);
			cout << f[i] << endl;
		}
		c=plain;
	}

	RsaPublicKey getPublicKey() {
		return this->publicKey;
	}
};
#endif /* RSA_SECURITY_H_ */

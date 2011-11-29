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
#include <math.h>
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
	unsigned int keySize;

public:
	RsaSecurity(unsigned int keySize) :
		SecurityStrategy(keySize) {
		srand((unsigned)time(0));
		this->keySize = keySize;
		unsigned int p = 0;
		unsigned int q = 0;
		unsigned int d = 0;
		unsigned int phi = 0;
		unsigned int n = 0;
		EuclidesResult result;
		result.e = 0;
		result.d = 0;
		//obtener p y q
		do{
			 do{
				p = MathUtils::generateRandomIntegersOf(this->keySize / 2 + 1);
			}while (!MathUtils::isPrime(p) || p == 0);
			 do{
				q = MathUtils::generateRandomIntegersOf((this->keySize + 1) / 2);
			}while (!MathUtils::isPrime(q) || q == p || q == 0);

			//calcular n
			n = p * q;
		}while (n < (unsigned int) pow(2, keySize-1 )|| n >= (unsigned int) pow(2, keySize ));
		//calcular fi(n)
		phi = (p - 1) * (q - 1);

		while (result.d != 1) {

			d = MathUtils::randomNumber(0, n - 1);
			result = MathUtils::euclidesExtendido(phi, d);
		}

		publicKey.e = (result.e +phi)%phi;
		publicKey.n = n;
		privateKey.n = n;
		privateKey.d = d;

		cout << "n: " << n << endl;
		cout << "p: " << p << endl;
		cout << "q: " << q << endl;
		cout << "fi de n: " << phi << endl;
		cout << "e: " << publicKey.e << endl;
		cout << "d: " << d << endl;

		/*
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
		 privateKey.d = d;*/
	}

	unsigned char * encrypt(unsigned char * m, size_t bytes) {
		unsigned int splittedSize = bytes * 8 / (keySize - 1);
		if (bytes % (keySize - 1))
			splittedSize++;
		unsigned int * msgSplitted = new unsigned int[splittedSize];
		split(m, bytes, msgSplitted, splittedSize,keySize-1);
		for (unsigned int i = 0; i < splittedSize; i++)
			cout << "encrypton splitted: " << msgSplitted[i] << endl;

		for (unsigned int i = 0; i < splittedSize; i++){
			cout<<"encriptando: "<<msgSplitted[i]<<endl;
			msgSplitted[i] = MathUtils::powMod(msgSplitted[i], publicKey.e, publicKey.n);
			cout<<"criptograma splitted: "<<msgSplitted[i]<<endl;
		}

		unsigned char * encrypted = new unsigned char[getEncryptedBytes(bytes)];
		join(msgSplitted,splittedSize, encrypted,getEncryptedBytes(bytes), keySize);
		for(unsigned int i=0;i<getEncryptedBytes(bytes);i++)
			cout<<(unsigned int)encrypted[i]<<endl;

		return encrypted;

		/* unsigned char * c_toChar = new unsigned char[bytes * 4];
		 unsigned int c_i;
		 unsigned char char_c_i;
		 cout << "exponente e:" << publicKey.e << endl;
		 cout << "n " << publicKey.n << endl;

		 for (unsigned int i = 0; i < bytes; i++) {
		 c_i = MathUtils::powMod(m[i], publicKey.e, publicKey.n);
		 cout << c_i << endl;
		 for (int j = 0; j < 4; j++) {
		 char_c_i = (unsigned char) (c_i & 255);
		 cout << "char c-i: " << (unsigned int) char_c_i << endl;
		 c_toChar[4 * i + j] = char_c_i;
		 c_i = c_i >> 8;
		 }
		 }

		 delete (m);
		 m = c_toChar;*/
	}

	void split(unsigned char * m, unsigned int bytes,unsigned int * toSplit,unsigned int splittedSize ,unsigned int bits) {
		for (unsigned int i = 0; i < splittedSize; i++) {
			toSplit[i] = 0;
		}
		unsigned int i = 0, b = 0;

		for(unsigned int byte=0;byte<bytes;byte++)
			for(unsigned int bit=0;bit<8;bit++){
				if(m[byte]&(unsigned char)pow(2,bit))
					toSplit[i]+= pow(2, b);

				b++;
				if(b==bits){
					i++;
					b=0;
				}
			}




	}
	void join(unsigned int * splitted, unsigned int splittedSize,unsigned char * toJoin,unsigned int toJoinSize ,unsigned int bits) {
		for (unsigned int i = 0; i < toJoinSize; i++) {
			toJoin[i] = 0;
				}
		unsigned int i = 0, b = 0;

				for(unsigned int byte=0;byte<toJoinSize;byte++)
					for(unsigned int bit=0;bit<8;bit++){
						if((splitted[i] & (unsigned int) pow(2, b)))
							toJoin[byte]+= (unsigned char) pow(2, bit);

						b++;
						if(b==bits){
							i++;
							b=0;
						}
					}

				/*
		unsigned int byte = 0, k = 0;
		for (unsigned int i = 0; i < splittedSize; i++) {
			for (unsigned int b = 0; b < keySize; b++) {
				if ((splitted[i] & (unsigned int) pow(2, b)))
					toJoin[byte] |= (unsigned char) pow(2, k);
				else
					toJoin[byte] &= ~(unsigned char) pow(2, k);
				k++;
				if (k == 8) {
					k = 0;
					byte++;
				}
			}
		}*/

	}
	unsigned int getEncryptedBytes(size_t bytes) {
		unsigned int splittedSize = bytes * 8 / (keySize - 1);
		if (bytes % (keySize - 1))
			splittedSize++;
		unsigned int encryptedSize = splittedSize * keySize / 8;
		if ((splittedSize * keySize) % 8)
			encryptedSize++;
		return encryptedSize;

	}

	unsigned char * decrypt(unsigned char * c, size_t bytes) {
		unsigned int splittedSize = bytes * 8 / (keySize - 1);
				if (bytes % (keySize - 1))
					splittedSize++;
		unsigned int * cSplitted = new unsigned int[splittedSize];
		split(c, getEncryptedBytes(bytes), cSplitted, splittedSize,keySize);

		for (unsigned int i = 0; i < splittedSize; i++){
			cout<<"desencriptando: "<<cSplitted[i]<<endl;
			cSplitted[i] = MathUtils::powMod(cSplitted[i], privateKey.d, privateKey.n);
			cout<<"desencriptado: "<<cSplitted[i]<<endl;
		}

		unsigned char * decrypted = new unsigned char[bytes];
		join(cSplitted, splittedSize, decrypted, bytes,keySize-1);
		return decrypted;
		/*
		 unsigned char * plain = new unsigned char[bytes];
		 unsigned int * &f = (unsigned int *&) c;

		 cout << "MENSAJE " << endl;
		 for (unsigned int i = 0; i < bytes; i++) {
		 cout << "ENCRIPTADO ES: " << f[i] << endl;
		 plain[i] = (unsigned char) MathUtils::powMod(f[i], privateKey.d, privateKey.n);
		 cout << f[i] << endl;
		 }
		 c = plain;*/
	}

	RsaPublicKey getPublicKey() {
		return this->publicKey;
	}

};
#endif /* RSA_SECURITY_H_ */

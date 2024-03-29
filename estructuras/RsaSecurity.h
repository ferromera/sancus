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
#define RSA_PRIVATE_KEY_PATH "./forbiddenAccessFolder/RSA_private_key.bin"
#define RSA_PUBLIC_KEY_PATH "./RSA_public_key.txt"
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
	static const unsigned int XOR_KEY = 2964551658;

public:
	RsaSecurity(unsigned int keySize) :
		SecurityStrategy(keySize) {
		if (!readKey()) {
			srand((unsigned) time(0));
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
			do {
				do {
					p = MathUtils::generateRandomIntegersOf(this->keySize / 2 + 1);
				} while (!MathUtils::isPrime(p) || p == 0);
				do {
					q = MathUtils::generateRandomIntegersOf((this->keySize + 1) / 2);
				} while (!MathUtils::isPrime(q) || q == p || q == 0);

				//calcular n
				n = p * q;
			} while (n < (unsigned int) pow(2, keySize - 1) || n >= (unsigned int) pow(2, keySize));
			//calcular fi(n)
			phi = (p - 1) * (q - 1);

			while (result.d != 1) {

				d = MathUtils::randomNumber(0, n - 1);
				result = MathUtils::euclidesExtendido(phi, d);
			}

			publicKey.e = (result.e + phi) % phi;
			publicKey.n = n;
			privateKey.n = n;
			privateKey.d = d;

			File privateFile(RSA_PRIVATE_KEY_PATH, File::BIN | File::NEW);
			unsigned int encryptedD = privateKey.d ^ XOR_KEY;
			privateFile.write(&encryptedD, sizeof(encryptedD));
			File publicFile(RSA_PUBLIC_KEY_PATH, File::NEW);
			publicFile << "e: \n";
			publicFile << publicKey.e;
			publicFile << " n: \n";
			publicFile << publicKey.n;

		}

	}
	bool readKey() {
		try {
			File privateFile(RSA_PRIVATE_KEY_PATH, File::BIN);
			unsigned int d, e=0, n=0;
			privateFile.read(&d, sizeof(d));
			privateKey.d = d ^ XOR_KEY;
			File publicFile(RSA_PUBLIC_KEY_PATH);
			std::string str;

			publicFile >> str;
			publicFile >> e;
			publicFile >> str;
			publicFile >> n;

			privateKey.n = n;
			publicKey.n = n;
			publicKey.e = e;
//			cout << "n: " << n << endl;
//						cout << "e: " << publicKey.e << endl;
//						cout << "d: " << privateKey.d << endl;

		} catch (OpenFileException e) {
			return false;
		}
		return true;
	}
	unsigned char * encrypt(unsigned char * m, size_t bytes) {
		unsigned int splittedSize = bytes * 8 / (keySize - 1);
		if ((bytes * 8) % (keySize - 1))
			splittedSize++;
		unsigned int * msgSplitted = new unsigned int[splittedSize];
		split(m, bytes, msgSplitted, splittedSize, keySize - 1);


		for (unsigned int i = 0; i < splittedSize; i++) {
			msgSplitted[i] = MathUtils::powMod(msgSplitted[i], publicKey.e, publicKey.n);
		}

		unsigned char * encrypted = new unsigned char[getEncryptedBytes(bytes)];
		join(msgSplitted, splittedSize, encrypted, getEncryptedBytes(bytes), keySize);

		return encrypted;

	}

	void split(unsigned char * m, unsigned int bytes, unsigned int * toSplit, unsigned int splittedSize,
			unsigned int bits) {
		for (unsigned int i = 0; i < splittedSize; i++) {
			toSplit[i] = 0;
		}
		unsigned int i = 0, b = 0;

		for (unsigned int byte = 0; byte < bytes; byte++)
			for (unsigned int bit = 0; bit < 8; bit++) {
				if (m[byte] & (unsigned char) pow(2, bit))
					toSplit[i] += pow(2, b);

				b++;
				if (b == bits) {
					i++;
					b = 0;
				}
			}

	}
	void join(unsigned int * splitted, unsigned int splittedSize, unsigned char * toJoin, unsigned int toJoinSize,
			unsigned int bits) {
		for (unsigned int i = 0; i < toJoinSize; i++) {
			toJoin[i] = 0;
		}
		unsigned int i = 0, b = 0;

		for (unsigned int byte = 0; byte < toJoinSize; byte++)
			for (unsigned int bit = 0; bit < 8; bit++) {
				if ((splitted[i] & (unsigned int) pow(2, b)))
					toJoin[byte] += (unsigned char) pow(2, bit);

				b++;
				if (b == bits) {
					i++;
					b = 0;
				}
			}

	}
	unsigned int getEncryptedBytes(size_t bytes) {
		unsigned int splittedSize = bytes * 8 / (keySize - 1);
		if ((bytes * 8) % (keySize - 1))
			splittedSize++;
		unsigned int encryptedSize = splittedSize * keySize / 8;
		if ((splittedSize * keySize) % 8)
			encryptedSize++;
		return encryptedSize;

	}
	/*
	 * Retorna la cantidad de bytes obtenidos al desencriptar
	 * encryptedBytes, debido a que es imposible obtener la cantidad
	 * de bytes exactas del bloque plano a partir de la cantidad de bytes
	 * encriptados se retorna la mayor cantidad
	 * de bytes posible del bloque plano.
	 */
	unsigned int getPlainBytes(size_t encryptedBytes){
		unsigned int splittedSize = encryptedBytes * 8 / keySize;
				if ((encryptedBytes * 8) % keySize )
					splittedSize++;
				unsigned int plainSize = splittedSize * (keySize-1) / 8;
				if ((splittedSize * (keySize-1)) % 8)
					plainSize++;
				return plainSize;
	}

	unsigned char * decrypt(unsigned char * c, size_t bytes) {
		unsigned int splittedSize = bytes * 8 / (keySize - 1);
		if (bytes * 8 % (keySize - 1))
			splittedSize++;
		unsigned int * cSplitted = new unsigned int[splittedSize];
		split(c, getEncryptedBytes(bytes), cSplitted, splittedSize, keySize);

		for (unsigned int i = 0; i < splittedSize; i++) {
			cSplitted[i] = MathUtils::powMod(cSplitted[i], privateKey.d, privateKey.n);
		}

		unsigned char * decrypted = new unsigned char[bytes];
		join(cSplitted, splittedSize, decrypted, bytes, keySize - 1);
		return decrypted;

	}

	RsaPublicKey getPublicKey() {
		return this->publicKey;
	}

};
#endif /* RSA_SECURITY_H_ */

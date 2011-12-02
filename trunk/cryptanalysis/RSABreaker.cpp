/*
 * RSADecryptor.cpp
 *
 *  Created on: 20/11/2011
 *      Author: fernando
 */

#include "RSABreaker.h"
#include <vector>
#include "../estructuras/MathUtils.h"
#include "../estructuras/File.h"
#include <math.h>
#include <iostream>

RSABreaker::RSABreaker() {

}
void RSABreaker::attack2(std::string encryptedFilePath,unsigned int N,unsigned int E) {
	/*Puede agregar algunos bytes basura al final*/

	unsigned int lastPrime=2;
	while(N%lastPrime!=0)
		lastPrime=MathUtils::nextPrime(lastPrime+1);
	unsigned int p= lastPrime;
	unsigned int q= N/lastPrime;
	unsigned int phi=(p-1)*(q-1);
	unsigned int d=  (MathUtils::euclidesExtendido(E, phi).f+phi)%phi;;
	File *file=new File();
	file->open(encryptedFilePath,File::BIN);
	file->seek(0,File::END);
	unsigned int size= file->tell();
	file->seek(0,File::BEG);
	unsigned char *buffer= new unsigned char[size];
	file->read(buffer,size);
	unsigned char *decryptedBuffer=decrypt(buffer,size,N,d);
	File *fileBreaked=new File();
	fileBreaked->open(encryptedFilePath.append("_breaked.bin"),File::NEW);
	unsigned int keySize=0;
		for(unsigned int i=1 ; i<=N;i=i<<1){
			keySize++;
		}
	unsigned int splittedSize = size * 8 / keySize ;
				if ((size*8) % keySize )
					splittedSize++;
				unsigned int painSize = splittedSize * (keySize-1) / 8;
				if ((splittedSize * (keySize-1)) % 8)
					painSize++;
	fileBreaked->write(decryptedBuffer,painSize);
	delete buffer;
	delete decryptedBuffer;
	delete file;
	delete fileBreaked;

}
unsigned char * RSABreaker::decrypt(unsigned char * c, size_t encryptedBytes,unsigned int N,unsigned int D){
	unsigned int keySize=0;
	for(unsigned int i=1 ; i<=N;i=i<<1){
		keySize++;
	}
	unsigned int splittedSize = encryptedBytes * 8 / keySize ;
	unsigned int * cSplitted = new unsigned int[splittedSize];
	split(c, encryptedBytes , cSplitted, splittedSize,keySize);

	for (unsigned int i = 0; i < splittedSize; i++){
		cSplitted[i] = MathUtils::powMod(cSplitted[i], D, N);
	}
	unsigned int plainSize = splittedSize * (keySize-1)/8;

	unsigned char * decrypted = new unsigned char[plainSize];
	join(cSplitted, splittedSize, decrypted, plainSize,keySize-1);
	return decrypted;
}
void RSABreaker::split(unsigned char * m, unsigned int bytes,unsigned int * toSplit,unsigned int splittedSize ,unsigned int bits) {
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
void RSABreaker::join(unsigned int * splitted, unsigned int splittedSize,unsigned char * toJoin,unsigned int toJoinSize ,unsigned int bits) {
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

}

RSABreaker::~RSABreaker() {
	// TODO Auto-generated destructor stub
}

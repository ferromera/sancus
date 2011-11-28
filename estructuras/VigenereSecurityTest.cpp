/*
 * Test.cpp
 *
 *  Created on: 20/10/2011
 *      Author: cecilia
 */

//#include "AdministratorScreen.h"
//#include "LoginScreen.h"
//#include "../core/VoteApp.h"
#include "VigenereSecurity.h"
#include <iostream>
#include <stdlib.h>
#include "../records/StudentRecord.h"
#include "../estructuras/MathUtilsTest.h"
#include "../estructuras/RsaSecurityTest.h"

using namespace std;

int main() {
	RsaSecurityTest * test = new RsaSecurityTest();

	test->testEncryptAndDecrypt();
//
//	int r;
//	for (int i = 0; i < 3; i++){
//
//
//		r = MathUtils::randomNumber(0,256);
//		cout << (unsigned char) r;
//		cout <<  r;
//	}

	//unsigned char message2[] = "CECI";
//	VigenereSecurity * vs;
//
//	char name[6];
//	char * buffer = name;
//
//	StudentRecord * record = new StudentRecord(12, "ceci");
//	record->read(&buffer);
//
//	vs = new VigenereSecurity(6);
//	vs->encrypt(&buffer,6);

//	VigenereSecurity sec(4);
//	sec.encrypt(message2, 4);
//	cout << message2 << endl;
//
//	sec.decrypt(message2, 4);
//	cout << message2 << endl;
//
//	int ceci2 = (int)'A';
//	char ceci1 = 'A';
//
//	cout << ceci2 << endl;
//	cout << ceci1 << endl;
//	cout << (int)ceci1 << endl;
//	printf("valooor: %c\n", ceci2);
//	printf("valooor: %d\n", ceci2);







	//AdministratorScreen myscreen;
//	LoginScreen myscreen;
	//VigenereSecurityTest mytest;
//	char key[] = "HOLA";
//	char message[] = "CHOCLO";
//	unsigned char encriptedMessage[6];
//
//	int bytes = 7;
//	int keySize = 4;
//
//	cout << key << endl;
//	cout << message << endl;
//
//	//mytest.desencrypt();
//
//	int j=0;
//	for(int i = 0; i< bytes; i++){
//		if(j>keySize){
//			j=0;
//		}
//		cout << "valor j: " << j << endl;
//
////		cout << "mensaje antes de la suma: " <<  message[i] << endl;
////		cout << "caracter de la clave: " << key[j] << endl;
////		printf("valooor: %d\n",message[i]);
////		printf("valooor: %d\n",key[j]);
//		printf("valooor: %d\n",(key[j] + message[i]) % 256);
//		unsigned char z;
//		cout << key[j] << endl;
//		cout << message[i] << endl;
//
//		z = (key[j] + message[i]) % 256;
//		printf("valooor: %d\n",z);
//		printf("valooor: %c\n",z);
//		encriptedMessage[i] = /*(char)*/z;
//
//		printf("valooor: %d\n",encriptedMessage[i]);
//		cout << "mensaje despues de la suma: " << encriptedMessage[i] << endl;
//		cout << "valor i : " << i << endl;
//		j++;
//	}
//
////	int j=0;
////	cout << message << endl;
////	for(int i = 0; i< bytes; i++){
////		if(j>keySize){
////			j=0;
////		}
////		cout << message[i] << endl;
////		if (message[i] - key[j] < 0){
////			message[i] = (message[i] - key[j]) + 256;
////			cout << message[i] << endl;
////			cout << key[j];
////		}
////		else {
////			message[i] = (message[i] - key[j]);
////			cout << message[i] << endl;
////		}
////		j++;
////
////	}
////	cout << key << endl;
////	cout << message << endl;

}


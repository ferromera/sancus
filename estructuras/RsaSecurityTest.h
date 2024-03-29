/*
 * RsaSecurityTest.h
 *
 *  Created on: Nov 20, 2011
 *      Author: alfredo
 */

#ifndef RSAUTILTEST__H_
#define RSAUTILTEST__H_

#include "RsaSecurity.h"
#include "../records/StudentRecord.h"

using namespace std;

class RsaSecurityTest {
private:
	RsaSecurity * rsa;

	void assertTrue(bool condition, string testname, string message) {
		if (!condition) {
			cout << testname << " FAIL." << endl;
			cout << message << endl;
			throw new exception();
		} else {
			cout << testname << " OK." << endl;
		}
	}

	void assertFalse(bool condition, string testname, string message) {
		assertTrue(!condition, testname, message);
	}

public:
	void testEncryptAndDecrypt() {
		StudentRecord * record = new StudentRecord(12,"anda hasta 26 bits");
	//	cout<<MathUtils::powMod(123456,32,200000)<<endl;


		this->rsa = new RsaSecurity(26);

		unsigned char * mensaje = new unsigned char[record->size()];
		char * punteroMensaje = (char *)mensaje;

		record->write(&punteroMensaje);
		unsigned char * encryption;
		encryption=this->rsa->encrypt(mensaje, record->size());


		mensaje=this->rsa->decrypt(encryption, record->size());

		for(unsigned int i = 0; i<record->size();i++){
			cout<<"Mensaje "<< mensaje[i] <<endl;
		}

		/**


		for (unsigned int i = 0; i < 4; i++) {
			cout << name[i];
		}

		cout << "**********" <<endl;
		cout << "DECRYPT" << endl;

		this->rsa->decrypt(buffer, 4);
		for (unsigned int i = 0; i < 4; i++) {
			cout << name[i];
		}
*/
	}

};

#endif

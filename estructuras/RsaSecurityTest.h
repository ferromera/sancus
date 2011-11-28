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
		char * buffer = "hola";

		string str="hola";

		for(unsigned int i=0; i<4; i++)
		       buffer[i]=str[i];

		this->rsa = new RsaSecurity(6);
		this->rsa->encrypt(buffer, 4);

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

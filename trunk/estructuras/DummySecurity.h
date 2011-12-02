/*
 * DummySecurity.h
 *
 * No security at all, same as a plain text file
 *
 *  Created on: Nov 15, 2011
 *      Author: alfredo
 */

#ifndef DUMMY_SECURITY_H_
#define DUMMY_SECURITY_H_

#include "SecurityStrategy.h"

class DummySecurity: public SecurityStrategy {

public:
	DummySecurity() :
		SecurityStrategy(0) {
	}

	unsigned char * encrypt(unsigned char * buffer, size_t bytes) {
		unsigned char * copy = new unsigned char[bytes];
		for (unsigned int i = 0; i < bytes; i++)
			copy[i] = buffer[i];
		return copy;
	}

	unsigned char * decrypt(unsigned char * buffer, size_t bytes) {
		unsigned char * copy = new unsigned char[bytes];
		for (unsigned int i = 0; i < bytes; i++)
			copy[i] = buffer[i];
		return copy;
	}
};
#endif /* DUMMY_SECURITY_H_ */

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

class DummySecurity : public SecurityStrategy {

public:
	DummySecurity(): SecurityStrategy(0){
	}

   void encrypt(void * buffer, size_t bytes){
        //DO NOTHING
	}
	
	void decrypt(void * buffer, size_t bytes){
		//DO NOTHING
	}
};
#endif /* DUMMY_SECURITY_H_ */

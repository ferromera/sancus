/*
 * BaseTestCase.h
 *
 *  Created on: 09/10/2011
 *      Author: ascoppa
 */

#ifndef BASETESTCASE_H_
#define BASETESTCASE_H_

#include <iostream>

using namespace std;

class BaseTestCase {

public:
	virtual void run() = 0;

	void assertTrue(bool condition, string message){
		if(!condition){
			cout<<message<<endl;
			throw new exception();
		}
	}

	void assertFalse(bool condition, string message){
		assertTrue(!condition, message);
	}
}
#endif /* BASETESTCASE_H_ */

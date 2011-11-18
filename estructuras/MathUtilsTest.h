/*
 * MathUtilsTest
 *
 *  Created on: Nov 18, 2011
 *      Author: alfredo
 */

#ifndef MATHUTILSTEST_H_
#define MATHUTILSTEST_H_

#include "MathUtils.h"
using namespace std;

class MathUtilsTest {

private:
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
	void testEuclides() {
		int a = 2668;
		int b = 157;

		EuclidesResult mcd = MathUtils::euclidesExtendido(a,b);

		assertTrue(mcd.e == 17, "testEuclides", "El resultado no es el esperado");

		assertTrue(mcd.d == (mcd.f * a) + (mcd.e*b), "testEuclides", "No es una combinacion lineal");
	}

};

#endif

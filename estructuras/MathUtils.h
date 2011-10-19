/*
 * MathUtils.h
 *
 *  Created on: Oct 9, 2011
 *      Author: alfredo
 */

#ifndef MATHUTILS_H_
#define MATHUTILS_H_

#include <math.h>

class MathUtils {

public:
	static int nextPrime(unsigned int number) {
		while(!isPrime(number)){
			number++;
		}

		return number;
	}

	static bool isPrime(const unsigned int number) {
		if (number == 2)
			return true;
		if (number % 2 == 0)
			return false;

		for (int i = 3; i <= ceil(sqrt(number)); i += 2) {
			if (number % i == 0)
				return false;
		}

		return true;
	}

	static int randomNumber(unsigned int base, unsigned int max){
			return rand() % (max - base + 1) + base;
		}
};
#endif /* MATHUTILS_H_ */

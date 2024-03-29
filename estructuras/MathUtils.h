/*
 * MathUtils.h
 *
 *  Created on: Oct 9, 2011
 *      Author: alfredo
 */

#ifndef MATHUTILS_H_
#define MATHUTILS_H_

#include <math.h>
#include <time.h>
#include <stdlib.h>

struct EuclidesResult {
	int d;
	int f;
	int e; //este es el numero e, en RSA
};

class MathUtils {
public:
	static int nextPrime(unsigned int number) {
		while (!isPrime(number)) {
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

	static unsigned int randomNumber(unsigned int base, unsigned int max) {
		//srand((unsigned)time(0));
		return ((unsigned int )rand()) % (max - base + 1) + base;
	}

	/* procedimiento que calcula el valor del maximo común divisor

	 * de a y b siguiendo el algoritmo de euclides extendido,

	 * devolviendo en un arreglo la siguiente estructura [d,x,y]

	 * donde:
	 *    mcd(a,b) = d = a*x + b*y
	 **/

	/**
	 * Calcula el maximo comun divisor de a y b con algoritmo de
	 * euclides extendido.
	 *
	 * mcd(a,b) = d = a*f + b*e;
	 *
	 * @returns EuclidesResult stuct
	 */
	static EuclidesResult euclidesExtendido(int a, int b) {
		EuclidesResult result;
		int x = 0, y = 0;

		if (b == 0) {
			result.d = a;
			result.f = 1;
			result.e = 0;
		} else {
			int x2 = 1, x1 = 0, y2 = 0, y1 = 1;
			int q = 0, r = 0;

			while (b > 0) {
				q = (a / b);
				r = a - q * b;
				x = x2 - q * x1;
				y = y2 - q * y1;
				a = b;
				b = r;
				x2 = x1;
				x1 = x;
				y2 = y1;
				y1 = y;
			}

			result.d = a;
			result.f = x2;
			result.e = y2;
		}
		return result;
	}

	static int generateRandomIntegersOf(unsigned int bits) {
		unsigned int randomNumber = 0;
		/*
		for (unsigned int i = 0; i < bits; i++) {
			randomNumber += ((rand() >> i) & 1) * pow(2, i);
		}*/
		randomNumber=pow(2, bits-1);
		randomNumber+= MathUtils::randomNumber(0,pow(2, bits-1)-1);

		return randomNumber;
	}

	static unsigned int powMod(unsigned int base, unsigned int exp, unsigned int mod) {
		if (exp == 0)
			return 1;
		if (exp % 2 == 0) {
			return ((unsigned int)fmod((pow(powMod((long double)base, (exp / 2), mod), 2)),mod));
		} else {
			return ((unsigned int)fmod(((long double)base * powMod((long double)base, (exp - 1), mod)),mod)) ;
		}
	}

};
#endif /* MATHUTILS_H_ */

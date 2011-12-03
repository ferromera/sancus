/*
 * ResultScreen.h
 *
 *  Created on: 17/10/2011
 *      Author: Romera 2
 */

#ifndef RESULTSCREEN_H_
#define RESULTSCREEN_H_
#define VIGENEREKEYSIZE 23

#include "Drawable.h"
#include "../estructuras/VigenereSecurity.h"

class ResultScreen: public Drawable {
public:
	ResultScreen();
	void draw();
	virtual ~ResultScreen();
	char resultByDistrict();
	char resultByList();
	char resultByElection();
	char doQuestion();

};

#endif /* RESULTSCREEN_H_ */

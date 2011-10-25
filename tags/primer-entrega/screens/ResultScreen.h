/*
 * ResultScreen.h
 *
 *  Created on: 17/10/2011
 *      Author: Romera 2
 */

#ifndef RESULTSCREEN_H_
#define RESULTSCREEN_H_

#include "Drawable.h"

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

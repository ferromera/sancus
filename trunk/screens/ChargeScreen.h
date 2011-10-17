/*
 * ChargeScreen.h
 *
 *  Created on: 16/10/2011
 *      Author: juanmromera
 */

#ifndef CHARGESCREEN_H_
#define CHARGESCREEN_H_

#include "Drawable.h"

class ChargeScreen: public Drawable {
public:
	ChargeScreen();
	void draw();
private:
	char chargeDistrict();
	char chargeVoter();
	void chargeElection();
	void chargeList();
	void chargeCandidate();
	void chargeCharge();
	void chargeCounting();
	char doQuestion();
};

#endif /* CHARGESCREEN_H_ */

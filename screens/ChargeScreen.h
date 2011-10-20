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
	//VoterRecord user;
	char chargeDistrict();
	char chargeVoter();
	char chargeElection();
	char chargeList();
	char chargeCandidate();
	char chargeCharge();
	char chargeCounting();
	char chargeAdministrator();
	char doQuestion();
	bool validateDNI(uint32_t dni);
};

#endif /* CHARGESCREEN_H_ */

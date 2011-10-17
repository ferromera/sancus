/*
 * LoginScreen.h
 *
 *  Created on: 14/10/2011
 *      Author: juanmromera
 */

#ifndef LOGINSCREEN_H_
#define LOGINSCREEN_H_

#include "Drawable.h"
#include "stdint.h"
//#include "../records/VoterRecord.h"

class LoginScreen: public Drawable {
public:
	LoginScreen();
	void draw();
private:
	//bool validateDNI(uint32_t dni);
	//VoterRecord user;
};

#endif /* LOGINSCREEN_H_ */

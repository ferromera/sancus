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
#include "../core/VoteApp.h"

class LoginScreen: public Drawable {
public:
	LoginScreen();
	void draw();
private:
	bool validateDNI(uint32_t dni);
	VoterRecord user;
	bool validateAdmin(uint32_t user);
	AdministratorRecord adminUser;
};

#endif /* LOGINSCREEN_H_ */

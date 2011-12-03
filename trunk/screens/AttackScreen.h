/*
 * AttackScreen.h
 *
 *  Created on: 02/12/2011
 *      Author: juanmromera
 */

#ifndef ATTACKSCREEN_H_
#define ATTACKSCREEN_H_

#include "Drawable.h"

class AttackScreen : public Drawable{
public:
	AttackScreen();
	void draw();
	void attackAdminFile();
	void attackVoterFile();
	void attackRSAFile();
	void attackreportFilebyList();
	void attackreportFilebyElection();
	void attackreportFileByDistrict();
};

#endif /* ATTACKSCREEN_H_ */

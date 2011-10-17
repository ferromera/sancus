/*
 * ElectionScreen.h
 *
 *  Created on: 14/10/2011
 *      Author: juanmromera
 */

#ifndef ELECTIONSCREEN_H_
#define ELECTIONSCREEN_H_

#include "Drawable.h"
//#include "../records/ElectionRecord.h"
#include <list>

class ElectionScreen: public Drawable {
public:
	ElectionScreen();
	void draw();
private:
	//std::list<ElectionRecord> elections;
	//std::list<ElectionRecord>::iterator getElectionNumber(unsigned int);
};

#endif /* ELECTIONSCREEN_H_ */

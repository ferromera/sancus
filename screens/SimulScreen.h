/*
 * SimulScreen.h
 *
 *  Created on: 21/10/2011
 *      Author: fernando
 */

#ifndef SIMULSCREEN_H_
#define SIMULSCREEN_H_

#include "../generators/Generator.h"
#include "../generators/VotesGenerator.h"
#include "../core/VoteApp.h"
#include "Drawable.h"

class SimulScreen : public Drawable {
public:
	SimulScreen();
	void draw();
	virtual ~SimulScreen();
};

#endif /* SIMULSCREEN_H_ */

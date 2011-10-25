/*
 * SimulScreen.cpp
 *
 *  Created on: 21/10/2011
 *      Author: fernando
 */

#include "SimulScreen.h"

SimulScreen::SimulScreen() {
	// TODO Auto-generated constructor stub

}

SimulScreen::~SimulScreen() {
	// TODO Auto-generated destructor stub
}
void SimulScreen::draw(){
	Generator gen(20111023);
	gen.GenerateElection();
	VoteApp* app= VoteApp::getInstance();
	VotesGenerator vGen;
	for(unsigned int i= 20000000;i<20005000;i++)
		vGen.vote(i);
	app->setActualScreen(ADM_SCREEN);

	cout<<"Votacion concluida "<<endl;

	return;

}

/*
 * VoteApp.cpp
 *
 *  Created on: 14/10/2011
 *      Author: juanmromera
 */

#include "VoteApp.h"
#include "../screens/Drawable.h"
#include "../screens/LoginScreen.h"
#include "../screens/ElectionScreen.h"
#include "../screens/ListScreen.h"
#include "../screens/AdministratorScreen.h"
#include "../screens/ChargeScreen.h"
#include "../screens/ResultScreen.h"

VoteApp* VoteApp::voteApp = NULL;

VoteApp::VoteApp() {

	Drawable *scr1 = new LoginScreen();
	Drawable *scr2 = new ElectionScreen();
	Drawable *scr3 = new ListScreen();
	Drawable *scr4 = new AdministratorScreen();
	Drawable *scr5 = new ChargeScreen();
	Drawable *scr6 = new ChargeScreen();
	Drawable *scr7 = new ChargeScreen();
	Drawable *scr8 = new ResultScreen();

	this->screens.push_back(scr1);
	this->screens.push_back(scr2);
	this->screens.push_back(scr3);
	this->screens.push_back(scr4);
	this->screens.push_back(scr5);
	this->screens.push_back(scr6);
	this->screens.push_back(scr7);
	this->screens.push_back(scr8);

	actualScreen = ALTA_SCREEN;

}
VoteApp* VoteApp::getInstance()
{
	if ( VoteApp::voteApp  == NULL )
		VoteApp::voteApp = new VoteApp();
	return VoteApp::voteApp;
}

void VoteApp::run(){
	while(true)
	{
		std::list<Drawable*>::iterator screen  = getToScreen(actualScreen);
		(*screen)->draw();
	}
}
std::list<Drawable*>::iterator VoteApp::getToScreen(unsigned int screenNumber)
{
	std::list<Drawable*>::iterator itList = screens.begin();
	int a = screens.size();
	int i = 0;
	while(i < screenNumber)
	{
		itList++;
		i++;
	}


	return itList;
}
void VoteApp::setActualScreen(unsigned int number){
	actualScreen = number;
}
/*
void VoteApp::setUserLogin(VoterRecord user)
{
	userLogin = user;
}
void VoteApp::setChooseElection(ElectionRecord election)
{
	chooseElection = election;
}
VoterRecord VoteApp::getUserLogin()
{
	return userLogin;
}
ElectionRecord VoteApp::getChooseElection()
{
	return chooseElection;
}
*/


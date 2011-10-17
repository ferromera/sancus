/*
 * VoteApp.h
 *
 *  Created on: 14/10/2011
 *      Author: juanmromera
 */

#ifndef VOTEAPP_H_
#define VOTEAPP_H_

#include <list>
#include "Runable.h"
#include "stdint.h"
#include "../utils/Time.h"
//#include "../records/VoterRecord.h"
//#include "../records/ElectionRecord.h"
#include "../screens/Drawable.h"
#define LOGIN_SCREEN 0
#define ELECTION_SCREEN 1
#define LIST_SCREEN 2
#define ADM_SCREEN 3
#define ALTA_SCREEN 4
#define BAJA_SCREEN 5
#define MOD_SCREEN 6
#define CONSULTA_SCREEN 7

class VoteApp : public Runable{
public:
	VoteApp();
	void run();
	void setActualScreen(unsigned int);
	uint32_t dni;
	static VoteApp* getInstance();
	/*
	void setUserLogin(VoterRecord);
	void setChooseElection(ElectionRecord);
	VoterRecord getUserLogin();
	ElectionRecord getChooseElection();
	*/
private:
	std::list<Drawable*> screens;
	std::list<Drawable*>::iterator getToScreen(unsigned int);
	unsigned int actualScreen;
	static VoteApp* voteApp;
	/*
	VoterRecord userLogin;
	ElectionRecord chooseElection;
	*/

};

#endif /* VOTEAPP_H_ */

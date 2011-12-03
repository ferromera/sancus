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
#include "../managers/VoterFile.h"
#include "../managers/AdministratorFile.h"
#include "../managers/DistrictFile.h"
#include "../managers/ElectionFile.h"
#include "../managers/CandidateFile.h"
#include "../managers/ChargeFile.h"
#include "../managers/ListFile.h"
#include "../managers/VoteCountingFile.h"
#include "../records/ElectionRecord.h"
#include "../screens/Drawable.h"
#define LOGIN_SCREEN 0
#define ELECTION_SCREEN 1
#define LIST_SCREEN 2
#define ADM_SCREEN 3
#define ALTA_SCREEN 4
#define BAJA_SCREEN 5
#define MOD_SCREEN 6
#define CONSULTA_SCREEN 7
#define SIMUL_SCREEN 8
#define REPORT_FILE_SCREEN 9
#define ATTACK_SCREEN 10
#define ENC_SCREEN 11
#define DESC_SCREEN 12
#define CRYPT_SCREEN 13

class VoteApp : public Runable{
public:
	VoteApp();
	void run();
	void setActualScreen(unsigned int);
	uint32_t dni;
	static VoteApp* getInstance();

	void setUserLogin(VoterRecord);
	void setChooseElection(ElectionRecord);
	const VoterRecord & getUserLogin();
	ElectionRecord getChooseElection();
    bool getQuit() const;
    void setQuit(bool quit);

private:
	bool quit;
	std::list<Drawable*> screens;
	std::list<Drawable*>::iterator getToScreen(unsigned int);
	unsigned int actualScreen;
	static VoteApp* voteApp;

	VoterRecord userLogin;
	ElectionRecord chooseElection;


};

#endif /* VOTEAPP_H_ */

//============================================================================
// Name        : Interfaz.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "../utils/Time.h"
#include "VoteApp.h"
using namespace std;

int main() {
	//cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	VoteApp *app = VoteApp::getInstance();
	app->run();
	delete app;
	return 0;
}

/*
 * Time.h
 *
 *  Created on: 17/10/2011
 *      Author: Romera 2
 */

#ifndef TIME_H_
#define TIME_H_

#include <stdio.h>
#include <time.h>
#include <string>

class Time {
public:
	Time();
	static int getCurrentDate();
	static std::string getTime();

};

#endif /* TIME_H_ */

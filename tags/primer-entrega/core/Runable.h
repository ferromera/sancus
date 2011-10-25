/*
 * Runable.h
 *
 *  Created on: 14/10/2011
 *      Author: juanmromera
 */

#ifndef RUNABLE_H_
#define RUNABLE_H_

class Runable {
public:
	Runable();
	void virtual run()=0;
	void virtual setActualScreen(unsigned int)=0;
};

#endif /* RUNABLE_H_ */

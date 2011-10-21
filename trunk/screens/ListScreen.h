/*
 * ListScreen.h
 *
 *  Created on: 14/10/2011
 *      Author: juanmromera
 */

#ifndef LISTSCREEN_H_
#define LISTSCREEN_H_

#include "Drawable.h"
#include "../core/VoteApp.h"

class ListScreen: public Drawable {
	std::list<DistrictRecord::Key> getDistrictListOf(const DistrictRecord::Key & district);
	void showList(ListRecord);
public:
	ListScreen();
	void draw();

};

#endif /* LISTSCREEN_H_ */

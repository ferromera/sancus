/*
 * Drawable.h
 *
 *  Created on: 14/10/2011
 *      Author: juanmromera
 */

#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include "stdlib.h"
#include "../utils/IstreamUtils.h"
#include <iostream>
#include <string>
#include <stdlib.h>

class Drawable {
public:
	Drawable(){}
	virtual void draw()=0;
};

#endif /* DRAWABLE_H_ */

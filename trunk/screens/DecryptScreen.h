/*
 * DecryptScreen.h
 *
 *  Created on: 03/12/2011
 *      Author: fernando
 */

#ifndef DECRYPTSCREEN_H_
#define DECRYPTSCREEN_H_

#include "Drawable.h"

class DecryptScreen: public Drawable {
	void decryptByVigenereWithCustomKey();
	void decryptByVigenereWithSystemKey();

public:
	DecryptScreen();
	void draw();
	void decryptByVigenere();
	void decryptByRsa();
};

#endif /* DECRYPTSCREEN_H_ */

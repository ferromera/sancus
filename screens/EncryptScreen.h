/*
 * EncryptScreen.h
 *
 *  Created on: 03/12/2011
 *      Author: fernando
 */

#ifndef ENCRYPTSCREEN_H_
#define ENCRYPTSCREEN_H_

#include "Drawable.h"

class EncryptScreen: public Drawable {
	void encryptByVigenereWithCustomKey();
	void encryptByVigenereWithSystemKey();
	void decryptByRsa();
public:
	EncryptScreen();
	void draw();
	void encryptByVigenere();
	void encryptByRsa();
};

#endif /* ENCRYPTSCREEN_H_ */

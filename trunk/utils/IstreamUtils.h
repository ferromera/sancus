/*
 * IstreamUtils.h
 *
 *  Created on: 16/10/2011
 *      Author: juanmromera
 */

#ifndef ISTREAMUTILS_H_
#define ISTREAMUTILS_H_

#include <string>
#include <istream>
#include <stdint.h>

class IstreamUtils {
public:
	IstreamUtils();
	static std::string getString();
	static char getChar();
	static uint32_t getUint();
};

#endif /* ISTREAMUTILS_H_ */

/*
 * IstreamUtils.cpp
 *
 *  Created on: 16/10/2011
 *      Author: juanmromera
 */

#include "IstreamUtils.h"
#include "StringUtils.h"
#include <iostream>

IstreamUtils::IstreamUtils() {

}

std::string IstreamUtils::getString(){
	std::string str;
	getline(std::cin,str);
	return str;
}
char IstreamUtils::getChar()
{
	std::string str;
	getline(std::cin,str);
	return str[0];
}
uint32_t IstreamUtils::getUint()
{
	std::string str;
	getline(std::cin,str);
	return StringUtils::strToint(str);
}

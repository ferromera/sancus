/*
 * StringUtils.cpp
 *
 *  Created on: 14/10/2011
 *      Author: juan manuel
 */

#include "StringUtils.h"
#include <iostream>

StringUtils::StringUtils() {}

StringUtils::~StringUtils() {}

bool StringUtils::isRGB(std::string cadenaRGB)
{
	int length = cadenaRGB.length();

	if(length != 7)
		return false;
	if(cadenaRGB[0] != '#')
		return false;

	bool result = false;
	for(int i=1; i<length ;i++)
	{
		if((cadenaRGB[i]>='0') && (cadenaRGB[i]<='9'))
		{
			result = true;
		}
		else if ((cadenaRGB[i]>='A') && (cadenaRGB[i]<='F'))
		{
			result = true;
		}
		else if ((cadenaRGB[i]>='a') && (cadenaRGB[i]<='f'))
		{
			result = true;
		}
		else
			return false;
	}
	return result;
}

std::string StringUtils::parseTimeToHHMMSS(int time)
{
	std::string util;
	util = "0";
	int playedInt = time;

	playedInt = playedInt / 1000;
	std::string seg = intToString(playedInt%60);
	if(seg.size() < 2)
		seg = util+seg;
	playedInt = playedInt / 60;
	std::string min = intToString(playedInt%60);
	if(min.size() < 2)
		min = util+min;
	playedInt = playedInt / 60;
	std::string hora = intToString(playedInt);
	hora += ":" + min +":" + seg;

	return hora;
}
int StringUtils::hexToInt(char value){
	int num;
	if (value >= '0' && value <= '9')
		num = value-'0';
	else if (value >= 'A' && value <= 'Z')
		num = value-'A'+10;
	else if (value >= 'a' && value <= 'z')
		num = value-'a'+10;
	return num;
}

std::string StringUtils::toLower(std::string upper){
	std::string lower = "";
	int length = upper.length();
	for(int i=0; i<length ;i++){
		if((upper[i]>='A') && (upper[i]<='Z'))
			lower += (char)tolower((int)upper[i]);
		else
			lower += upper[i];
	}
	return lower;
}
int StringUtils::strToint(std::string str)
{
	std::stringstream ss(str);
	int n;
	ss >> n;
	return n;
}
float StringUtils::strToFloat(std::string str)
{
	std::stringstream ss(str);
	float n;
	ss >> n;
	return n;
}
void StringUtils::leftTrim(std::string &str)
{
	if (str.find_first_of(' ') == 0) //si hay espacio al principio
	{
		str.assign(str, 1, str.size()-1); //borro el primero
		leftTrim(str); //vuelvo a chequear
	}
}

void StringUtils::rightTrim(std::string &str)
{
	if (str.find_last_of(' ') == str.size()-1) //si hay espacio al final
	{
		str.assign(str, 0, str.size()-1); //borro el ultimo
		rightTrim(str); //vuelvo a chequear
	}
}

void StringUtils::trim(std::string &str)
{
	leftTrim(str);
	rightTrim(str);
}
std::string StringUtils::intToString(int num)
{
	std::stringstream strm;
	std::string s_num;
	strm << num;
	strm >> s_num;

	return s_num;

}

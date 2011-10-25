/*
 * StringUtils.h
 *
 *  Created on: 14/10/2011
 *      Author: juan manuel
 */

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_
#include <string>
#include <sstream>

class StringUtils {
public:
	StringUtils();
	virtual ~StringUtils();
	static std::string toLower(std::string);
	static int strToint(std::string);
	static std::string parseTimeToHHMMSS(int);
	static float strToFloat(std::string str);
	static void trim(std::string &str);
	static bool isRGB(std::string);
	static int hexToInt(char);
	static std::string intToString(int);
private:
	static void rightTrim(std::string &str);
	static void leftTrim(std::string &str);

};

#endif /* STRINGUTILS_H_ */

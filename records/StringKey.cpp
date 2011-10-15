/*
 * StringKey.cpp
 *
 *  Created on: 06/10/2011
 *      Author: juanmromera
 */

#include "StringKey.h"
#include <cstring>

StringKey::StringKey(char ** input)
{
	read(input);
}
StringKey::StringKey(std::string key)
{
	dataString = key;
}
StringKey::StringKey(const StringKey & sk)
{
	dataString = sk.dataString;
}
void StringKey::setKey(std::string  string)
{
	dataString = string;
}
void StringKey::setKey(const Record::Key & rk)
{
    const StringKey & sk= dynamic_cast<const StringKey &>(rk);
    dataString=sk.dataString;
}
std::string StringKey::getKey()const
{
	return dataString;
}
void StringKey::read(char ** input)
{
    unsigned int sizeStr;
	memcpy(&sizeStr,*input,1);
    (*input)+=1;
    char *myChar = new char[sizeStr + 1];
    memcpy(&myChar,*input,sizeStr);
    (*input)+=sizeStr;
    myChar[sizeStr] = '\0';
    dataString = myChar;

}
void StringKey::write(char ** output)const
{
	unsigned int sizeStr = dataString.size();
    memcpy(*output,&sizeStr,1);
    (*output)+=1;
    memcpy(*output,dataString.c_str(),sizeStr);
    (*output)+=sizeStr;
}
bool StringKey::operator <(const Record::Key &r)const
{
	const StringKey & key = (const StringKey &) r;
	if(dataString.compare(key.dataString) > 0)
		return false;
	else
		return true;
}
bool StringKey::operator ==(const Record::Key &r)const
{
	const StringKey & key = (const StringKey &) r;
	if(dataString.compare(key.dataString) == 0)
		return true;
	return false;
}
bool StringKey::operator <=(const Record::Key & r)const
{
	return *this < r || *this == r;
}
bool StringKey::operator >(const Record::Key &r)const
{
	return ! *this <= r;
}
bool StringKey::operator !=(const Record::Key &r)const
{
	return ! *this == r;
}

bool StringKey::operator >=(const Record::Key & r)const
{
	return ! *this<r;
}
StringKey & StringKey::operator=(const StringKey & rk)
{
	if(this == &rk)
		return *this;

	dataString = rk.dataString;
	return *this;
}
StringKey & StringKey::operator=(std::string string)
{
	dataString = string;
	return *this;
}



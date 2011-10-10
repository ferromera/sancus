/*
 * StringKey.cpp
 *
 *  Created on: 06/10/2011
 *      Author: juanmromera
 */

#include "StringKey.h"

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
    myChar[sizeStr] = "\0";
    dataString = myChar;

}
void StringKey::write(char ** output)const
{
	unsigned int sizeStr = dataString.size();
    memcpy(*output,&sizeStr,1);
    (*output)+=1;
    char *myChar = dataString.c_str();
    memcpy(*output,&myChar,sizeStr);
    (*output)+=sizeStr;
}
//bool StringKey::operator <(const Record::Key &r)const
//bool StringKey::operator ==(const Record::Key &r)const
//bool StringKey::operator <=(const Record::Key & r)const
//bool StringKey::operator >(const Record::Key &r)const
//bool StringKey::operator !=(const Record::Key &r)const
//bool StringKey::operator >=(const Record::Key & r)const
//Record::Key & StringKey::operator=(const Record::Key & rk)
//StringKey & StringKey::operator=(const StringKey & rk)
//StringKey & StringKey::operator=(std::string string)


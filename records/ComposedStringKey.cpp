/*
 * ComposedStringKey.cpp
 *
 *  Created on: 06/10/2011
 *      Author: juanmromera
 */

#include "ComposedStringKey.h"

 ComposedStringKey::ComposedStringKey(char ** input){
	 read(input);
 }
 ComposedStringKey::ComposedStringKey(std::list<std::string> keys)
 {
	 stringList = keys;
	 updateStringKey();
 }
 ComposedStringKey::ComposedStringKey(const ComposedStringKey & ck)
 {
	 stringList = ck.stringList;
	 updateStringKey();
 }
 void ComposedStringKey::updateStringKey()
 {
	 std::list<std::string>::iterator itKeys = stringList.begin();
	 dataString = "";
	 dataString.append(*itKeys);
	 itKeys++;

	 for(;itKeys != stringList.end();itKeys++)
	 {
		 dataString.append(SEPARATOR_SYMBOL);
		 dataString.append(*itKeys);
	 }
 }
 void ComposedStringKey::setKey(std::string stringKey)
 {
	 dataString = stringKey;
 }
 void ComposedStringKey::setString(unsigned int index , std::string keyComponent)
 {
	 if(stringList.size() > index)
	 {
		 //Arrojo excepcion
	 }
	 std::list<std::string>::iterator itKeys = stringList.begin();
	 for(int i = 0 ; i < index ; itKeys++);
	 *itKeys = keyComponent;
	 updateStringKey();
 }
void ComposedStringKey::setKey(const Record::Key & rk)
{
    const ComposedStringKey & ik= dynamic_cast<const ComposedStringKey &>(rk);
    stringList=ik.stringList;
    updateStringKey();
}
std::string ComposedStringKey::getKey()const
{
	return dataString;
}
std::string ComposedStringKey::getString(unsigned int index)
{
	 if(stringList.size() > index)
	 {
		 //Arrojo excepcion
	 }
	 std::list<std::string>::iterator itKeys = stringList.begin();
	 for(int i = 0 ; i < index ; itKeys++);
	 return itKeys++;
}

Record::Key & ComposedStringKey::operator=(const Record::Key & rk)
{
    const ComposedStringKey & ik= dynamic_cast<const ComposedStringKey &>(rk);
    stringList=ik.stringList;
    updateStringKey();
    return (*this);
}

ComposedStringKey & ComposedStringKey::operator=(const ComposedStringKey & rk)
{
	stringList = rk.stringList;
	updateStringKey();
	return (*this);
}


ComposedStringKey::~ComposedStringKey() {
}


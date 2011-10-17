/*
 * StringKey.h
 *
 *  Created on: 06/10/2011
 *      Author: juanmromera
 */

#ifndef STRINGKEY_H_
#define STRINGKEY_H_

#include "Record.h"
#include <string>
#include <stdio.h>

class StringKey: public Record::Key {
protected:
	std::string dataString;
public:
    static const bool isVariable=true;
    static const bool isString=true;
    StringKey(char ** input); //Load the key from input.
    StringKey(const std::string & key="");
    StringKey(const StringKey &);
    void setKey(std::string  string);
    void setKey(const Record::Key & rk);
    const std::string & getKey()const;
    const std::string & getString()const{return getKey();}
    void read(char ** input);
    void write(char ** output)const;

    bool operator <(const Record::Key &r)const;
    bool operator ==(const Record::Key &r)const;
    bool operator <=(const Record::Key & r)const;
    bool operator >(const Record::Key &r)const;
    bool operator !=(const Record::Key &r)const;
    bool operator >=(const Record::Key & r)const;
    StringKey & operator=(const StringKey & rk);
    StringKey & operator=(const std::string& string);
    unsigned int size()const{ return dataString.size() + 1; }
	~StringKey(){}

};

#endif /* STRINGKEY_H_ */

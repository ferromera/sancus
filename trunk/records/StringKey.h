/*
 * StringKey.h
 *
 *  Created on: 06/10/2011
 *      Author: juanmromera
 */

#ifndef STRINGKEY_H_
#define STRINGKEY_H_

#include "../estructuras/Record.h"
#include <string>

class StringKey: public Record::Key {
public:
    static const bool isVariable=false;
    StringKey(char ** input); //Load the key from input.
    StringKey(std::string key="");
    StringKey(const StringKey &);
    void setKey(std::string  string);
    void setKey(const Record::Key & rk);
    std::string getKey()const;

    void read(char ** input);
    void write(char ** output)const;

    bool operator <(const Record::Key &r)const;
    bool operator ==(const Record::Key &r)const;
    bool operator <=(const Record::Key & r)const;
    bool operator >(const Record::Key &r)const;
    bool operator !=(const Record::Key &r)const;
    bool operator >=(const Record::Key & r)const;

    Record::Key & operator=(const Record::Key & rk);
    StringKey & operator=(const StringKey & rk);
    StringKey & operator=(std::string string);
    unsigned int size()const{ return dataString.size() + 1; }
	StringKey();
	~StringKey(){}
protected:
	std::string dataString;
};

#endif /* STRINGKEY_H_ */

/*
 * ComposedStringKey.h
 *
 *  Created on: 06/10/2011
 *      Author: juanmromera
 */

#ifndef COMPOSEDSTRINGKEY_H_
#define COMPOSEDSTRINGKEY_H_

#define SEPARATOR_SYMBOL '|'

#include "StringKey.h"
#include <string>
#include <list>
#include "Record.h"



class ComposedStringKey: public StringKey {
	std::list<std::string> stringList;
public:
    static const bool isVariable=false;
    ComposedStringKey(char ** input); //Load the key from input.
    ComposedStringKey(std::list<std::string> keys);
    ComposedStringKey(const ComposedStringKey &);
    void setKey(std::string);
    void setString(unsigned int index , std::string );
    void setKey(const Record::Key & rk);
    std::string getKey()const;
    std::string getString(unsigned int);

    Record::Key & operator=(const Record::Key & rk);
    ComposedStringKey & operator=(const ComposedStringKey & rk);
	virtual ~ComposedStringKey();
private:
	void updateStringKey();
};

#endif /* COMPOSEDSTRINGKEY_H_ */

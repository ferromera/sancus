/*
 * VoteCountingRecord.h
 *
 *  Created on: 06/10/2011
 *      Author: juanmromera
 */

#ifndef VOTECOUNTINGRECORD_H_
#define VOTECOUNTINGRECORD_H_

#include "../estructuras/Record.h"
#include <list>

class VoteCountingRecord: public Record {
public:
    static const bool isVariable=true;
     //It's variable, so fixed size must be a number > 0.
    static const unsigned int fixedSize=1;
    class Key: public ComposedStringKey{
    public:
        Key(char ** input):ComposedStringKey(input){}
        Key(std::list<std::string> keys):ComposedStringKey(keys){}
        ~Key(){}
    };
	VoteCountingRecord();
	virtual ~VoteCountingRecord();
};

#endif /* VOTECOUNTINGRECORD_H_ */

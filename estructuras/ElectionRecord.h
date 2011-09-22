/*
 * ElectionRecord.h
 *
 *  Created on: 21/09/2011
 *      Author: juanmromera
 */

#ifndef ELECTIONRECORD_H_
#define ELECTIONRECORD_H_

#include "Record.h"
#include "Uint16Key.h"

class ElectionRecord: public Record {
public:

    static const bool isVariable=true;
     //It's variable, so fixed size must be a number > 0.
    static const unsigned int fixedSize=1;
    class Key: public Uint16Key{
    public:
        Key(char ** input):Uint16Key(input){}
        Key(uint16_t key=0):Uint16Key(key){}
    };
	ElectionRecord();
	virtual ~ElectionRecord();
	const Record::Key & getKey()const;
    void setKey(const Record::Key & k);
    void setKey(int16_t k);
    void read(char ** input);
    void write(char ** output);
};

#endif /* ELECTIONRECORD_H_ */

/*
 * ElectionIndexRecord.h
 *
 *  Created on: 21/09/2011
 *      Author: juanmromera
 */

#ifndef ELECTIONINDEXRECORD_H_
#define ELECTIONINDEXRECORD_H_

#include "Record.h"
#include "Uint16Key.h"
#include <string>
#include "stdint.h"

class ElectionIndexRecord: public Record {
public:
    static const bool isVariable=true;
     //It's variable, so fixed size must be a number > 0.
    static const unsigned int fixedSize=1;
    class Key: public Uint16Key{
    public:
        Key(char ** input):Uint16Key(input){}
        Key(uint16_t key=0):Uint16Key(key){}
    };
private:
    unsigned long blockNumber;
    void update();
public:
    ElectionIndexRecord();
    ElectionIndexRecord(const ElectionIndexRecord &);
	virtual ~ElectionIndexRecord();
    const Record::Key & getKey()const;
    void setKey(const Record::Key & k);
    void setKey(int16_t k);
    unsigned long getBlockNumber() const;
    void setBlockNumber(unsigned long  blockNumber);
    void read(char ** input);
    void write(char ** output);
};

#endif /* ELECTIONINDEXRECORD_H_ */

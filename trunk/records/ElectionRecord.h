/*
 * ElectionRecord.h
 *
 *  Created on: 21/09/2011
 *      Author: juanmromera
 */

#ifndef ELECTIONRECORD_H_
#define ELECTIONRECORD_H_

#include "Record.h"
#include "ComposedStringKey.h"
#include "ChargeRecord.h"
#include "DistrictRecord.h"


class ElectionRecord: public Record {
private:
	DistrictRecord::Key* district;
	ChargeRecord::Key* charge;
	std::string date;
public:

    static const bool isVariable=true;
     //It's variable, so fixed size must be a number > 0.
    static const unsigned int fixedSize=1;
    class Key: public ComposedStringKey{
    public:
        Key(char ** input):ComposedStringKey(input){}
        Key(std::string key=""):ComposedStringKey(key){}
    };
    unsigned int size()const;
	ElectionRecord();
	ElectionRecord(std::string date, DistrictRecord::Key* district, ChargeRecord::Key* charge);
	virtual ~ElectionRecord();
	const ElectionRecord::Key & getKey()const;
    void setKey(const ElectionRecord::Key & k);
    void setKey(std::string k);
    void read(char ** input);
    void write(char ** output);
    void setDistrict(DistrictRecord::Key* district);
    void setCharge(ChargeRecord::Key* charge);
    void setDate(std::string date);
    DistrictRecord::Key* getDistrict();
    ChargeRecord::Key* getCharge();
    std::string getDate();

};

#endif /* ELECTIONRECORD_H_ */

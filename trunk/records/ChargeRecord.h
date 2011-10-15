/*
 * ChargeRecord.h
 *
 *  Created on: 10/10/2011
 *      Author: cecilia
 */

#ifndef CHARGERECORD_H_
#define CHARGERECORD_H_
#include <string>
#include <list>
#include "Record.h"
#include "StringKey.h"
#include <cstring>

class ChargeRecord : public Record {
private:
	std::string chargeName;
	std::list<std::string>* chargeList;

public:
	static const bool isVariable=true;
    class Key: public StringKey{
    public:
        Key(char ** input):StringKey(input){}
        Key(std::string key=" "):StringKey(key){}
    };
    unsigned int size()const;
    void setKey(const ChargeRecord::Key & k);
    void setKey(std::string k);
    void read(char ** input);
    void write(char ** output);
	ChargeRecord();
	ChargeRecord(std::string chargeName, std::list<std::string>* chargeList);
	virtual ~ChargeRecord();
	void setChargeName(std::string chargeName);
	std::string getChargeName();
	const ChargeRecord::Key & getKey()const;
	void setChargeList(std::list<std::string>* charges);
	std::list<std::string>* getChargeList();
};

#endif /* CHARGERECORD_H_ */

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
#include "../estructuras/Record.h"
#include "StringKey.h"

class ChargeRecord : public Record {
private:
	std::string chargeName;
	//std::list<ChargeRecord::Key*>* chargeList;
	std::list<ChargeRecord::Key*>* chargeList;

public:
	static const bool isVariable=true;
	static const unsigned int fixedSize=1;
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
	ChargeRecord(std::string chargeName, std::list<ChargeRecord::Key*>* chargeList);
	virtual ~ChargeRecord();
	void setChargeName(std::string chargeName);
	std::string getChargeName();
	const ChargeRecord::Key & getKey()const;
	//void setChargeList(const std::list<ChargeRecord::Key*>* charge);
	//std::list<ChargeRecord::Key*>* getChargeList();
};

#endif /* CHARGERECORD_H_ */

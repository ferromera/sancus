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
	ChargeRecord::Key* chargeFather;

public:
	static const bool isVariable=true;
    class Key: public StringKey{
    public:
        Key(char ** input):StringKey(input){}
        Key(std::string key=""):StringKey(key){}
        Key(const Key & key):StringKey(key){}
        Key& operator=(const Key& k){
        	StringKey::operator =(k);
        	return *this;
        }
    };
    unsigned int size()const;
    void setKey(const ChargeRecord::Key & k);
    void setKey(std::string k);
    void read(char ** input);
    void write(char ** output);
    ChargeRecord(char ** input);
    ChargeRecord(const ChargeRecord & rec);
    ChargeRecord(const ChargeRecord::Key &k);
    ChargeRecord(const std::string & str);
    ChargeRecord(const ChargeRecord::Key &k,const ChargeRecord::Key &kFather);
	ChargeRecord(std::string chargeName,std::string chargeFather);
	virtual ~ChargeRecord();
	void setChargeName(std::string chargeName);
	std::string getChargeName();
	void setChargeFather(std::string chargeFather);
	std::string getChargeFather();
	const ChargeRecord::Key & getKey()const;
	void setChargeFather(const ChargeRecord::Key & k);
	ChargeRecord::Key & getChargeFatherKey()const;
	bool hasFather()const;
    ChargeRecord & operator=(const ChargeRecord &rec);
};
class chargeFatherNullException : public std::exception{};

#endif /* CHARGERECORD_H_ */

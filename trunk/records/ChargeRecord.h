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
#include "DistrictRecord.h"
#include <cstring>

class ChargeRecord : public Record {
private:
	ChargeRecord::Key* chargeFather;

public:
	static const bool isVariable=true;
    class Key: public Record::Key{
    	StringKey *charge_;
    	DistrictRecord::Key * district_;
    	std::string stringKey_;
    	void updateString();

    public:
    	static const bool isString=true;
    	static const bool isVariable=true;
        Key(char ** input);
        Key(const std::string &,const std::string&);
        Key(const std::string &,const DistrictRecord::Key &);
        Key();
        Key(const Key & key);
        void setHighValue();
        Key & operator=(const Key & k);
        const std::string & getString()const{return getKey();}
        void setKey(const std::string &, const DistrictRecord::Key&);
        const std::string & getKey()const;
        void setDistrict(const DistrictRecord::Key &);
        void setCharge(const std::string &);
        const DistrictRecord::Key& getDistrict()const;
        const std::string & getCharge()const;
        void read(char ** input);
        void write(char ** output)const;
        unsigned int size()const;
        bool operator <(const Record::Key &rk)const;
        bool operator <=(const Record::Key &rk)const;
        bool operator ==(const Record::Key &rk)const;
        bool operator >(const Record::Key & rk)const;
        bool operator >=(const Record::Key & rk)const;
        bool operator !=(const Record::Key & rk)const;
        ~Key();
    };

    ChargeRecord(char ** input);
    ChargeRecord(const ChargeRecord & rec);
    ChargeRecord(const ChargeRecord::Key &k);
    ChargeRecord(const std::string & ,const DistrictRecord::Key& );
    ChargeRecord(const std::string & ,const std::string & );
    ChargeRecord(const ChargeRecord::Key &k,const ChargeRecord::Key &kFather);
	ChargeRecord(const std::string & charge,const DistrictRecord::Key& district,const std::string & chargeFather,const DistrictRecord::Key& districtFather);
	ChargeRecord(const std::string & charge,const std::string & district,const std::string & chargeFather,const std::string & districtFather);
	unsigned int size()const;
	void setKey(const ChargeRecord::Key & k);
	void setKey(const std::string & str,const DistrictRecord::Key&);
	void read(char ** input);
	void write(char ** output);
	void setChargeName(const std::string & chargeName);
	std::string getChargeName();
	void setChargeFather(const std::string & chargeFather,const std::string &);
	void setChargeFather(const std::string & chargeFather,const DistrictRecord::Key&);
	void setChargeFather(const ChargeRecord::Key & k);
	const ChargeRecord::Key & getChargeFather()const;
	const ChargeRecord::Key & getKey()const;
	bool hasFather()const;
	void setDistrict(const std::string & district);
	void setDistrict(const DistrictRecord::Key & district);
	const DistrictRecord::Key & getDistrict()const;
    ChargeRecord & operator=(const ChargeRecord &rec);
    virtual ~ChargeRecord();
};
class chargeFatherNullException : public std::exception{};

#endif /* CHARGERECORD_H_ */

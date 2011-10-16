/*
 * ElectionRecord.h
 *
 *  Created on: 21/09/2011
 *      Author: juanmromera
 */

#ifndef ELECTIONRECORD_H_
#define ELECTIONRECORD_H_

#include "Record.h"
#include "ChargeRecord.h"
#include "DistrictRecord.h"


class ElectionRecord: public Record {
private:
	DistrictRecord::Key* district;
public:

    static const bool isVariable=true;
    class Key : public Record::Key{
    private:
    	ChargeRecord::Key* charge;
    	unsigned int  date;
    	std::string stringKey_;
    	void updateString();
    public:
    	Key();
        Key(unsigned int date,const ChargeRecord::Key &);
        Key(char ** input);
        Key(const Key & k);
        Key & operator=(const Key & k);
        const std::string & getString()const{return getKey();}
        const std::string & getKey()const;
        void setDate(unsigned int);
        void setCharge(const ChargeRecord::Key&);
        const unsigned int getDate()const;
        const ChargeRecord::Key& getCharge()const;
       	void read(char ** input);
       	void write(char ** output)const;
        unsigned int size()const;
        bool operator <(const Record::Key &r)const;
        bool operator ==(const Record::Key &r)const;
        bool operator <=(const Record::Key & r)const;
        bool operator >(const Record::Key &r)const;
        bool operator !=(const Record::Key &r)const;
        bool operator >=(const Record::Key & r)const;
        ChargeRecord::Key & operator=(const ChargeRecord::Key & rk);
        ~Key();

    };
    unsigned int size()const;
	ElectionRecord(const ElectionRecord & rec);
	ElectionRecord(const ElectionRecord::Key & k);
	ElectionRecord(unsigned int date, const ChargeRecord::Key& charge, const DistrictRecord::Key& district);
	const ElectionRecord::Key & getKey()const;
    void setKey(const ElectionRecord::Key & k);
    void setKey(unsigned int date, const ChargeRecord::Key& charge);
    void read(char ** input);
    void write(char ** output);
    void setDistrict(const DistrictRecord::Key& district);
    void setCharge(const ChargeRecord::Key& charge);
    void setDate(unsigned int date);
    const DistrictRecord::Key& getDistrict()const;
    const ChargeRecord::Key& getCharge()const;
    unsigned int getDate()const;
    virtual ~ElectionRecord();

};

#endif /* ELECTIONRECORD_H_ */

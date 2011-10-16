/*
 * District.h
 *
 *  Created on: 08/10/2011
 *      Author: cecilia
 */

#ifndef DISTRICTRECORD_H_
#define DISTRICTRECORD_H_

#include <string>
#include <list>
#include "Record.h"
#include "StringKey.h"


class DistrictRecord : public Record {
private:
	DistrictRecord::Key* districtFather;

public:
	static const bool isVariable=true;
    class Key: public StringKey{
    public:
        Key(char ** input):StringKey(input){}
        Key(std::string key=" "):StringKey(key){}
    };
    unsigned int size()const;
    void setKey(const DistrictRecord::Key & k);
    void setKey(std::string k);
    void read(char ** input);
    void write(char ** output);
    DistrictRecord(const DistrictRecord::Key &k);
    DistrictRecord(const DistrictRecord::Key &k,const DistrictRecord::Key &kFather);
	DistrictRecord(std::string district,std::string father);
	virtual ~DistrictRecord();
	void setDistrictName(std::string district);
	std::string getDistrictName();
	void setFatherName(std::string father);
	std::string getFatherName();
	const DistrictRecord::Key & getKey()const;
	void setFather(const DistrictRecord::Key & k);
	DistrictRecord::Key & getFather()const;
	bool hasFather()const;
    bool operator <(const Record::Key &rk)const;
    bool operator <=(const Record::Key &rk)const;
    bool operator ==(const Record::Key &rk)const;
    bool operator >(const Record::Key & rk)const;
    bool operator >=(const Record::Key & rk)const;
    bool operator !=(const Record::Key & rk)const;
    DistrictRecord & operator=(const DistrictRecord &rec);
};
class districtFatherNullException : public std::exception{};

#endif /* DISTRICTRECORD_H_ */
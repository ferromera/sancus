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
	std::string name;
	std::list<DistrictRecord::Key*>* districtList;

public:
	static const bool isVariable=true;
	static const unsigned int fixedSize=1;
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
	DistrictRecord();
	DistrictRecord(std::string name, std::list<DistrictRecord::Key*>* districtList);
	virtual ~DistrictRecord();
	void setName(std::string name);
	std::string getName();
	const DistrictRecord::Key & getKey()const;
	void setDistrictList(const std::list<DistrictRecord::Key*>* district);
	std::list<DistrictRecord::Key*>* getDistrictList();
};

#endif /* DISTRICTRECORD_H_ */

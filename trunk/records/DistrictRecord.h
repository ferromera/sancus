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

class DistrictRecord: public Record {
public:
	class Key: public StringKey {
	public:
		Key(char ** input) :StringKey(input) {
		}
		Key(const std::string & key = "") :StringKey(key) {
		}
		Key(const Key & key) :StringKey(key) {
		}
		Key& operator=(const Key & key) {
			StringKey::operator=(key);
			return *this;
		}
	};

private:
	DistrictRecord::Key* districtFather;

public:
	static const bool isVariable = true;

	unsigned int size() const;
	void setKey(const DistrictRecord::Key & k);
	void setKey(std::string k);
	void read(char ** input);
	void write(char ** output);
	DistrictRecord():Record(){}
	DistrictRecord(char ** input);
	DistrictRecord(const DistrictRecord &k);
	DistrictRecord(const DistrictRecord::Key &k);
	DistrictRecord(const std::string & district);
	DistrictRecord(const DistrictRecord::Key &k, const DistrictRecord::Key &kFather);
	DistrictRecord(const std::string & district, const std::string & father);
	virtual ~DistrictRecord();
	void setDistrictName(std::string district);
	std::string getDistrictName();
	void setFatherName(std::string father);
	std::string getFatherName();
	const DistrictRecord::Key & getKey() const;
	void setFather(const DistrictRecord::Key & k);
	DistrictRecord::Key & getFather() const;
	bool hasFather() const;
	//ESTOS OPERADOS YA SE SOBRECARGAN EN RECORD
	// bool operator <(const Record::Key &rk)const;
	// bool operator <=(const Record::Key &rk)const;
	// bool operator ==(const Record::Key &rk)const;
	// bool operator >(const Record::Key & rk)const;
	// bool operator >=(const Record::Key & rk)const;
	// bool operator !=(const Record::Key & rk)const;
	DistrictRecord & operator=(const DistrictRecord &rec);
};
class districtFatherNullException: public std::exception {
};

#endif /* DISTRICTRECORD_H_ */

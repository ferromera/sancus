/*
 * VoteCountingRecord.h
 *
 *  Created on: 06/10/2011
 *      Author: juanmromera
 */

#ifndef VOTECOUNTINGRECORD_H_
#define VOTECOUNTINGRECORD_H_

#include "Record.h"
#include "ListRecord.h"
#include "DistrictRecord.h"
#include "ElectionRecord.h"

#include <list>

class VoteCountingRecord: public Record {
	unsigned int count_;

public:
    static const bool isVariable=true;

    class Key: public Record::Key{
    	ListRecord::Key * list_;
    	DistrictRecord::Key * district_;
    	ElectionRecord::Key * election_;
    	std::string stringKey_;
    	void updateString();
    public:
    	static const bool isVariable=true;
    	static const bool isString=true;
    	Key();
        Key(char ** input);
        Key(const Key & k);
        Key(const ListRecord::Key & , const DistrictRecord::Key & , const ElectionRecord::Key &);
        void setHighValue();
        Key & operator=(const Key & k);
        const std::string & getString(){return getKey();}
        void setKey(const ListRecord::Key & , const DistrictRecord::Key & , const ElectionRecord::Key &);
        void setKey(const Key & k);
        const std::string & getKey()const;
        void setList(const ListRecord::Key &);
        void setDistrict(const DistrictRecord::Key&);
        void setElection(const ElectionRecord::Key&);
        const ListRecord::Key & getList()const;
        const ElectionRecord::Key& getElection()const;
        const DistrictRecord::Key& getDistrict()const;
       	void read(char ** input);
       	void write(char ** output)const;
        unsigned int size()const;
        bool operator <(const Record::Key &r)const;
        bool operator <=(const Record::Key &r)const;
        bool operator ==(const Record::Key &r)const;
        bool operator >(const Record::Key & r)const;
        bool operator >=(const Record::Key & r)const;
        bool operator !=(const Record::Key & r)const;
        ~Key();
    };

	VoteCountingRecord(const Key &, unsigned int);
	VoteCountingRecord(const ListRecord::Key & , const DistrictRecord::Key & , const ElectionRecord::Key & , unsigned int);
	VoteCountingRecord(const VoteCountingRecord &);
	VoteCountingRecord(const VoteCountingRecord::Key &);
	const ListRecord::Key & getList()const;
	const ElectionRecord::Key& getElection()const;
	const DistrictRecord::Key& getDistrict()const;
	unsigned int getCount()const;
	void setList(const ListRecord::Key &);
	void setDistrict(const DistrictRecord::Key&);
	void setElection(const ElectionRecord::Key&);
	void setCount(unsigned int);
	void setKey(const Key &);
	void setKey(const ListRecord::Key & , const DistrictRecord::Key & , const ElectionRecord::Key & );
	const Key & getKey()const;
	void read(char ** input);
	void write(char ** output);
	unsigned int size()const;

	VoteCountingRecord & operator = (const VoteCountingRecord &);

	virtual ~VoteCountingRecord(){}
};

#endif /* VOTECOUNTINGRECORD_H_ */

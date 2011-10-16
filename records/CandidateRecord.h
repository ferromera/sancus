/*
 * CandidateRecord.h
 *
 *  Created on: 11/10/2011
 *      Author: cecilia
 */

#ifndef CANDIDATERECORD_H_
#define CANDIDATERECORD_H_
#include <string>
#include <list>
#include "Record.h"
#include "VoterRecord.h"
#include "ChargeRecord.h"
#include "ListRecord.h"


class CandidateRecord : public Record {
	VoterRecord::Key* voterKey;
public:
	static const bool isVariable=true;

    class Key: public Record::Key{
    	ListRecord::Key* list;
    	ChargeRecord::Key* charge;
    	std::string stringKey;
    	void updateString();
    public:
    	static const bool isString=true;
    	static const bool isVariable=true;
        Key(char ** input);
        Key();
        Key(const Key &);
        Key(const ListRecord::Key &, const ChargeRecord::Key&);
        Key & operator=(const Key & k);
        const std::string & getString(){return getKey();}
        void setKey(const ListRecord::Key &, const ChargeRecord::Key&);
        const std::string & getKey()const;
        void setList(const ListRecord::Key &);
        void setCharge(const ChargeRecord::Key &);
        const ListRecord::Key& getList()const;
        const ChargeRecord::Key & getCharge()const;
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
    CandidateRecord(const CandidateRecord::Key& key);
    CandidateRecord(const CandidateRecord&);
    CandidateRecord(const ListRecord::Key& listName,const ChargeRecord::Key& chargeName,const VoterRecord::Key& dni);
    unsigned int size()const;
    void setKey(const CandidateRecord::Key & k);
    void setKey(const ListRecord::Key& listName,const ChargeRecord::Key& chargeName);
    void read(char ** input);
    void write(char ** output);
	const CandidateRecord::Key & getKey()const;
	void setCharge(const ChargeRecord::Key& chargeName);
	const ChargeRecord::Key& getCharge()const;
	void setVoter(const VoterRecord::Key& dni);
	const VoterRecord::Key& getVoter()const;
	void setList(const ListRecord::Key& listName);
	const ListRecord::Key& getList()const;
	CandidateRecord & operator=(const CandidateRecord &);
	virtual ~CandidateRecord();
};

#endif /* CANDIDATERECORD_H_ */

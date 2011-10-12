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
#include "ComposedStringKey.h"
#include "VoterRecord.h"
#include "ChargeRecord.h"
#include "ListRecord.h"


class CandidateRecord : public Record {
private:
	ChargeRecord::Key* chargeName;
	VoterRecord::Key* voterName;
	ListRecord::Key* listName;

public:
	static const bool isVariable=true;
	static const unsigned int fixedSize=1;
    class Key: public ComposedStringKey{
    public:
        Key(char ** input):ComposedStringKey(input){}
        Key(std::string key=" "):ComposedStringKey(key){}
    };
    unsigned int size()const;
    void setKey(const CandidateRecord::Key & k);
    void setKey(std::string k);
    void read(char ** input);
    void write(char ** output);
    CandidateRecord();
    CandidateRecord(ChargeRecord::Key* chargeName, VoterRecord::Key* votername, ListRecord::Key* listName);
	virtual ~CandidateRecord();
	const CandidateRecord::Key & getKey()const;
	void setChargeName(ChargeRecord::Key* chargeName);
	ChargeRecord::Key* getChargeName();
	void setVoterName(VoterRecord::Key* voterName);
	VoterRecord::Key* getVoterName();
	void setListName(const ListRecord::Key* listName);
	ListRecord::Key* getListName();
}

#endif /* CANDIDATERECORD_H_ */

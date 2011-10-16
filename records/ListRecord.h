/*
 * ListRecord.h
 *
 *  Created on: 11/10/2011
 *      Author: cecilia
 */

#ifndef LISTRECORD_H_
#define LISTRECORD_H_
#include <string>
#include <list>
#include "Record.h"
#include "ElectionRecord.h"

class ListRecord : public Record {

public:
	static const bool isVariable=true;
    class Key: public Record::Key{
    	ElectionRecord::Key * election;
    	std::string name;
    	std::string stringKey_;
    	void updateString();
    public:
    	static const bool isVariable=true;
    	static const bool isString=true;

        Key();
        Key(char ** input);
        Key(const Key & k);
        Key(const ElectionRecord::Key & , const std::string & );
        Key & operator=(const Key & k);
        const std::string & getString(){return getKey();}
        void setKey(const ElectionRecord::Key & , const std::string  &);
        void setKey(const Key & k);
        const std::string & getKey()const;
        void setElection(const ElectionRecord::Key &);
        void setName(const std::string &);
        const ElectionRecord::Key& getElection()const;
        const std::string & getName()const;
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
    ListRecord(const ListRecord::Key &);
    ListRecord(const ListRecord &);
    ListRecord(const ElectionRecord::Key& election,const std::string & listName);
    unsigned int size()const;
    void setKey(const ListRecord::Key & k);
    void setKey(const ElectionRecord::Key& election,const std::string & listName);
    const ListRecord::Key & getKey()const;
    void read(char ** input);
    void write(char ** output);
	void setName(const std::string & listName);
	const std::string & getName()const;
	void setElection (const ElectionRecord::Key & election);
	const ElectionRecord::Key & getElection()const;
	ListRecord & operator=(const ListRecord &rec);

	virtual ~ListRecord();
};


#endif /* LISTRECORD_H_ */

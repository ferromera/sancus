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
#include "ComposedStringKey.h"
#include "ElectionRecord.h"

class ListRecord : public Record {
private:
	std::string listName;
	ElectionRecord::Key* election;

public:
	static const bool isVariable=true;
	static const unsigned int fixedSize=1;
    class Key: public ComposedStringKey{
    public:
        Key(char ** input):ComposedStringKey(input){}
        Key(std::string key=" "):ComposedStringKey(key){}
    };
    unsigned int size()const;
    void setKey(const ListRecord::Key & k);
    void setKey(std::string k);
    void read(char ** input);
    void write(char ** output);
	ListRecord();
	ListRecord(std::string listName, ElectionRecord* election);
	virtual ~ListRecord();
	void setListName(std::string listName);
	std::string getListName();
	const ListRecord::Key & getKey()const;
	void setElection (ElectionRecord* election);
	ElectionRecord* getElection();
}


#endif /* LISTRECORD_H_ */

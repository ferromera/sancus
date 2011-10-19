/*
 * AdministratorRecord.h
 *
 *  Created on: 17/10/2011
 *      Author: cecilia
 */

#ifndef ADMINISTRATORRECORD_H_
#define ADMINISTRATORRECORD_H_

#include <cstring>
#include <list>
#include "Record.h"
#include "../estructuras/Uint32Key.h"

class AdministratorRecord : public Record {
private:
	uint32_t user;
	std::string password;
public:
	 static const bool isVariable=true;
	 static const bool isString=false;
	 class Key: public Uint32Key {
	  public:
		  Key(char ** input):Uint32Key(input){}
		  Key(uint32_t key=0):Uint32Key(key){}
		  unsigned int getUint() const {return getKey();}

	 };

	AdministratorRecord(const AdministratorRecord &);
	AdministratorRecord(char** input);
	AdministratorRecord(const uint32_t & user, const std::string & password);

	const AdministratorRecord::Key & getKey()const;
	void setKey(const AdministratorRecord::Key & k);
	void setKey(const uint32_t & k);
	void read(char ** input);
	void write(char ** output);
	unsigned int size()const;

	void setUser(const uint32_t &);
	void setPassword (const std::string &);
	const uint32_t & getUser () const;
	const std::string & getPassword() const;

	const AdministratorRecord & operator=(const AdministratorRecord & k);
	~AdministratorRecord();

};


#endif /* ADMINISTRATORRECORD_H_ */

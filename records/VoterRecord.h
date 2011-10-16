/*
 * VoterRecord.h
 *
 *  Created on: 09/10/2011
 *      Author: cecilia
 */

#ifndef VOTERRECORD_H_
#define VOTERRECORD_H_

#include <string>
#include <list>
#include "DistrictRecord.h"
#include "ElectionRecord.h"
#include "Record.h"
#include "../estructuras/Uint32Key.h"

class VoterRecord : public Record {
private:
	std::string voterName;
	uint32_t dni;
	std::string address;
	std::string voterKey;
	DistrictRecord::Key* voterDistrict;
	std::list<ElectionRecord::Key>* electionList;
	//VoterRecord();
	void update();
public:
	 static const bool isVariable=true;
	 class Key: public Uint32Key {
	  public:
	      Key(char ** input):Uint32Key(input){}
	      Key(uint32_t key=0):Uint32Key(key){}
	      static const bool isString() { return false;}

	 };
	const VoterRecord::Key & getKey()const;
	void setKey(const VoterRecord::Key & k);
	void setKey(int32_t k);
	void read(char ** input);
    void write(char ** output);
	unsigned int size()const;

	VoterRecord(char** input);
	VoterRecord(const VoterRecord &);
	VoterRecord(const std::string & name, const uint32_t & dni, const std::string & address, const std::string & voterKey, const DistrictRecord::Key &  voterDistrict,  const std::list<ElectionRecord::Key> & el);

	void setVoterName(const std::string & name);
	void setDni(const uint32_t & dni);
	void setAddress(const std::string & address);
	void setVoterKey (const std::string & voterKey);
	void setVoterDistrict (const DistrictRecord::Key & voterDistrict);
	const std::string & getVoterName() const;
	const uint32_t & getDni() const;
	const std::string & getAddress() const;
	const std::string & getVoterKey() const;
	const DistrictRecord::Key & getVoterDistrict() const;
	void setElectionList(const std::list<ElectionRecord::Key> & el);
	const std::list<ElectionRecord::Key> & getElectionList() const;

	bool addElection (const ElectionRecord::Key & election);
	bool removeElection(const ElectionRecord::Key & election);
	const VoterRecord & operator=(const VoterRecord & rk);
	~VoterRecord();

};

#endif /* VOTERRECORD_H_ */
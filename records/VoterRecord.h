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
#include "Uint16Key.h"

class VoterRecord : public Record{
private:
	std::string voterName;
	Uint16Key dni;
	std::string address;
	std::string voterKey;
	DistrictRecord* voterDistrict;
	std::list<ElectionRecord::Key*>* electionList;
public:
	 static const bool isVariable=true;
	 static const unsigned int fixedSize=1;
	 class Key: public Uint16Key {
	  public:
	      Key(char ** input):Uint16Key(input){}
	      Key(uint16_t key=0):Uint16Key(key){}
	 };
	const VoterRecord::Key & getKey()const;
	void setKey(const VoterRecord::Key & k);
	void setKey(int16_t k);
	void read(char ** input);
    void write(char ** output);
	unsigned int size()const;
	VoterRecord();
	VoterRecord(std::string name, Uint16Key dni, std::string address, std::string voterKey, DistrictRecord* voterDistrict);
	void setVoterName(std::string name);
	void setDni(Uint16Key dni);
	void setAddress(std::string address);
	void setVoterKey (std::string voterKey);
	void setVoterDistrict (DistrictRecord* voterDistrict);
	std::string getVoterName();
	Uint16Key getDni();
	std::string getAddress();
	std::string getVoterKey();
	DistrictRecord* getVoterDistrict();
	void setElectionList(const std::list<ElectionRecord::Key*>* el);
	std::list<ElectionRecord::Key*>* getElectionList();
	~VoterRecord();

};

#endif /* VOTERRECORD_H_ */

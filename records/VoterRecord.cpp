/*
 * VoterRecord.cpp
 *
 *  Created on: 09/10/2011
 *      Author: cecilia
 */

#include "VoterRecord.h"

VoterRecord::VoterRecord() {
	this->voterName.clear();
	this->dni = NULL;
	this->address.clear();
	this->voterKey.clear();
	this->voterDistrict = NULL;
	electionList = NULL;
}

VoterRecord::VoterRecord(std::string name, Uint16Key dni, std::string address, std::string voterKey,DistrictRecord* voterDistrict, std::list<Electionrecord*>* list) {
	this->voterName = name;
	this->dni = dni;
	this->address = address;
	this->voterKey = voterKey;
	this->voterDistrict = voterDistrict;
	this->electionList = list;
}

VoterRecord::~VoterRecord(){
	delete this->voterDistrict;
	std::list<ElectionRecord::Key*>::iterator it;
	if (this->electionList != NULL) {
      for(it = this->electionList->begin(); it != this->electionList->end(); it++){
          ElectionRecord::Key* election;
          election = *it;
          delete election;
      }
    }
    delete this->electionList;
}

void VoterRecord::setVoterName(std::string name) {
		this->voterName = name;
}

void VoterRecord::setDni(Uint16Key dni) {
		this->dni = dni;
}

void VoterRecord::setAddress(std::string address) {
		this->address = address;
}

void VoterRecord::setVoterKey(std::string voterKey) {
		this->voterKey= voterKey;
}

void VoterRecord::setVoterDistrict(DistrictRecord* voterDistrict) {
		this->voterDistrict = voterDistrict;
}
std::string VoterRecord::getVoterName() {
	return this->voterName;
}

Uint16Key VoterRecord::getDni() {
	return this->dni;
}

std::string VoterRecord::getAddress(){
	return this->address;
}

DistrictRecord* VoterRecord::getVoterDistrict() {
	return this->voterDistrict;
}

std::string VoterRecord::getVoterKey() {
	return this->voterKey;
}

void VoterRecord::size(){
	return voterName.size()+1+ sizeof(dni) + voterKey.size()+1 + address.size()+1+this->voterDistrict->size()+this->electionList->size();
}

void VoterRecord::getElectionList(){
	return this->electionList;
}
void VoterRecord::setElectionList(const std::list<ElectionRecord::Key*>* el){
	this->electionList = el;
}

void VoterRecord::read(char** in){
//todo
}
void VoterRecord::write(char** out){
//todo
}

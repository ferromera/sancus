/*
 * VoterRecord.cpp
 *
 *  Created on: 09/10/2011
 *      Author: cecilia
 */

#include "VoterRecord.h"

VoterRecord::VoterRecord(const VoterRecord & vr) {
	this->key_ = NULL;
	this->setKey(vr.dni);

	this->voterName = vr.voterName;
	this->address = vr.address;
	this->voterKey = vr.voterKey;
	this->voterDistrict = vr.voterDistrict;
	electionList = vr.electionList;
	//key_ = new Key((uint32_t) dni);
}

//VoterRecord::VoterRecord() {
//	this->key_ = NULL;
//	this->voterName = "";
//	this->dni = NULL;
//	this->address = "";
//	this->voterKey = "" ;
//	this->voterDistrict = "";
//	electionList = NULL;
//}

VoterRecord::VoterRecord(char** input) {
	this->key_ = NULL;

	 key_= new Key();
	 read(input);
}

VoterRecord::VoterRecord(const std::string & name, const uint32_t & dni, const std::string & address, const std::string & voterKey, const DistrictRecord::Key & voterDistrict, const std::list<ElectionRecord::Key> & el) {
	this->key_ = NULL;

	this->setKey(dni);

	this->voterName = name;
	this->address = address;
	this->voterKey = voterKey;
	this->voterDistrict = new DistrictRecord::Key(voterDistrict);

	this->electionList = new std::list<ElectionRecord::Key>();
	std::list<ElectionRecord::Key>::const_iterator it2;
	for(it2 = el.begin(); it2 != el.end(); it2++){
	  // ElectionRecord::Key* election = new ElectionRecord::Key(**it2);
	   this->electionList->push_back(it2);
	}
}

VoterRecord::~VoterRecord(){
	if(this->voterDistrict != NULL) {
		delete this->voterDistrict;
	}
	std::list<ElectionRecord::Key>::iterator it;
	if (this->electionList != NULL) {
      for(it = this->electionList->begin(); it != this->electionList->end(); it++){
          delete *it;
      }
    }

    delete this->electionList;
}

void VoterRecord::setVoterName(const std::string & name) {
	this->voterName = name;
}

//void VoterRecord::setDni(const Uint32Key & dni) {
//		this->dni = this->setKey();
//}

void VoterRecord::setAddress(const std::string & address) {
	this->address = address;
}

void VoterRecord::setVoterKey(const std::string & voterKey) {
	this->voterKey= voterKey;
}

void VoterRecord::setVoterDistrict(const DistrictRecord::Key & voterDistrict) {
	this->voterDistrict = new DistrictRecord::Key(voterDistrict);
}

const std::string & VoterRecord::getVoterName() const {
	return this->voterName;
}

const uint32_t & VoterRecord::getDni() const {
	return ((Key*)key_)->getKey();
}

const std::string & VoterRecord::getAddress() const {
	return this->address;
}

const DistrictRecord::Key & VoterRecord::getVoterDistrict() const {
	return *this->voterDistrict;
}

const std::string & VoterRecord::getVoterKey() const {
	return this->voterKey;
}

unsigned int VoterRecord::size() const {
	return voterName.size()+1+ sizeof(dni) + voterKey.size()+1 + address.size()+1+this->voterDistrict->size()+this->electionList->size();
}

//const std::list<ElectionRecord::Key*> & VoterRecord::getElectionList() const{
//	return this->electionList;
//}

void VoterRecord::setElectionList(const std::list<ElectionRecord::Key> & el){
//	std::list<ElectionRecord::Key>::iterator it;
//
//	if (this->electionList != NULL) {
//	  for(it = this->electionList->begin(); it != this->electionList->end(); it++){
//		  ElectionRecord::Key election = it;
//		  delete election;
//	  }
//	}
//	delete this->electionList;
//
//	this->electionList = new std::list<ElectionRecord::Key>();
//	std::list<ElectionRecord::Key>::const_iterator it2;
//    for(it2 = el.begin(); it2 != el.end(); it2++){
//	   ElectionRecord::Key election = *it2;
//	   this->electionList->push_back(election);
//    }
}

void VoterRecord::read(char** in){
	uint8_t nameSize;
	memcpy(&nameSize,*in,1);
	(*in)++;
	char * c_str = new char[nameSize+1];
	memcpy(c_str,*in,nameSize);
	(*in)+=nameSize;
	c_str[nameSize]='\0';
	this->voterName = c_str;

	key_->read(in);
	memcpy(&dni,*in,4);
	(*in)+=4;

	memcpy(&nameSize,*in,1);
	(*in)++;
	char * c_str=new char[nameSize+1];
	memcpy(c_str,*in,nameSize);
	(*in)+=nameSize;
	c_str[nameSize] = '\0';
	this->address = c_str;

	this->voterDistrict->read(in);
	//this->electionList->read(in);

	uint8_t nameSize;
	memcpy(&nameSize,*in,1);
	(*in)++;

	delete c_str;
}
void VoterRecord::write(char** out){
	uint8_t nameSize;
	nameSize = this->voterName.size();
	key_->write(out);
	memcpy(*out,&voterName,nameSize);
	(*out)+=nameSize;

	key_->write(out);
	memcpy(*out,&dni,4);
	(*out)+=4;

	nameSize = this->address.size();
	key_->write(out);
	memcpy(*out,&address,nameSize);
	(*out)+=nameSize;

	nameSize = this->voterKey.size();
	key_->write(out);
	memcpy(*out,&voterKey,nameSize);
	(*out)+=nameSize;

	this->voterDistrict->read(in);
	(*out) += this->voterDistrict->size();



}
const VoterRecord::Key & VoterRecord::getKey()const {
	return * ((VoterRecord::Key *) key_ );
}

void VoterRecord::setKey(int32_t k){
	if(this->key_ != NULL) {
		delete this->key_;
	}

    this->key_= new Key(k);
    this->dni = k;
}

void VoterRecord::setKey(const VoterRecord::Key & k){

	if(this->key_ != NULL) {
		delete key_;
	}

    this->key_= new Key(k);
    this->dni = k.getKey();
}

bool VoterRecord::addElection (const ElectionRecord::Key & election){
	//this->electionList

	return true;
}

bool VoterRecord::removeElection(const ElectionRecord::Key & election){
	//this->electionList->remove(&election);
	// cambiar por recorrer la lista y eliminar el que es igual

	return true;
}

const VoterRecord & VoterRecord::operator=(const VoterRecord & rk){
	if(this==&rk)
		return *this;
	voterName = rk.voterName;
	address = rk.address;
	voterKey = rk.voterKey;
	voterDistrict = rk.voterDistrict;
	dni = rk.dni;
	std::list<ElectionRecord::Key>::iterator it;
	if (this->electionList != NULL) {
	  for(it = this->electionList->begin(); it != this->electionList->end(); it++){
		  //ElectionRecord::Key* election = *it;
		  ElectionRecord::Key election = it;
		  delete election;
	  }
	}
	delete this->electionList;

	this->electionList = new std::list<ElectionRecord::Key>();
	std::list<ElectionRecord::Key>::const_iterator it2;
	for(it2 = el.begin(); it2 != el.end(); it2++){
	   //ElectionRecord::Key election = new ElectionRecord::Key(**it2);
	   this->electionList->push_back(it2);
	}
	key_=new Key((uint32_t) dni);
	return *this;
}


/*
 * VoterRecord.cpp
 *
 *  Created on: 09/10/2011
 *      Author: cecilia
 */

#include "VoterRecord.h"
#include <iostream>

VoterRecord::VoterRecord(const VoterRecord & vr) {
	this->key_ = NULL;
	this->setKey(vr.dni);

	this->voterName = vr.voterName;
	this->address = vr.address;
	this->voterKey = vr.voterKey;
	this->voterDistrict = new DistrictRecord::Key(vr.getDistrict());
	electionList = new std::list<ElectionRecord::Key>(vr.getElectionList());
}

VoterRecord::VoterRecord() {
	this->key_ = NULL;
	this->voterName = "";
	this->dni = 0;
	this->address = "";
	this->voterKey = "";
	this->voterDistrict = NULL;
	electionList = NULL;
}

VoterRecord::VoterRecord(char** input) {
	this->key_ = NULL;

	key_ = new Key();
	voterDistrict = new DistrictRecord::Key();
	electionList = new std::list<ElectionRecord::Key>();
	read(input);
}

VoterRecord::VoterRecord(const std::string & name, uint32_t dni, const std::string & address,
		const std::string & voterKey, const DistrictRecord::Key & voterDistrict,
		const std::list<ElectionRecord::Key> & el) {
	this->key_ = NULL;
	this->setKey(dni);

	this->voterName = name;
	this->address = address;
	this->voterKey = voterKey;
	this->voterDistrict = new DistrictRecord::Key(voterDistrict);

	this->electionList = new std::list<ElectionRecord::Key>();
	std::list<ElectionRecord::Key>::const_iterator it2;
	for (it2 = el.begin(); it2 != el.end(); it2++) {
		this->electionList->push_back(*it2);
	}
}

VoterRecord::~VoterRecord() {

	delete this->voterDistrict;

	delete this->electionList;
}

void VoterRecord::setName(const std::string & name) {
	this->voterName = name;
}

void VoterRecord::setDni(uint32_t dni) {
	this->key_ = NULL;
	this->setKey(dni);
	this->dni = dni;
}

void VoterRecord::setAddress(const std::string & address) {
	this->address = address;
}

void VoterRecord::setKey(const std::string & voterKey) {
	this->voterKey = voterKey;
}

void VoterRecord::setDistrict(const DistrictRecord::Key & voterDistrict) {
	this->voterDistrict = new DistrictRecord::Key(voterDistrict);
}

const std::string & VoterRecord::getName() const {
	return this->voterName;
}

uint32_t VoterRecord::getDni() const {
	return ((Key*) key_)->getKey();
}

const std::string & VoterRecord::getAddress() const {
	return this->address;
}

const DistrictRecord::Key & VoterRecord::getDistrict() const {
	return *this->voterDistrict;
}

const std::string & VoterRecord::getUserKey() const {
	return this->voterKey;
}

unsigned int VoterRecord::size() const {
	if (electionList == NULL) {
		return key_->size() + voterName.size() + 1 + sizeof(dni) + voterKey.size() + 1 + address.size() + 1
				+ this->voterDistrict->size();
	}
	uint32_t size = key_->size() + voterName.size() + 1 + sizeof(dni) + voterKey.size() + 1 + address.size() + 1
			+ this->voterDistrict->size();
	std::list<ElectionRecord::Key>::const_iterator it;
	for (it = this->electionList->begin(); it != this->electionList->end(); it++)
		size += it->size();
	return size;
}

const std::list<ElectionRecord::Key> & VoterRecord::getElectionList() const {
	return *this->electionList;
}

void VoterRecord::setElectionList(const std::list<ElectionRecord::Key> & el) {
	this->electionList = new std::list<ElectionRecord::Key>();
	std::list<ElectionRecord::Key>::const_iterator it;
	for (it = el.begin(); it != el.end(); it++) {
		this->electionList->push_back(*it);
	}
}

void VoterRecord::read(char** in) {
	uint8_t nameSize;

	key_->read(in);

	memcpy(&nameSize, *in, 1);
	(*in)++;
	char * c_str = new char[nameSize + 1];
	memcpy(c_str, *in, nameSize);
	(*in) += nameSize;
	c_str[nameSize] = '\0';
	this->voterName = c_str;

	memcpy(&dni, *in, 4);
	(*in) += 4;

	memcpy(&nameSize, *in, 1);
	(*in)++;
	char * c_str_2 = new char[nameSize + 1];
	memcpy(c_str_2, *in, nameSize);
	(*in) += nameSize;
	c_str_2[nameSize] = '\0';
	this->address = c_str_2;

	memcpy(&nameSize, *in, 1);
	(*in)++;
	char * c_str_3 = new char[nameSize + 1];
	memcpy(c_str_3, *in, nameSize);
	(*in) += nameSize;
	c_str_3[nameSize] = '\0';
	this->voterKey = c_str_3;

	this->voterDistrict->read(in);

	memcpy(&nameSize, *in, 1);
	(*in) += 1;
	for (int i = 0; i < nameSize; i++) {
		ElectionRecord::Key e;
		e.read(in);
		this->electionList->push_back(e);
	}

	delete c_str;
}

void VoterRecord::write(char** out) {
	uint8_t nameSize;
	key_->write(out);

	nameSize = voterName.size();
	memcpy(*out, &nameSize, 1);
	(*out) += 1;

	memcpy(*out, voterName.c_str(), nameSize);
	(*out) += nameSize;

	memcpy(*out, &dni, 4);
	(*out) += 4;

	nameSize = this->address.size();
	memcpy(*out, &nameSize, 1);
	(*out) += 1;

	memcpy(*out, address.c_str(), nameSize);
	(*out) += nameSize;

	nameSize = this->voterKey.size();
	memcpy(*out, &nameSize, 1);
	(*out) += 1;

	memcpy(*out, voterKey.c_str(), nameSize);
	(*out) += nameSize;

	this->voterDistrict->write(out);

	nameSize = electionList->size();
	memcpy(*out, &nameSize, 1);
	(*out) += 1;

	std::list<ElectionRecord::Key>::const_iterator it;
	for (it = this->electionList->begin(); it != this->electionList->end(); it++) {
		it->write(out);
	}
}

const VoterRecord::Key & VoterRecord::getKey() const {
	return *((VoterRecord::Key *) key_);
}

void VoterRecord::setKey(uint32_t k) {
	if (this->key_ != NULL) {
		delete this->key_;
	}

	this->key_ = new Key(k);
	this->dni = k;
}

void VoterRecord::setKey(const VoterRecord::Key & k) {

	if (this->key_ != NULL) {
		delete key_;
	}

	this->key_ = new Key(k);
	this->dni = k.getKey();
}

void VoterRecord::addElection(const ElectionRecord::Key & election) {
	this->electionList->push_back(election);
}

void VoterRecord::removeElection(const ElectionRecord::Key & election) {
	this->electionList = new std::list<ElectionRecord::Key>();
	std::list<ElectionRecord::Key>::const_iterator it;
	for (it = electionList->begin(); it != electionList->end(); it++) {
		if (*it == election) {
			this->electionList->remove(*it);
		}
	}
}

const VoterRecord & VoterRecord::operator=(const VoterRecord & rk) {
	if (this == &rk)
		return *this;
	voterName = rk.voterName;
	address = rk.address;
	voterKey = rk.voterKey;
	voterDistrict = new DistrictRecord::Key(*rk.voterDistrict);
	this->key_ = NULL;
	this->setKey(rk.dni);
	this->dni = rk.dni;

	delete this->electionList;

	this->electionList = new std::list<ElectionRecord::Key>();
	std::list<ElectionRecord::Key>::const_iterator it;
	for (it = rk.electionList->begin(); it != rk.electionList->end(); it++) {
		this->electionList->push_back(*it);
	}
	return *this;
}


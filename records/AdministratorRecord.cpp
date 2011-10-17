/*
 * AdministratorRecord.cpp
 *
 *  Created on: 17/10/2011
 *      Author: cecilia
 */

#include "AdministratorRecord.h"

AdministratorRecord::AdministratorRecord(const AdministratorRecord & ar) {
	this->key_ = NULL;
	this->setKey(ar.user);
	this->userKey = ar.userKey;
}

AdministratorRecord::AdministratorRecord(const uint32_t & user, const std::string & userKey) {
	this->key_ = NULL;
	this->setKey(user);
	this->userKey = userKey;
}

AdministratorRecord::AdministratorRecord(char** input) {
	this->key_ = NULL;

	 key_= new Key();
	 read(input);
}

void AdministratorRecord::setKey(const AdministratorRecord::Key & k) {
	if(this->key_ != NULL) {
			delete this->key_;
		}

	    this->key_= new Key(k);
	    this->user = k.getKey();
}

void AdministratorRecord::setKey(const uint32_t & k) {
	if(this->key_ != NULL) {
		delete this->key_;
	}

	this->key_= new Key(k);
	this->user = k;
}

const AdministratorRecord::Key & AdministratorRecord::getKey() const {
	return * ((AdministratorRecord::Key *) key_ );
}

void AdministratorRecord::read(char** input) {
	uint8_t nameSize;

	key_->read(input);
	memcpy(&user,*input,4);
	(*input)+=4;

	memcpy(&nameSize,*input,1);
	(*input)++;
	char * c_str = new char[nameSize+1];
	memcpy(c_str,*input,nameSize);
	(*input)+=nameSize;
	c_str[nameSize]='\0';
	this->userKey = c_str;

	delete c_str;

}

void AdministratorRecord::write(char** output) {
	uint8_t nameSize;
	nameSize = sizeof(this->user);
	key_->write(output);
	(*output)+=nameSize;

	nameSize = this->userKey.size();
	memcpy(*output,&userKey,nameSize);
	(*output)+=nameSize;
}

void AdministratorRecord::setUser(const uint32_t & userName) {
	this->user = userName;
	this->key_ = NULL;
	this->setKey(user);
}

void AdministratorRecord::setUserKey(const std::string & k) {
	this->userKey = k;
}

const uint32_t & AdministratorRecord::getUser() const {
	return ((Key*)key_)->getKey();
}

const std::string & AdministratorRecord::getUserKey() const {
	return this->userKey;
}

const AdministratorRecord & AdministratorRecord::operator =(const AdministratorRecord & k) {
	if(this==&k)
		return *this;

	userKey = k.userKey;
	this->key_ = NULL;
	this->setKey(k.user);
	user = k.user;
	return *this;
}

unsigned int AdministratorRecord::size() const {
	return key_->size() + this->userKey.size() + 1;
}

AdministratorRecord::~AdministratorRecord() {

}


/*
 * ElectionRecord.cpp
 *
 *  Created on: 21/09/2011
 *      Author: juanmromera
 */

#include "ElectionRecord.h"

ElectionRecord::ElectionRecord() {
	this->date = NULL;
	this->district = NULL;
	this->charge = NULL;
}
ElectionRecord::ElectionRecord(std::string date, DistrictRecord::Key* district, ChargeRecord::Key* charge) {
	this->date = date;
	this->charge = new ChargeRecord::Key(charge);
	this->district = new DistrictRecord::Key(district);
}
void ElectionRecord::setCharge(ChargeRecord::Key* charge) {
	this->charge = charge;
}
void ElectionRecord::setDate(std::string date) {
	this->date = date;
}
void ElectionRecord::setDistrict(DistrictRecord::Key* district) {
	this->district = district;
}
ChargeRecord::key* ElectionRecord::getCharge(){
	return this->charge;
}
DistrictRecord::Key* ElectionRecord::getDistrict(){
	return this->district;
}
std::string ElectionRecord::getDate() {
	return this->date;
}
const ElectionRecord::Key ElectionRecord::getKey() {
	return this->key_;
}

void ElectionRecord::read(char** in) {

	 uint8_t nameSize;
	 memcpy(&nameSize,*in,1);
	 (*input)++;

	 char * c_str=new char[nameSize+1];
	 memcpy(c_str,*in,nameSize);
	 (*input)+=nameSize;

	 c_str[nameSize]='\0';
	 this->date=c_str;

	 this->district->read(in);
	 this->charge->read(in);

	 delete c_str;
}
void ElectionRecord::write(char** out) {
	//TODO
}

void ElectionRecord::update(){
	//TODO
}

unsigned int ElectionRecord::size()const{
	return date.size()+1+this->district->size()+this->charge->size();
}

ElectionRecord::~ElectionRecord() {
	delete this->charge;
	delete this->district;
}


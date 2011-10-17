/*
 * ElectionRecord.cpp
 *
 *  Created on: 21/09/2011
 *      Author: juanmromera
 */

#include "ElectionRecord.h"


ElectionRecord::ElectionRecord(char ** input){
	key_= new ElectionRecord::Key(input);
	district = new DistrictRecord::Key(input);
 }

ElectionRecord::ElectionRecord(const ElectionRecord::Key & k)
{
	key_= new ElectionRecord::Key(k);
	district = new DistrictRecord::Key();
}
ElectionRecord::ElectionRecord(unsigned int date, const ChargeRecord::Key& charge, const DistrictRecord::Key& district) {
	key_= new ElectionRecord::Key(date,charge);
	this->district = new DistrictRecord::Key(district);
}
ElectionRecord::ElectionRecord(const ElectionRecord & rec){
	key_= new ElectionRecord::Key(rec.getKey());
	this->district = new DistrictRecord::Key(*rec.district);
}
void ElectionRecord::setCharge(const ChargeRecord::Key& charge) {
	((Key*)key_)->setCharge(charge);
}
void ElectionRecord::setDate(unsigned int date) {
	((Key*)key_)->setDate(date);
}
void ElectionRecord::setDistrict(const DistrictRecord::Key& district) {
	delete this->district;
	this->district = new DistrictRecord::Key(district);
}
void ElectionRecord::setKey(const ElectionRecord::Key & k){
	delete key_;
	key_= new ElectionRecord::Key(k);
}
void ElectionRecord::setKey(unsigned int date, const ChargeRecord::Key& charge){
	delete key_;
	key_= new ElectionRecord::Key(date,charge);
}
const ChargeRecord::Key& ElectionRecord::getCharge()const{
	return ((Key*)key_)->getCharge();
}
const DistrictRecord::Key& ElectionRecord::getDistrict()const{
	return *district;
}
unsigned int ElectionRecord::getDate()const {
	return ((Key*)key_)->getDate();
}
const ElectionRecord::Key& ElectionRecord::getKey()const{
	return *((Key*)key_);
}

void ElectionRecord::read(char** input) {

	key_->read(input);
	district->read(input);
}
void ElectionRecord::write(char** output) {
	key_->write(output);
	district->write(output);
}

unsigned int ElectionRecord::size()const{
	return key_->size() + district->size();
}
ElectionRecord& ElectionRecord::operator=(const ElectionRecord & rec){
	if(this==&rec)
		return *this;
	delete key_;
	delete district;
	key_= new ElectionRecord::Key(rec.getKey());
	this->district = new DistrictRecord::Key(*rec.district);
	return *this;
}

ElectionRecord::~ElectionRecord() {
	delete district;
}



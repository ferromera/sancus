/*
 * ElectionRecord.cpp
 *
 *  Created on: 21/09/2011
 *      Author: juanmromera
 */

#include "ElectionRecord.h"

ElectionRecord::ElectionRecord(const ElectionRecord::Key & k)
{
	this->setKey(k);
}
ElectionRecord::ElectionRecord(unsigned int date, const ChargeRecord::Key& charge, const DistrictRecord::Key& district) {
	key_= new ElectionRecord::Key(date,charge);
	this->district = new DistrictRecord::Key(district);
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

ElectionRecord::~ElectionRecord() {
	delete district;
}


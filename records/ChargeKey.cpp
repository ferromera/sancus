/*
 * ChargeKey.cpp
 *
 *  Created on: 17/10/2011
 *      Author: fernando
 */

#include "ChargeRecord.h"

ChargeRecord::Key::Key(char ** input){
	charge_= new StringKey(input);
	district_= new DistrictRecord::Key(input);
	updateString();
}
ChargeRecord::Key::Key(const std::string &charge,const std::string& district){
	charge_= new StringKey(charge);
	district_= new DistrictRecord::Key(district);
	updateString();
}
ChargeRecord::Key::Key(const std::string & charge ,const DistrictRecord::Key & district){
	charge_= new StringKey(charge);
	district_= new DistrictRecord::Key(district);
	updateString();
}
ChargeRecord::Key::Key(){
	charge_= new StringKey();
	district_= new DistrictRecord::Key();
	updateString();
}
ChargeRecord::Key::Key(const Key & key){
	charge_= new StringKey(*key.charge_);
	district_= new DistrictRecord::Key(*key.district_);
	stringKey_=key.stringKey_;
}
ChargeRecord::Key & ChargeRecord::Key::operator=(const Key & key){
	if(this==&key)
		return *this;
	delete charge_;
	delete district_;
	charge_= new StringKey(*key.charge_);
	district_= new DistrictRecord::Key(*key.district_);
	stringKey_=key.stringKey_;
	return *this;

}
void ChargeRecord::Key::setKey(const std::string & charge, const DistrictRecord::Key& district){
	delete charge_;
	delete district_;
	charge_= new StringKey(charge);
	district_= new DistrictRecord::Key(district);
	updateString();
}
const std::string & ChargeRecord::Key::getKey()const{
	return stringKey_;
}
void ChargeRecord::Key::setDistrict(const DistrictRecord::Key & district){
	delete district_;
	district_= new DistrictRecord::Key(district);
	updateString();
}
void ChargeRecord::Key::setCharge(const std::string & charge){
	delete charge_;
	charge_= new StringKey(charge);
	updateString();
}
const DistrictRecord::Key& ChargeRecord::Key::getDistrict()const{
	return *district_;
}
const std::string & ChargeRecord::Key::getCharge()const{
	return charge_->getString();
}
void ChargeRecord::Key::read(char ** input){
	charge_->read(input);
	district_->read(input);
}
void ChargeRecord::Key::write(char ** output)const{
	charge_->write(output);
	district_->write(output);
}
unsigned int ChargeRecord::Key::size()const{
	return charge_->size()+district_->size();
}
bool ChargeRecord::Key::operator <(const Record::Key &rk)const{
	const Key & k=(const Key&)rk;
	if((*charge_)<(*k.charge_))
		return true;
	else if((*charge_)==(*k.charge_))
		return (*district_)<(*k.district_);
	return false;
}
bool ChargeRecord::Key::operator ==(const Record::Key &rk)const{
	const Key & k=(const Key&)rk;
	return (*charge_)==(*k.charge_) && (*district_)==(*k.district_);
}
bool ChargeRecord::Key::operator <=(const Record::Key &rk)const{
	return (*this)<rk || (*this)==rk;
}
bool ChargeRecord::Key::operator >(const Record::Key & rk)const{
	return !((*this)<=rk);
}
bool ChargeRecord::Key::operator >=(const Record::Key & rk)const{
	return !((*this)<rk);
}
bool ChargeRecord::Key::operator !=(const Record::Key & rk)const{
	return !((*this)==rk);
}
ChargeRecord::Key::~Key(){
	delete charge_;
	delete district_;
}

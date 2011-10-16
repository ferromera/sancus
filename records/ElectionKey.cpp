/*
 * VoteCountingKey.cpp
 *
 *  Created on: 14/10/2011
 *      Author: juan manuel
 */

#include "ElectionRecord.h"
#include "../utils/StringUtils.h"

ElectionRecord::Key::Key(char ** input){
	memcpy(&date,*input,4);
	(*input)+=4;
	charge=new ChargeRecord::Key(input);
	updateString();
}

ElectionRecord::Key::Key(const Key & k){
	date = k.date;
	charge = new ChargeRecord::Key(*k.charge);
	stringKey_=k.stringKey_;
}

ElectionRecord::Key::Key(unsigned int date_, const ChargeRecord::Key & charge){
	date = date_;
	charge = new ChargeRecord::Key(charge);
	updateString();
}
ElectionRecord::Key & ElectionRecord::Key::operator=(const Key & k){
	if(this==&k)
		return *this;
	delete charge;
	date = k.date;
	charge = new ChargeRecord::Key(*k.charge);
	stringKey_=k.stringKey_;
	return *this;
}
const std::string & ElectionRecord::Key::getKey()const{
	return stringKey_;
}
void ElectionRecord::Key::setDate(unsigned int date_){
	date = date_;
	updateString();
}
void ElectionRecord::Key::setCharge(const ChargeRecord::Key& charge){
	delete charge;
	charge =new ChargeRecord::Key(charge);
	updateString();
}
const unsigned int ElectionRecord::Key::getDate()const{
	return date;
}
const ChargeRecord::Key& ElectionRecord::Key::getCharge()const{
	return *charge;
}
void ElectionRecord::Key::read(char ** input){
	delete charge;
	memcpy(&date,*input,4);
	(*input)+=4;
	charge->read(input);
	updateString();

}
void ElectionRecord::Key::write(char ** output)const{
	memcpy(*output,&date,4);
	(*output)+=4;
	charge->write(output);
}
unsigned int ElectionRecord::Key::size()const{
	return 4 + charge->size();
}

void ElectionRecord::Key::updateString(){
	stringKey_="(";
	stringKey_.push_back('(');
	stringKey_.append(StringUtils::intToString(date));
	stringKey_.push_back(')');
	stringKey_.append(charge->getString());
	stringKey_.append(")");

}
bool ElectionRecord::Key::operator <(const Record::Key &r)const
{
	const ElectionRecord::Key & key = (const ElectionRecord::Key &) r;
	if(date < key.date)
		return true;
	else if (date > key.date)
		return false;

	return charge < key.charge;

}
bool ElectionRecord::Key::operator ==(const Record::Key &r)const
{
	const ElectionRecord::Key & key = (const ElectionRecord::Key &) r;
	if(date == key.date)
		return charge == key.charge;

	return false;
}
bool ElectionRecord::Key::operator <=(const Record::Key & r)const
{
	return *this < r || *this == r;
}
bool ElectionRecord::Key::Key::operator >(const Record::Key &r)const
{
	return ! *this <= r;
}
bool ElectionRecord::Key::operator !=(const Record::Key &r)const
{
	return ! *this == r;
}

bool ElectionRecord::Key::operator >=(const Record::Key & r)const
{
	return ! *this<r;
}
ElectionRecord::Key & ElectionRecord::Key::operator=(const ElectionRecord::Key & rk)
{
	if(this == &rk)
		return *this;

	delete charge;
	date = rk.date;
	charge = rk.charge;
	stringKey_ = rk.stringKey_;
	return *this;
}
ElectionRecord::Key::~Key(){
	delete charge;
}

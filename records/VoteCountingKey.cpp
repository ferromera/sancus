/*
 * VoteCountingKey.cpp
 *
 *  Created on: 14/10/2011
 *      Author: fernando
 */

#include "VoteCountingRecord.h"

VoteCountingRecord::Key::Key(char ** input){
	list_=new ListRecord::Key(input);
	district_=new DistrictRecord::Key(input);
	election_=new ElectionRecord::Key(input);
	updateString();
}

VoteCountingRecord::Key::Key(const Key & k){
	list_=new ListRecord::Key(*k.list_);
	district_=new DistrictRecord::Key(*k.district_);
	election_=new ElectionRecord::Key(*k.election_);
	stringKey_=k.stringKey_;
}

VoteCountingRecord::Key::Key(const ListRecord::Key & list, const DistrictRecord::Key & district, const ElectionRecord::Key & election){
	list_=new ListRecord::Key(list);
	district_=new DistrictRecord::Key(district);
	election_=new ElectionRecord::Key(election);
	updateString();
}
VoteCountingRecord::Key & VoteCountingRecord::Key::operator=(const Key & k){
	if(this==&k)
		return *this;
	delete list_;
	delete district_;
	delete election_;
	list_=new ListRecord::Key(*k.list_);
	district_=new DistrictRecord::Key(*k.district_);
	election_=new ElectionRecord::Key(*k.election_);
	stringKey_=k.stringKey_;
	return *this;
}
void VoteCountingRecord::Key::setKey(const ListRecord::Key & list, const DistrictRecord::Key & district, const ElectionRecord::Key & election){
	delete list_;
	delete district_;
	delete election_;
	list_=new ListRecord::Key(list);
	district_=new DistrictRecord::Key(district);
	election_=new ElectionRecord::Key(election);
	updateString();

}
const std::string & VoteCountingRecord::Key::getKey()const{
	return stringKey_;
}
void VoteCountingRecord::Key::setList(const ListRecord::Key & list){
	delete list_;
	list_=new ListRecord::Key(list);
	updateString();
}
void VoteCountingRecord::Key::setDistrict(const DistrictRecord::Key& district){
	delete district_;
	district_=new DistrictRecord::Key(district);
	updateString();
}
void VoteCountingRecord::Key::setElection(const ElectionRecord::Key& election){
	delete election_;
	election_=new ElectionRecord::Key(election);
	updateString();
}
const ListRecord::Key & VoteCountingRecord::Key::getList()const{
	return *list_;
}
const ElectionRecord::Key& VoteCountingRecord::Key::getElection()const{
	return *election_;
}
const DistrictRecord::Key& VoteCountingRecord::Key::getDistrict()const{
	return *district_;
}
void VoteCountingRecord::Key::read(char ** input){
	delete list_;
	delete district_;
	delete election_;
	list_->read(input);
	district_->read(input);
	election_->read(input);
	updateString();

}
void VoteCountingRecord::Key::write(char ** output)const{
	list_->write(output);
	district_->write(output);
	election_->write(output);
}
unsigned int VoteCountingRecord::Key::size()const{
	return list_->size()+district_->size()+election_->size();
}

void VoteCountingRecord::Key::updateString(){
	stringKey_="(";
	stringKey_.append(list_->getString());
	stringKey_.append(district_->getString());
	stringKey_.append(election_->getString());
	stringKey_.append(")");

}
VoteCountingRecord::Key::~Key(){
	delete list_;
	delete district_;
	delete election_;
}

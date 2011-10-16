/*
 * VoteCountingRecord.cpp
 *
 *  Created on: 06/10/2011
 *      Author: juanmromera
 */

#include "VoteCountingRecord.h"

VoteCountingRecord::VoteCountingRecord(const Key & k, unsigned int count){
	key_=new Key(k);
	count_=count;
}
VoteCountingRecord::VoteCountingRecord(const ListRecord::Key & list, const DistrictRecord::Key & district, const ElectionRecord::Key & election, unsigned int block){
	std::string listString=list.getString();
	std::string districtString=district.getString();
	std::string electionString=election.getString();

}
VoteCountingRecord::VoteCountingRecord(const VoteCountingRecord & rec){
	key_=new Key(rec.getKey());
	count_=rec.count_;
}
const ListRecord::Key & VoteCountingRecord::getList()const{
	return ((Key*)key_)->getList();
}
const ElectionRecord::Key& VoteCountingRecord::getElection()const{
	return ((Key*)key_)->getElection();
}
const DistrictRecord::Key& VoteCountingRecord::getDistrict()const{
	return ((Key*)key_)->getDistrict();
}
unsigned int VoteCountingRecord::getCount()const{
	return count_;
}
void VoteCountingRecord::setList(const ListRecord::Key & list){
	((Key*)key_)->setList(list);
}
void VoteCountingRecord::setDistrict(const DistrictRecord::Key& district){
	((Key*)key_)->setDistrict(district);
}
void VoteCountingRecord::setElection(const ElectionRecord::Key& election){
	((Key*)key_)->setElection(election);
}
void VoteCountingRecord::setCount(unsigned int count){
	count_=count;
}
void VoteCountingRecord::setKey(const Key & k){
	delete key_;
	key_=new Key(k);
}
void VoteCountingRecord::setKey(const ListRecord::Key & list, const DistrictRecord::Key & district, const ElectionRecord::Key & election){
	delete key_;
	key_=new Key(list,district,election);
}
const VoteCountingRecord::Key & VoteCountingRecord::getKey()const{
	return *((Key*)key_);
}
void VoteCountingRecord::read(char ** input){
	key_->read(input);
	memcpy(&count_,*input,4);
	(*input)+=4;
}
void VoteCountingRecord::write(char ** output){
	key_->write(output);
	memcpy(*output,&count_,4);
	(*output)+=4;

}
unsigned int VoteCountingRecord::size()const{
	return key_->size()+4;
}

VoteCountingRecord & VoteCountingRecord::operator = (const VoteCountingRecord & rec){
	if(this==&rec)
		return *this;
	delete key_;
	key_=new Key(rec.getKey());
	count_=rec.count_;
	return *this;
}

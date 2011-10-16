/*
 * CandidateKey.cpp
 *
 *  Created on: 16/10/2011
 *      Author: fernando
 */

#include "CandidateRecord.h"

CandidateRecord::Key::Key(char ** input){
	list=new ListRecord::Key(input);
	charge=new ChargeRecord::Key(input);
	updateString();
}
CandidateRecord::Key::Key(){
	list=new ListRecord::Key();
	charge=new ChargeRecord::Key();
	updateString();
}
CandidateRecord::Key::Key(const Key & k){
	list=new ListRecord::Key(*k.list);
	charge=new ChargeRecord::Key(*k.charge);
	stringKey=k.stringKey;
}
CandidateRecord::Key::Key(const ListRecord::Key & aList, const ChargeRecord::Key& aCharge){
	list=new ListRecord::Key(aList);
	charge=new ChargeRecord::Key(aCharge);
	updateString();
}
CandidateRecord::Key & CandidateRecord::Key::operator=(const Key & k){
	if(this==&k)
		return *this;
	delete list;
	delete charge;
	list=new ListRecord::Key(*k.list);
	charge=new ChargeRecord::Key(*k.charge);
	stringKey=k.stringKey;

}

void CandidateRecord::Key::setKey(const ListRecord::Key & aList, const ChargeRecord::Key& aCharge){
	delete list;
	delete charge;
	list=new ListRecord::Key(aList);
	charge=new ChargeRecord::Key(aCharge);
	updateString();
}
const std::string & CandidateRecord::Key::getKey()const{
	return stringKey;
}
void CandidateRecord::Key::setList(const ListRecord::Key & aList){
	delete list;
	list=new ListRecord::Key(aList);
}
void CandidateRecord::Key::setCharge(const ChargeRecord::Key & aCharge){
	delete charge;
	charge=new ChargeRecord::Key(aCharge);
}
const ListRecord::Key& CandidateRecord::Key::getList()const{
	return *list;
}
const ChargeRecord::Key & CandidateRecord::Key::getCharge()const{
	return *charge;
}
void CandidateRecord::Key::read(char ** input){
	list->read(input);
	charge->read(input);
}
void CandidateRecord::Key::write(char ** output)const{
	list->write(output);
	charge->write(output);
}
unsigned int CandidateRecord::Key::size()const{
	return list->size()+charge->size();
}
bool CandidateRecord::Key::operator <(const Record::Key &rk)const{
	const Key & k= (const Key &)rk;
	if((*list)<(*k.list))
		return true;
	else if((*list)==(*k.list))
		return (*charge)<(*k.charge);
	return false;
}
bool CandidateRecord::Key::operator ==(const Record::Key &rk)const{
	const Key & k= (const Key &)rk;
	return (*list)==(*k.list)&&(*charge)==(*k.charge);
}
bool CandidateRecord::Key::operator <=(const Record::Key &rk)const{
	return (*this)<rk || (*this)==rk;
}
bool CandidateRecord::Key::operator >(const Record::Key & rk)const{
	return !(*this<=rk);
}
bool CandidateRecord::Key::operator >=(const Record::Key & rk)const{
	return !(*this<rk);
}
bool CandidateRecord::Key::operator !=(const Record::Key & rk)const{
	return !(*this==rk);
}
void CandidateRecord::Key::updateString(){
	stringKey="(";
	stringKey.append(list->getString());
	stringKey.append(charge->getString());
	stringKey.append(")");
}
CandidateRecord::Key::~Key(){
	delete list;
	delete charge;
}

/*
 * CandidateRecord.cpp
 *
 *  Created on: 16/10/2011
 *      Author: fernando
 */

#include "CandidateRecord.h"

CandidateRecord::CandidateRecord(const CandidateRecord::Key& key){
	key_=new CandidateRecord::Key(key);
	voterKey= new VoterRecord();
}
CandidateRecord::CandidateRecord(const CandidateRecord& rec){
	key_=new CandidateRecord::Key(rec.getKey());
	voterKey= new VoterRecord(rec.getVoter());
}
CandidateRecord::CandidateRecord(const ListRecord::Key& listName,const ChargeRecord::Key& chargeName,const VoterRecord::Key& dni){
	key_=new CandidateRecord::Key(key);
	voterKey= new VoterRecord(dni);
}
unsigned int CandidateRecord::size()const{
	return key_->size() + voterKey->size();
}
void CandidateRecord::setKey(const CandidateRecord::Key & k){
	delete key_;
	key_=new CandidateRecord::Key(k);
}
void CandidateRecord::setKey(const ListRecord::Key& listName,const ChargeRecord::Key& chargeName){
	delete key_;
	key_=new CandidateRecord::Key(listName,chargeName);
}
void CandidateRecord::read(char ** input){
	key_->read(input);
	voterKey->read(input);
}
void CandidateRecord::write(char ** output){
	key_->write(output);
	voterKey->write(output);
}
const CandidateRecord::Key & CandidateRecord::getKey()const{
	return *((Key*)key_);
}
void CandidateRecord::setCharge(const ChargeRecord::Key& chargeName){
	((Key*)key_)->setCharge(chargeName);
}
const ChargeRecord::Key& CandidateRecord::getCharge(){
	return ((Key*)key_)->getCharge();
}
void CandidateRecord::setVoter(const VoterRecord::Key& dni){
	delete voterKey;
	voterKey= new VoterRecord::Key(dni);
}
const VoterRecord::Key& CandidateRecord::getVoter(){
	return *voterKey;
}
void CandidateRecord::setList(const ListRecord::Key& listName){
	((Key*)key_)->setList(listName);
}
const ListRecord::Key& CandidateRecord::getList(){
	return ((Key*)key_)->getList();
}
CandidateRecord & CandidateRecord::operator=(const CandidateRecord &rec){
	if(this==&rec)
		return *this;
	delete key_;
	delete voterKey;
	key_=new CandidateRecord::Key(rec.getKey());
	voterKey= new VoterRecord::Key(rec.getVoter());
}
virtual CandidateRecord::~CandidateRecord(){
	delete voterKey;
}

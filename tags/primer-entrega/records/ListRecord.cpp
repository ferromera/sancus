/*
 * ListRecord.cpp
 *
 *  Created on: 15/10/2011
 *      Author: fernando
 */

#include "ListRecord.h"
ListRecord::ListRecord(){
	key_= new ListRecord::Key();
}
ListRecord::ListRecord(char ** input){
	key_= new ListRecord::Key(input);
}
ListRecord::ListRecord(const ListRecord::Key & k){
	key_= new ListRecord::Key(k);
}
ListRecord::ListRecord(const ListRecord & rec){
	key_= new ListRecord::Key(rec.getKey());
}
ListRecord::ListRecord(const ElectionRecord::Key& election,const std::string & listName){
	key_= new ListRecord::Key(election,listName);
}
unsigned int ListRecord::size()const{
	return key_->size();
}
void ListRecord::setKey(const ListRecord::Key & k){
	delete key_;
	key_= new ListRecord::Key(k);
}
void ListRecord::setKey(const  ElectionRecord::Key& election,const std::string & listName){
	delete key_;
	key_= new ListRecord::Key(election,listName);
}
void ListRecord::read(char ** input){
	key_->read(input);
}
void ListRecord::write(char ** output){
	key_->write(output);
}
void ListRecord::setName(const std::string & listName){
	((Key*)key_)->setName(listName);
}
const std::string & ListRecord::getName()const{
	return ((Key*)key_)->getName();
}
const ListRecord::Key & ListRecord::getKey()const{
	return *((Key*)key_);
}
void ListRecord::setElection (const ElectionRecord::Key & election){
	((Key*)key_)->setElection(election);
}
const ElectionRecord::Key & ListRecord::getElection()const{
	return ((Key*)key_)->getElection();
}
ListRecord & ListRecord::operator=(const ListRecord &rec){
	if(this==&rec)
		return *this;
	delete key_;
	key_= new ListRecord::Key(rec.getKey());
	return *this;

}
ListRecord::~ListRecord(){}

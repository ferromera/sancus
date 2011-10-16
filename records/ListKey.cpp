/*
 * ListKey.cpp
 *
 *  Created on: 15/10/2011
 *      Author: fernando
 */

#include "ListRecord.h"

ListRecord::Key::Key(){
	election=new ElectionRecord::Key();
	updateString();
}
ListRecord::Key::Key(char ** input){
	read(input);
}
ListRecord::Key::Key(const Key & k){
	election= new ElectionRecord::Key(*k.election);
	name=k.name;
	updateString();
}
ListRecord::Key::Key(const ElectionRecord::Key & elect, const std::string & listName){
	election= new ElectionRecord::Key(elect);
	name=listName;
	updateString();
}
Key & ListRecord::Key::operator=(const Key & k){
	if(this==&k)
		return *this;
	delete election;
	election= new ElectionRecord::Key(*k.election);
	name= k.name;
	updateString();
}
void ListRecord::Key::setKey(const ElectionRecord::Key & elect , const std::string  & listName){
	delete election;
	election= new ElectionRecord::Key(elect);
	name=listName;
	updateString();
}
const std::string & ListRecord::Key::getKey()const{
	return stringKey_;
}
void ListRecord::Key::setElection(const ElectionRecord::Key &elect){
	delete election;
	election= new ElectionRecord::Key(elect);
	updateString();
}
void ListRecord::Key::setName(const std::string & listName){
	name=listName;
	updateString();
}
const ElectionRecord::Key& ListRecord::Key::getElection()const{
	return *election;
}
const std::string & ListRecord::Key::getName()const{
	return name;
}
void ListRecord::Key::read(char ** input){
	election->read(input);
	uint8_t stringSize;
	memcpy(&stringSize,*input,1);
	(*input)++;
	char str[257];
	memcpy(str,*input,stringSize);
	(*input)+=stringSize;
	str[stringSize]='\0';
	name=str;
	updateString();

}
void ListRecord::Key::write(char ** output)const{
	election->write(output);
	uint8_t stringSize=name.size();
	memcpy(*output,&stringSize,1);
	(*input)++;
	memcpy(*output,name.c_str(),stringSize);
	(*input)+=stringSize;


}
unsigned int ListRecord::Key::size()const{
	return election->size()+name.size();
}
bool ListRecord::Key::operator <(const Record::Key &rk)const{
	const Key & k= (const Key & )rk;
	if(name.compare(k.name)<0)
		return true;
	if(name.compare(k.name)==0)
		return (*election)<(*k.election);
	return false;
}
bool ListRecord::Key::operator ==(const Record::Key &rk)const{
	const Key & k= (const Key & )rk;
	return (*election)==(*k.election)&& name.compare(k.name)==0;
}

bool ListRecord::Key::operator <=(const Record::Key &rk)const{
	const Key & k= (const Key & )rk;
	return ((*this)< k ||(*this) == k);

}
bool ListRecord::Key::operator >(const Record::Key & rk)const{
	const Key & k= (const Key & )rk;
	return !((*this)<= k);
}
bool ListRecord::Key::operator >=(const Record::Key & rk)const{
	const Key & k= (const Key & )rk;
	return ((*this)> k ||(*this) == k);

}
bool ListRecord::Key::operator !=(const Record::Key & rk)const{
	const Key & k= (const Key & )rk;
	return !((*this)== k);

}
void ListRecord::Key::updateString(){
	stringKey_="(";
	stringKey_.append(name>getString());  // ((dsffdsf)(fdsfsdsfdfs)(9fddsfsdf))
	stringKey_.append(election->getString());
	stringKey_.append(")");
}

ListRecord::Key::~Key(){
	delete election;
}

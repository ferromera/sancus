/*
 * DistrictRecord.cpp
 *
 *  Created on: 09/10/2011
 *      Author: cecilia
 */

#include "DistrictRecord.h"
#include "stdint.h"
#include <cstring>

DistrictRecord::DistrictRecord(){
	key_ = NULL;
	districtFather = NULL;
}
DistrictRecord::DistrictRecord(const DistrictRecord::Key &k)
{
	key_ = new DistrictRecord::Key(k);
	districtFather = NULL;
}
DistrictRecord::DistrictRecord(char ** input){
	key_ = new DistrictRecord::Key(input);
	uint8_t flag;
	memcpy(&flag,*input,1);
	(*input)+=1;
	if(flag == 1)
		districtFather = new DistrictRecord::Key(input);
	else
		districtFather = NULL;
}

DistrictRecord::DistrictRecord(const DistrictRecord &rec){
	key_=new DistrictRecord::Key(rec.getKey());
	if(rec.hasFather())
		districtFather = new DistrictRecord::Key(rec.getFather());
	else
		districtFather = NULL;
}

DistrictRecord::DistrictRecord(const std::string & district){
	key_ = new DistrictRecord::Key(district);
	districtFather = NULL;
}
DistrictRecord::DistrictRecord(const DistrictRecord::Key &k,const DistrictRecord::Key &kFather)
{
	key_ = new DistrictRecord::Key(k);
	districtFather = new DistrictRecord::Key(kFather);
}
DistrictRecord::DistrictRecord(const std::string & chargeName,const std::string & districtFather)
{
	key_ = new DistrictRecord::Key(chargeName);
	this->districtFather = new DistrictRecord::Key(districtFather);
}
unsigned int DistrictRecord::size()const
{
	if(districtFather == NULL)
		return key_->size() + 1;
	return key_->size() + 1 + districtFather->size();
}
void DistrictRecord::setKey(const DistrictRecord::Key & k)
{
	delete key_;
	key_ = new DistrictRecord::Key(k);
}
void DistrictRecord::setFather(const DistrictRecord::Key & k)
{
	delete districtFather;
	districtFather = new DistrictRecord::Key(k);
}
void DistrictRecord::read(char ** input)
{
	key_->read(input);
	uint8_t flag;
	memcpy(&flag,*input,1);
	(*input)+=1;
	if(flag == 1)
		districtFather->read(input);
	else
		districtFather = NULL;
}
void DistrictRecord::write(char ** output){
	key_->write(output);
	uint8_t flag = 0;
	if(districtFather != NULL)
		flag = 1;
	memcpy(*output,&flag,1);
	(*output)+=1;
	if(districtFather != NULL)
		districtFather->write(output);
}
DistrictRecord::~DistrictRecord()
{
	if(districtFather != NULL)
		delete districtFather;
}
void DistrictRecord::setDistrictName(std::string district)
{
	delete key_;
	key_ = new DistrictRecord::Key(district);
}
std::string DistrictRecord::getDistrictName()
{
	return key_->getString();
}
const DistrictRecord::Key & DistrictRecord::getKey()const
{
	return *((DistrictRecord::Key*)key_);
}
void DistrictRecord::setFatherName(std::string districtFather)
{
	delete this->districtFather;
	this->districtFather = new DistrictRecord::Key(districtFather);
}
std::string DistrictRecord::getFatherName()
{
	return districtFather->getString();
}
DistrictRecord::Key & DistrictRecord::getFather()const
{
	if(!hasFather())
		throw districtFatherNullException();
	return *((DistrictRecord::Key*)districtFather);
}
bool DistrictRecord::hasFather()const
{
	if(districtFather != NULL)
		return true;
	return false;
}
DistrictRecord & DistrictRecord::operator=(const DistrictRecord &rec){
	if(this==&rec)
		return *this;
	delete key_;
	delete districtFather;
	key_=new DistrictRecord::Key(rec.getKey());
	if(rec.hasFather())
		districtFather = new DistrictRecord::Key(rec.getFather());
	else
		districtFather = NULL;
	return *this;
}


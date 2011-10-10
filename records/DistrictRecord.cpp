/*
 * DistrictRecord.cpp
 *
 *  Created on: 09/10/2011
 *      Author: cecilia
 */

#include "DistrictRecord.h"

DistrictRecord::DistrictRecord() {
	this->name.clear();
	districtList = NULL;
}

DistrictRecord::DistrictRecord(std::string name,std::list<DistrictRecord::Key*>* districtList) {
	this->name = name;
	districtList = new List<DistrictRecord*> (districtList);
}

void DistrictRecord::setName(std::string name) {
	this->name = name;
}

std::string DistrictRecord::getName() {
	return this->name;
}

const DistrictRecord::Key DistrictRecord::getKey() {
	return this->key_;
}

unsigned int DistrictRecord::size()const{
	return name.size()+1+this->districtList->size();
}
void DistrictRecord::setDistrictList(const std::list<DistrictRecord::Key*>* district){
	this->districtList = district;
}
std::list<DistrictRecord::Key*>* DistrictRecord::getDistrictList(){
	return this->districtList;
}
void DistrictRecord::read(char ** input){
	//todo
}
void DistrictRecord::write(char ** output){
	//todo
}

DistrictRecord::~DistrictRecord() {
	// TODO Auto-generated destructor stub
}


#include "ChargeRecord.h"
#include "stdint.h"

ChargeRecord::ChargeRecord():Record()
{
	key_ = NULL;
	chargeFather = NULL;
}
ChargeRecord::ChargeRecord(char ** input){
	key_=new ChargeRecord::Key(input);
	uint8_t flag;
	memcpy(&flag,*input,1);
	(*input)+=1;
	if(flag == 1)
		chargeFather=new ChargeRecord::Key(input);
	else
		chargeFather = NULL;
}
ChargeRecord::ChargeRecord(const ChargeRecord & rec){
	key_ = new ChargeRecord::Key(rec.getKey());
	if(rec.hasFather())
		chargeFather = new ChargeRecord::Key(rec.getChargeFather());
	else
		chargeFather = NULL;
}
ChargeRecord::ChargeRecord(const ChargeRecord::Key &k){
	key_ = new ChargeRecord::Key(k);
	chargeFather = NULL;
}
ChargeRecord::ChargeRecord(const std::string & charge,const DistrictRecord::Key& district){
	key_ = new ChargeRecord::Key(charge,district);
	chargeFather = NULL;
}
ChargeRecord::ChargeRecord(const std::string & charge,const std::string & district){
	key_ = new ChargeRecord::Key(charge,district);
	chargeFather = NULL;
}
ChargeRecord::ChargeRecord(const ChargeRecord::Key &k,const ChargeRecord::Key &kFather){
	key_ = new ChargeRecord::Key(k);
	chargeFather = new ChargeRecord::Key(kFather);
}
ChargeRecord::ChargeRecord(const std::string & charge,const DistrictRecord::Key& district,const std::string & chargeFather,const DistrictRecord::Key& districtFather){
	key_ = new ChargeRecord::Key(charge,district);
	this->chargeFather = new ChargeRecord::Key(chargeFather,districtFather);
}
ChargeRecord::ChargeRecord(const std::string & charge,const std::string & district,const std::string & chargeFather,const std::string & districtFather){
	key_ = new ChargeRecord::Key(charge,district);
	this->chargeFather = new ChargeRecord::Key(chargeFather,districtFather);
}
unsigned int ChargeRecord::size()const{
	if(chargeFather == NULL)
		return key_->size() + 1;
	return key_->size() + 1 + chargeFather->size();
}
void ChargeRecord::setKey(const ChargeRecord::Key & k){
	delete key_;
	key_ = new ChargeRecord::Key(k);
}
void ChargeRecord::setKey(const std::string & charge,const DistrictRecord::Key& district){
	delete key_;
	key_ = new ChargeRecord::Key(charge,district);
}
void ChargeRecord::read(char ** input){
	delete chargeFather;
	key_->read(input);
	uint8_t flag;
	memcpy(&flag,*input,1);
	(*input)+=1;
	if(flag == 1)
		chargeFather=new ChargeRecord::Key(input);
	else
		chargeFather = NULL;
}
void ChargeRecord::write(char ** output){
	key_->write(output);
	uint8_t flag = 0;
	if(chargeFather != NULL)
		flag = 1;
	memcpy(*output,&flag,1);
	(*output)+=1;
	if(chargeFather != NULL)
		chargeFather->write(output);
}
void ChargeRecord::setChargeName(const std::string & chargeName){
	((Key*)key_)->setCharge(chargeName);
}
std::string ChargeRecord::getChargeName(){
	return ((Key*)key_)->getCharge();
}
void ChargeRecord::setChargeFather(const std::string & chargeFather,const std::string & districtFather){
	delete this->chargeFather;
	this->chargeFather= new ChargeRecord::Key(chargeFather,districtFather);
}
void ChargeRecord::setChargeFather(const std::string & chargeFather,const DistrictRecord::Key& districtFather){
	delete this->chargeFather;
	this->chargeFather= new ChargeRecord::Key(chargeFather,districtFather);
}
void ChargeRecord::setChargeFather(const ChargeRecord::Key & k){
	delete chargeFather;
	chargeFather= new ChargeRecord::Key(k);
}
const ChargeRecord::Key & ChargeRecord::getChargeFather()const{
	if(!hasFather())
		throw chargeFatherNullException();
	return *chargeFather;
}
const ChargeRecord::Key & ChargeRecord::getKey()const{
	return *((Key*)key_);
}
bool ChargeRecord::hasFather()const{
	return chargeFather != NULL;
}
void ChargeRecord::setDistrict(const std::string & district){
	DistrictRecord::Key distKey(district);
	((Key*)key_)->setDistrict(distKey);
}
void ChargeRecord::setDistrict(const DistrictRecord::Key & district){
	((Key*)key_)->setDistrict(district);
}
const DistrictRecord::Key & ChargeRecord::getDistrict()const{
	return ((Key*)key_)->getDistrict();
}
ChargeRecord & ChargeRecord::operator=(const ChargeRecord &rec){
	if(this==&rec)
		return *this;
	delete key_;
	delete chargeFather;
	key_=new ChargeRecord::Key(rec.getKey());
	if(rec.hasFather())
		chargeFather = new ChargeRecord::Key(rec.getChargeFather());
	else
		chargeFather = NULL;
	return *this;
}
ChargeRecord::~ChargeRecord(){
	delete chargeFather;
}

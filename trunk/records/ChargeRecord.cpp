
#include "ChargeRecord.h"
#include "stdint.h"

ChargeRecord::ChargeRecord(const ChargeRecord::Key &k)
{
	key_ = new ChargeRecord::Key(k);
	chargeFather = NULL;
}
ChargeRecord::ChargeRecord(const ChargeRecord::Key &k,const ChargeRecord::Key &kFather)
{
	key_ = new ChargeRecord::Key(k);
	chargeFather = new ChargeRecord::Key(kFather);
}
ChargeRecord::ChargeRecord(std::string chargeName,std::string chargeFather)
{
	key_ = new ChargeRecord::Key(chargeName);
	this->chargeFather = new ChargeRecord::Key(chargeFather);
}
unsigned int ChargeRecord::size()const
{
	if(chargeFather == NULL)
		return key_->size() + 1;
	return key_->size() + 1 + chargeFather->size();
}
void ChargeRecord::setKey(const ChargeRecord::Key & k)
{
	delete key_;
	key_ = new ChargeRecord::Key(k);
}
void ChargeRecord::setChargeFather(const ChargeRecord::Key & k)
{
	delete chargeFather;
	chargeFather = new ChargeRecord::Key(k);
}
void ChargeRecord::read(char ** input)
{
	key_->read(input);
	uint8_t flag;
	memcpy(&flag,*input,1);
	(*input)+=1;
	if(flag == 1)
		chargeFather->read(input);
	else
		chargeFather == NULL;
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
ChargeRecord::~ChargeRecord()
{
	delete chargeFather;
}
void ChargeRecord::setChargeName(std::string chargeName)
{
	delete key_;
	key_ = new ChargeRecord::Key(chargeName);
}
std::string ChargeRecord::getChargeName()
{
	return key_->getString();
}
const ChargeRecord::Key & ChargeRecord::getKey()const
{
	return *((ChargeRecord::Key*)key_);
}
void ChargeRecord::setChargeFather(std::string chargeFather)
{
	delete this->chargeFather;
	this->chargeFather = new ChargeRecord::Key(chargeFather);
}
std::string ChargeRecord::getChargeFather()
{
	return chargeFather->getString();
}
ChargeRecord::Key & ChargeRecord::getChargeFatherKey()const
{
	if(!hasFather())
		throw chargeFatherNullException();
	return *((ChargeRecord::Key*)chargeFather);
}
bool ChargeRecord::hasFather()const
{
	if(chargeFather != NULL)
		return true;
	return false;
}
ChargeRecord & ChargeRecord::operator=(const ChargeRecord &rec){
	if(this==&rec)
		return *this;
	delete key_;
	delete chargeFather;
	key_=new ChargeRecord::Key(rec.getKey());
	if(rec.hasFather())
		chargeFather = new ChargeRecord::Key(rec.getChargeFatherKey());
	else
		chargeFather = NULL;
}

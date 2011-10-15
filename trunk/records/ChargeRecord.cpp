
#include "ChargeRecord.h"

unsigned int ChargeRecord::size()const
{
	unsigned int size = 1 + chargeName.size() + 1;
	std::list<std::string>::iterator itCharges = chargeList->begin();
	for(int i = 0;i<chargeList->size();i++,itCharges++)
	{
		size+= 1 + (*itCharges).size();
	}
	return size;

}
void ChargeRecord::setKey(const ChargeRecord::Key & k)
{
	delete key_;
	key_ = new Key(k);
}
void ChargeRecord::read(char ** input)
{
	char size;
	char largeStr;
	memcpy(&size,*input,1);
	(*input)+=1;
	memcpy(&chargeName,*input,size);
	(*input)+=size;
	memcpy(&size,*input,1);
	(*input)+=1;
	for(int i = 0 ; i < size ; i++)
	{
		memcpy(&largeStr,*input,1);
		(*input)+=1;
	    char *myChar = new char[largeStr + 1];
	    memcpy(myChar,*input,largeStr);
	    (*input)+=largeStr;
	    myChar[largeStr] = '\0';
	    chargeList->push_back(myChar);
	}
}
void ChargeRecord::write(char ** output){
	char size = chargeName.size();
	memcpy(*output,&size,1);
	(*output)+=1;
	memcpy(*output,chargeName.c_str(),size);
	(*output)+=chargeName.size();
	size = chargeList->size();
	memcpy(*output,&size,1);
	(*output)+=1;
	std::list<std::string>::iterator itCharges = chargeList->begin();
	for(unsigned int i = 0;i<chargeList->size();i++,itCharges++)
	{
		size = (*itCharges).size();
		memcpy(*output,&size,1);
		(*output)+=1;
		size = (*itCharges).size();
		memcpy(*output,(*itCharges).c_str(),size);
		(*output)+=(*itCharges).size();
	}

}
ChargeRecord::ChargeRecord()
{
}
ChargeRecord::ChargeRecord(std::string chargeName, std::list<std::string>* chargeList)
{
	this->chargeName = chargeName;
	this->chargeList = chargeList;
}
ChargeRecord::~ChargeRecord()
{
	delete chargeList;
}
void ChargeRecord::setChargeName(std::string chargeName)
{
	this->chargeName = chargeName;
}
std::string ChargeRecord::getChargeName()
{
	return chargeName;
}
const ChargeRecord::Key & ChargeRecord::getKey()const
{
	return *((ChargeRecord::Key*)key_);
}
void ChargeRecord::setChargeList(std::list<std::string>* charges)
{
	chargeList = charges;
}
std::list<std::string>* ChargeRecord::getChargeList()
{
	return chargeList;
}


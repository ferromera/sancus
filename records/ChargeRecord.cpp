
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
	unsigned int size;
	unsigned int largeStr;
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
	    memcpy(&myChar,*input,largeStr);
	    (*input)+=largeStr;
	    myChar[largeStr] = '\0';
	    chargeList->push_back(myChar);
	}
}
void ChargeRecord::write(char ** output){
	memcpy(*output,&chargeName.size(),1);
	(*output)+=1;
	memcpy(*output,chargeName.c_str(),chargeName.size());
	(*output)+=chargeName.size();
	memcpy(*output,&chargeList->size(),1);
	(*output)+=1;
	std::list<std::string>::iterator itCharges = chargeList->begin();
	for(int i = 0;i<chargeList->size();i++,itCharges++)
	{
		memcpy(*output,&(*itCharges).size(),1);
		(*output)+=1;
		memcpy(*output,(*itCharges).c_str(),(*itCharges).size());
		(*output)+=(*itCharges).size();
	}

}
ChargeRecord::ChargeRecord()
{

}
ChargeRecord::ChargeRecord(std::string chargeName, std::list<std::string>* chargeList)
{

}
virtual ChargeRecord::~ChargeRecord()
{

}
void ChargeRecord::setChargeName(std::string chargeName)
{

}
std::string ChargeRecord::getChargeName()
{

}
const ChargeRecord::Key & ChargeRecord::getKey()const
{

}
void setChargeList(const std::list<std::string>* charges)
{

}
std::list<std::string>* getChargeList()
{

}

		}

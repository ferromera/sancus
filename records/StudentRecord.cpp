#include "StudentRecord.h"
#include <cstring>


using namespace std;

StudentRecord::StudentRecord():idNumber_(0),name_(string()){
    key_=new Key;
    buffer= new char[260];
}
StudentRecord::StudentRecord(const StudentRecord & sr):
idNumber_(sr.idNumber_),name_(sr.name_){
    key_=new Key((uint16_t) idNumber_);
    buffer= new char[260];
}

StudentRecord::StudentRecord(char ** input){
    key_= new Key();
    buffer= new char[260];
    read(input);
}

StudentRecord::StudentRecord(uint16_t idNumber,const string & name)
:idNumber_(idNumber),name_(name){
    key_= new Key(idNumber);
    buffer= new char[260];
}

const StudentRecord::Key & StudentRecord::getKey()const{
    //Key * k= dynamic_cast<Key *>(key_);
    //return (*k);
	return * ( (StudentRecord::Key *) key_ );
}
void StudentRecord::setKey(const StudentRecord::Key & k){
    //const Key & ak=dynamic_cast<const Key &>(k);
    delete key_;
    key_=new Key(k);
    idNumber_=k.getKey();
}
void StudentRecord::setKey(int16_t k){
    delete key_;
    key_=new Key(k);
    idNumber_=k;
}
void StudentRecord::read(char ** input){
    char * buffCurr=buffer;
    memcpy(buffCurr,*input,2);
    memcpy(&idNumber_,*input,2);
    delete key_;
    key_=new Key(idNumber_);
    (*input)+=2;
    buffCurr+=2;

    uint8_t nameSize;
    memcpy(buffCurr,*input,1);
    memcpy(&nameSize,*input,1);
    buffCurr++;
    (*input)++;

    char * c_str=new char[nameSize+1];
    memcpy(buffCurr,*input,nameSize);
    memcpy(c_str,*input,nameSize);
    (*input)+=nameSize;

    c_str[nameSize]='\0';
    name_=c_str;


    delete c_str;
}
void StudentRecord::write(char ** output){
    update();
    memcpy(*output,buffer,size());
    (*output)+=size();
}

void StudentRecord::update(){
	//write back to the buffer
	char * buffCurr=buffer;
	memcpy(buffCurr,&idNumber_,2);
	buffCurr+=2;
	uint8_t nameSize=name_.size();
	memcpy(buffCurr,&nameSize,1);
	buffCurr++;
	memcpy(buffCurr,name_.c_str(),nameSize);


}

unsigned int StudentRecord::size()const{
	return sizeof(idNumber_)+1+name_.size();

}
uint16_t StudentRecord::idNumber()const{
    return idNumber_;
}
const string & StudentRecord::name()const{
    return name_;
}
void StudentRecord::idNumber(uint16_t p){
    setKey(p);
}
void StudentRecord::name(const string & n){
    name_=n;
}
StudentRecord& StudentRecord::operator=(const StudentRecord& rec){
	if(this==&rec)
		return *this;
	idNumber_=rec.idNumber_;
	name_=rec.name_;
	key_=new Key((uint16_t) idNumber_);
	buffer= new char[260];
	return *this;
}
StudentRecord::~StudentRecord(){
    delete buffer;
}


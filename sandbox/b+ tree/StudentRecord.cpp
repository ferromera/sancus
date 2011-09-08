#include "StudentRecord.h"
#include <cstring>

using namespace std;

StudentRecord::StudentRecord():dirty(true),idNumber_(0),name_(string()){
    key_=new Key;
    size_=sizeof(idNumber_)+1;
    buffer= new char[260];
}
StudentRecord::StudentRecord(const StudentRecord & sr):
dirty(true),idNumber_(sr.idNumber_),name_(sr.name_){
    key_=new Key(idNumber_);
    size_=sizeof(idNumber_)+1;
    buffer= new char[260];
}

StudentRecord::StudentRecord(char ** input){
    key_= new Key();
    buffer= new char[260];
    read(input);
}

StudentRecord::StudentRecord(uint16_t idNumber,const std::string & name)
:dirty(true),idNumber_(idNumber),name_(name){
    key_= new Key(idNumber);
    size_=sizeof(idNumber_)+1+name_.size();
    buffer= new char[260];
}

const Record::Key & StudentRecord::getKey()const{
    Key * k= dynamic_cast<Key *>(key_);
    return (*k);
}
void StudentRecord::setKey(const Record::Key & k){
    const Key & ak=dynamic_cast<const Key &>(k);
    delete key_;
    key_=new Key(ak);
    idNumber_=ak.getKey();
    dirty=true;
}
void StudentRecord::setKey(int16_t k){
    delete key_;
    key_=new Key(k);
    idNumber_=k;
    dirty=true;
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
    size_=3+nameSize;
    c_str[nameSize]='\0';
    name_=c_str;

    delete c_str;
    dirty=false;
}
void StudentRecord::write(char ** output){
    if(dirty)
        update();
    memcpy(*output,buffer,size_);
    (*output)+=size_;
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
    dirty=true;
}
StudentRecord::~StudentRecord(){
    delete buffer;
}


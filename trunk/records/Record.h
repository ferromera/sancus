#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED

#include <fstream>

class KeyException: public std::exception{};
class IsNotAnIntegerKeyException: public KeyException{};
class IsNotAStringKeyException: public KeyException{};

class Record{
public:
    class Key{
    public:
    virtual unsigned int getUint()const{
    	throw IsNotAnIntegerKeyException();
    }
    virtual const std::string & getString()const{
    	throw IsNotAStringKeyException();
    }
    virtual void read(char ** input)=0;
    virtual void write(char ** output)const=0;
    virtual unsigned int size()const=0;
    virtual bool operator <(const Key &r)const=0;
    virtual bool operator <=(const Key &r)const=0;
    virtual bool operator ==(const Key &r)const=0;
    virtual bool operator >(const Record::Key & r)const=0;
    virtual bool operator >=(const Record::Key & r)const=0;
    virtual bool operator !=(const Record::Key & r)const=0;
    virtual ~Key(){}

    };
protected:
    Key *key_;

public:
    Record(){}
    Record(Key * kp):key_(kp){}

    virtual void read(char ** input)=0;
    virtual void write(char ** output)=0;
    virtual unsigned int size()const=0;


    bool operator <(const Record &r)const{return (*key_)<(*(r.key_));}
    bool operator ==(const Record &r)const{return (*key_)==(*(r.key_));}
    bool operator !=(const Record &r)const{return !((*this)==r);}
    bool operator <=(const Record & r)const{return (*this)<r||(*this)==r;}
    bool operator >(const Record &r)const{return !((*this)<=r);}
    bool operator >=(const Record & r)const{return !((*this)<r);}
    virtual ~Record(){ delete key_;}
};

#endif // RECORD_H_INCLUDED

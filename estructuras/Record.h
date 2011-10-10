#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED

#include <fstream>

class Record{
public:
    class Key{
    public:

    virtual void read(char ** input)=0;
    virtual void write(char ** output)const=0;
    virtual unsigned int size()const=0;
    virtual bool operator <(const Key &r)const=0;
    virtual bool operator <=(const Key &r)const=0;
    virtual bool operator ==(const Key &r)const=0;
    virtual bool operator >(const Record::Key & r)const=0;
    virtual bool operator >=(const Record::Key & r)const=0;
    virtual bool operator !=(const Record::Key & r)const=0;
    virtual Key & operator=(const Record::Key & rk)=0;
    virtual ~Key(){}

    };
protected:
    Key *key_;

public:
    Record(){}
    Record(Key * kp):key_(kp){}
    //virtual const Key & getKey()const=0;
    //virtual void setKey(const Key & k)=0;
    virtual void read(char ** input)=0;
    virtual void write(char ** output)=0;
    virtual unsigned int size()const=0;


    bool operator <(const Record &r)const{return (*key_)<(*(r.key_));}
    bool operator ==(const Record &r)const{return (*key_)==(*(r.key_));}
    bool operator !=(const Record &r)const{return !((*this)==r);}
    bool operator <=(const Record & r)const{return (*this)<r||(*this)==r;}
    bool operator >(const Record& r)const{return !((*this)<=r);}
    bool operator >=(const Record & r)const{return !((*this)<r);}
    virtual ~Record(){ delete key_;}
};

#endif // RECORD_H_INCLUDED

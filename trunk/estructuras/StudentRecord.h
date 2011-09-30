#ifndef STUDENTRECORD_H_INCLUDED
#define STUDENTRECORD_H_INCLUDED

#include "Record.h"
#include "Uint16Key.h"
#include <string>
#include "stdint.h"


class StudentRecord : public Record{
public:
    static const bool isVariable=true;
     //It's variable, so fixed size must be a number > 0.
    static const unsigned int fixedSize=1;
    class Key: public Uint16Key{
    public:
        Key(char ** input):Uint16Key(input){}
        Key(uint16_t key=0):Uint16Key(key){}
        ~Key(){}
    };
private:
    char * buffer;
    uint16_t idNumber_;
    std::string name_;
    void update();

public:
    StudentRecord();
    StudentRecord(const StudentRecord & );
    StudentRecord(char ** input);
    StudentRecord(uint16_t idNumber, const std::string & name);
    const Record::Key & getKey()const;
    void setKey(const Record::Key & k);
    void setKey(int16_t k);

    void read(char ** input);
    void write(char ** output);
    unsigned int size()const;

    uint16_t idNumber()const;
    const std::string & name()const;
    void idNumber(uint16_t );
    void name(const std::string &);
    StudentRecord& operator=(const StudentRecord&);
    ~StudentRecord();

};

#endif // STUDENTRECORD_H_INCLUDED

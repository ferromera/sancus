#ifndef UINT16KEY_H_INCLUDED
#define UINT16KEY_H_INCLUDED

#include "Record.h"
#include "stdint.h"

class Uint16Key: public Record::Key{
        uint16_t buffer;
    public:
        static const bool isVariable=false;
        static const bool isString=false;
        Uint16Key(char ** input); //Load the key from input.
        Uint16Key(uint16_t key=0);
        Uint16Key(const Uint16Key &);
        void setKey(uint16_t  i);
        void setKey(const Record::Key & rk);
        uint16_t getKey()const;
        unsigned int getUint() const{return getKey();}

        void read(char ** input);
        void write(char ** output)const;

        bool operator <(const Record::Key &r)const;
        bool operator ==(const Record::Key &r)const;
        bool operator <=(const Record::Key & r)const;
        bool operator >(const Record::Key &r)const;
        bool operator !=(const Record::Key &r)const;
        bool operator >=(const Record::Key & r)const;

        Record::Key & operator=(const Record::Key & rk);
        Uint16Key & operator=(const Uint16Key & rk);
        Uint16Key & operator=(uint16_t i);
        unsigned int size()const{ return 2; }
        ~Uint16Key(){}
    };


#endif // UINT16KEY_H_INCLUDED

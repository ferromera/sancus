#ifndef UINT16KEY_H_INCLUDED
#define UINT16KEY_H_INCLUDED

#include "Record.h"
#include "stdint.h"

class Uint16Key: public Record::Key{
        uint16_t buffer;
    public:
        static const bool isVariable=false;
        static const unsigned int fixedSize=2;
        Uint16Key(char ** input); //Load the key from input.
        Uint16Key(uint16_t key=0);
        Uint16Key(const Uint16Key &);
        void setKey(uint16_t  i);
        void setKey(const Record::Key & rk);
        uint16_t getKey()const;

        void read(char ** input);
        void write(char ** output)const;

        bool operator <(const Record::Key &r)const;
        bool operator ==(const Record::Key &r)const;
        Record::Key & operator=(const Record::Key & rk);
        Uint16Key & operator=(uint16_t i);
    };


#endif // UINT16KEY_H_INCLUDED

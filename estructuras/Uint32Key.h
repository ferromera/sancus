/*
 * Uint32Key.h
 *
 *  Created on: 12/10/2011
 *      Author: cecilia
 */

#ifndef UINT32KEY_H_
#define UINT32KEY_H_

#include "../records/Record.h"
#include "stdint.h"

class Uint32Key: public Record::Key{
        uint32_t buffer;
    public:
        static const bool isVariable=false;
        static const unsigned int fixedSize=2;
        static const bool isString=false;
        Uint32Key(char ** input); //Load the key from input.
        Uint32Key(uint32_t key=0);
        Uint32Key(const Uint32Key &);
        void setKey(uint32_t  i);
        void setKey(const Record::Key & rk);
        const uint32_t & getKey()const;

        void read(char ** input);
        void write(char ** output)const;

        bool operator <(const Record::Key &r)const;
        bool operator ==(const Record::Key &r)const;
        bool operator <=(const Record::Key & r)const;
        bool operator >(const Record::Key &r)const;
        bool operator !=(const Record::Key &r)const;
        bool operator >=(const Record::Key & r)const;

        Record::Key & operator=(const Record::Key & rk);
        Uint32Key & operator=(const Uint32Key & rk);
        Uint32Key & operator=(uint32_t i);
        unsigned int size()const{ return 2; }
        ~Uint32Key(){}
    };

#endif /* UINT32KEY_H_ */

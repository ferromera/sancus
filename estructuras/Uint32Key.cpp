/*
 * Uint32Key.cpp
 *
 *  Created on: 12/10/2011
 *      Author: cecilia
 */

#include "Uint32Key.h"
#include <cstring>

using namespace std;

Uint32Key::Uint32Key(char ** input){
    read(input);
}
Uint32Key::Uint32Key(uint32_t key):buffer(key){
}
Uint32Key::Uint32Key(const Uint32Key & uk):buffer(uk.buffer){
}
const uint32_t & Uint32Key::getKey()const{
    return buffer;
}

void Uint32Key::setKey(uint32_t i){
    buffer=i;
}
void Uint32Key::setKey(const Record::Key & rk){
    const Uint32Key & ik= dynamic_cast<const Uint32Key &>(rk);
    buffer=ik.buffer;

}
void Uint32Key::read(char ** input){
    memcpy(&buffer,*input,4);
    (*input)+=4;
}
void Uint32Key::write(char ** output)const{
    memcpy(*output,&buffer,4);
    (*output)+=4;
}
bool Uint32Key::operator <(const Record::Key &rk)const{
    const Uint32Key & ik= dynamic_cast<const Uint32Key &>(rk);
    return buffer<ik.buffer;
}
bool Uint32Key::operator ==(const Record::Key &rk)const{
    const Uint32Key & ik= dynamic_cast<const Uint32Key &>(rk);
    return buffer==ik.buffer;
}
bool Uint32Key::operator <=(const Record::Key &rk)const{
    const Uint32Key & ik= dynamic_cast<const Uint32Key &>(rk);
    return buffer<=ik.buffer;
}
bool Uint32Key::operator >(const Record::Key & rk)const{
	const Uint32Key & ik= dynamic_cast<const Uint32Key &>(rk);
    return buffer>ik.buffer;
}
bool Uint32Key::operator >=(const Record::Key & rk)const{
	const Uint32Key & ik= dynamic_cast<const Uint32Key &>(rk);
	return buffer>=ik.buffer;
}
bool Uint32Key::operator !=(const Record::Key & rk)const{
	const Uint32Key & ik= dynamic_cast<const Uint32Key &>(rk);
	return buffer!=ik.buffer;
}

Record::Key & Uint32Key::operator=(const Record::Key & rk){
    const Uint32Key & ik= dynamic_cast<const Uint32Key &>(rk);
    buffer=ik.buffer;
    return (*this);
}
Uint32Key & Uint32Key::operator=(const Uint32Key & u){
    buffer=u.buffer;
    return (*this);
}
Uint32Key & Uint32Key::operator=(uint32_t i){
    buffer=i;
    return (*this);
}

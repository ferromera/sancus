#include "Uint16Key.h"
#include <cstring>

using namespace std;

Uint16Key::Uint16Key(char ** input){
    read(input);
}
Uint16Key::Uint16Key(uint16_t key):buffer(key){
}
Uint16Key::Uint16Key(const Uint16Key & uk):buffer(uk.buffer){
}
uint16_t Uint16Key::getKey()const{
    return buffer;
}

void Uint16Key::setKey(uint16_t i){
    buffer=i;
}
void Uint16Key::setKey(const Record::Key & rk){
    const Uint16Key & ik= dynamic_cast<const Uint16Key &>(rk);
    buffer=ik.buffer;

}
void Uint16Key::read(char ** input){
    memcpy(&buffer,*input,2);
    (*input)+=2;
}
void Uint16Key::write(char ** output)const{
    memcpy(*output,&buffer,2);
    (*output)+=2;
}
bool Uint16Key::operator <(const Record::Key &rk)const{
    const Uint16Key & ik= dynamic_cast<const Uint16Key &>(rk);
    return buffer<ik.buffer;
}
bool Uint16Key::operator ==(const Record::Key &rk)const{
    const Uint16Key & ik= dynamic_cast<const Uint16Key &>(rk);
    return buffer==ik.buffer;
}
Record::Key & Uint16Key::operator=(const Record::Key & rk){
    const Uint16Key & ik= dynamic_cast<const Uint16Key &>(rk);
    buffer=ik.buffer;
    return (*this);
}
Uint16Key & Uint16Key::operator=(uint16_t i){
    buffer=i;
    return (*this);
}

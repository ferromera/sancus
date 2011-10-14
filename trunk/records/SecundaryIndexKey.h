/*
 * SecundaryIndexKey.h
 *
 *  Created on: 14/10/2011
 *      Author: fernando
 */

#ifndef SECUNDARYINDEXKEY_H_
#define SECUNDARYINDEXKEY_H_

#include <stdio.h>

template <class attributeKey, class primaryKey>
class SecundaryIndexKey: public Record::Key{
	attributeKey * att;
	primaryKey * pri;
public:

	SecundaryIndexKey(char ** input);
	SecundaryIndexKey(const SecundaryIndexKey<attributeKey,primaryKey> &);
	SecundaryIndexKey(const attributeKey & a,const primaryKey & p);
	static const bool isVariable=true;
	void setKey(const attributeKey & a,const primaryKey & p);
	void setKey(const Record::Key & rk);
	std::string getKey()const;
	void setAttribute(const attributeKey &);
	void setPrimary(const primaryKey &);
	const attributeKey & getAttribute();
	const primaryKey & getPrimary();


	void read(char ** input);
	void write(char ** output)const;

    bool operator <(const Record::Key &k)const;
    bool operator ==(const Record::Key &k)const;
    bool operator <=(const Record::Key & k)const;
    bool operator >(const Record::Key &k)const;
    bool operator !=(const Record::Key &k)const;
    bool operator >=(const Record::Key & k)const;

    SecundaryIndexKey<attributeKey,primaryKey> & operator=(const SecundaryIndexKey<attributeKey,primaryKey> & rk);

    unsigned int size()const;
	~SecundaryIndexKey();

};

template <class attributeKey, class primaryKey>
SecundaryIndexKey<attributeKey,primaryKey>::SecundaryIndexKey(char ** input):att(NULL),pri(NULL){
	read(input);
}

template <class attributeKey, class primaryKey>
SecundaryIndexKey<attributeKey,primaryKey>::SecundaryIndexKey(const SecundaryIndexKey<attributeKey,primaryKey> & key){
	att=new attributeKey(*(key.att));
	pri=new primaryKey(*(key.pri));
}

template <class attributeKey, class primaryKey>
SecundaryIndexKey<attributeKey,primaryKey>::SecundaryIndexKey(const attributeKey & a,const primaryKey & p){
	att=new attributeKey(a);
	pri=new primaryKey(p);
}

template <class attributeKey, class primaryKey>
void SecundaryIndexKey<attributeKey,primaryKey>::setKey(const attributeKey & a,const primaryKey & p){
	delete att;
	delete pri;
	att=new attributeKey(a);
	pri=new primaryKey(p);
}

template <class attributeKey, class primaryKey>
void SecundaryIndexKey<attributeKey,primaryKey>::setKey(const Record::Key & rk){
	SecundaryIndexKey<attributeKey,primaryKey> & secKey= dynamic_cast<SecundaryIndexKey<attributeKey,primaryKey> &>(rk);
	setKey(secKey.getAttribute(),secKey.getPrimary());
}

template <class attributeKey, class primaryKey>
std::string SecundaryIndexKey<attributeKey,primaryKey>::getKey()const{
	std::string stringKey;
	if(att->isString())
		stringKey=att->getKey();
	else{
		char str [256];
		spintf(str,"%u",att->getKey());
		stringKey=str;
	}
	stringKey.append("|");
	if(pri->isString())
		stringKey.append(att->getKey());
	else{
		char str [256];
		spintf(str,"%u",att->getKey());
		stringKey.append(str);
	}
	return stringKey;

}

template <class attributeKey, class primaryKey>
void SecundaryIndexKey<attributeKey,primaryKey>::setAttribute(const attributeKey & a){
	delete att;
	att=new attributeKey(a);
}

template <class attributeKey, class primaryKey>
void SecundaryIndexKey<attributeKey,primaryKey>::setPrimary(const primaryKey & p){
	delete pri;
	pri=new primaryKey(p);
}

template <class attributeKey, class primaryKey>
const attributeKey & SecundaryIndexKey<attributeKey,primaryKey>::getAttribute(){
	return *att;
}

template <class attributeKey, class primaryKey>
const primaryKey & SecundaryIndexKey<attributeKey,primaryKey>::getPrimary(){
	return *pri;
}

template <class attributeKey, class primaryKey>
void SecundaryIndexKey<attributeKey,primaryKey>::read(char ** input){
	delete att;
	delete pri;
	att=new attributeKey(input);
	pri=new primaryKey(input);
}

template <class attributeKey, class primaryKey>
void SecundaryIndexKey<attributeKey,primaryKey>::write(char ** output)const{
	att->write(output);
	pri->write(output);
}

template <class attributeKey, class primaryKey>
bool SecundaryIndexKey<attributeKey,primaryKey>::operator <(const Record::Key &k)const{
	SecundaryIndexKey<attributeKey,primaryKey> & secKey= dynamic_cast<SecundaryIndexKey<attributeKey,primaryKey> &>(k);
	if((*att)<(*secKey.att))
		return true;
	else if((*att)==(*secKey.att))
		return (*pri)<(*secKey.pri);
	return false;

}

template <class attributeKey, class primaryKey>
bool SecundaryIndexKey<attributeKey,primaryKey>::operator ==(const Record::Key &k)const{
	SecundaryIndexKey<attributeKey,primaryKey> & secKey= dynamic_cast<SecundaryIndexKey<attributeKey,primaryKey> &>(k);
	return ((*att)==(*secKey.att) && (*pri)==(*secKey.pri));
}
template <class attributeKey, class primaryKey>
bool SecundaryIndexKey<attributeKey,primaryKey>::operator <=(const Record::Key & k)const{
	SecundaryIndexKey<attributeKey,primaryKey> & secKey= dynamic_cast<SecundaryIndexKey<attributeKey,primaryKey> &>(k);
	return ((*this)<secKey || (*this)==secKey );
}
template <class attributeKey, class primaryKey>
bool SecundaryIndexKey<attributeKey,primaryKey>::operator >(const Record::Key &k)const{
	SecundaryIndexKey<attributeKey,primaryKey> & secKey= dynamic_cast<SecundaryIndexKey<attributeKey,primaryKey> &>(k);
	return !((*this)<=secKey );
}
template <class attributeKey, class primaryKey>
bool SecundaryIndexKey<attributeKey,primaryKey>::operator !=(const Record::Key &k)const{
	SecundaryIndexKey<attributeKey,primaryKey> & secKey= dynamic_cast<SecundaryIndexKey<attributeKey,primaryKey> &>(k);
	return ((*this)<secKey || (*this)>secKey );
}
template <class attributeKey, class primaryKey>
bool SecundaryIndexKey<attributeKey,primaryKey>::operator >=(const Record::Key & k)const{
	return !((*this)<secKey );
}

template <class attributeKey, class primaryKey>
SecundaryIndexKey<attributeKey,primaryKey> & SecundaryIndexKey<attributeKey,primaryKey>::operator=(const SecundaryIndexKey<attributeKey,primaryKey> & k){
	if(this == &rk)
		return *this;
	setKey(k);
	return *this;
}

template <class attributeKey, class primaryKey>
unsigned int SecundaryIndexKey<attributeKey,primaryKey>::size()const{
	return att->size()+pri->size();
}

template <class attributeKey, class primaryKey>
SecundaryIndexKey<attributeKey,primaryKey>::~SecundaryIndexKey(){
	delete att;
	delete pri;
}


#endif /* SECUNDARYINDEXKEY_H_ */

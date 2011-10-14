/*
 * SecondaryIndexKey.h
 *
 *  Created on: 14/10/2011
 *      Author: fernando
 */

#ifndef SECONDARYINDEXKEY_H_
#define SECONDARYINDEXKEY_H_

#include <stdio.h>

template <class attributeKey, class primaryKey>
class SecondaryIndexKey: public Record::Key{
	attributeKey * att;
	primaryKey * pri;
	std::string stringKey;

	void updateString();
public:

	SecondaryIndexKey(char ** input);
	SecondaryIndexKey(const SecondaryIndexKey<attributeKey,primaryKey> &);
	SecondaryIndexKey(const attributeKey & a,const primaryKey & p);
	static const bool isVariable=true;
	static const bool isString=true;
	const std::string & getString(){return getKey();}
	void setKey(const attributeKey & a,const primaryKey & p);
	void setKey(const Record::Key & rk);
	const std::string & getKey()const;
	void setAttribute(const attributeKey &);
	void setPrimary(const primaryKey &);
	const attributeKey & getAttribute()const;
	const primaryKey & getPrimary()const;


	void read(char ** input);
	void write(char ** output)const;

    bool operator <(const Record::Key &k)const;
    bool operator ==(const Record::Key &k)const;
    bool operator <=(const Record::Key & k)const;
    bool operator >(const Record::Key &k)const;
    bool operator !=(const Record::Key &k)const;
    bool operator >=(const Record::Key & k)const;

    SecondaryIndexKey<attributeKey,primaryKey> & operator=(const SecondaryIndexKey<attributeKey,primaryKey> & rk);

    unsigned int size()const;
	~SecondaryIndexKey();

};

template <class attributeKey, class primaryKey>
SecondaryIndexKey<attributeKey,primaryKey>::SecondaryIndexKey(char ** input):att(NULL),pri(NULL){
	read(input);
}

template <class attributeKey, class primaryKey>
SecondaryIndexKey<attributeKey,primaryKey>::SecondaryIndexKey(const SecondaryIndexKey<attributeKey,primaryKey> & key){
	att=new attributeKey(*(key.att));
	pri=new primaryKey(*(key.pri));
	updateString();
}

template <class attributeKey, class primaryKey>
SecondaryIndexKey<attributeKey,primaryKey>::SecondaryIndexKey(const attributeKey & a,const primaryKey & p){
	att=new attributeKey(a);
	pri=new primaryKey(p);
	updateString();
}

template <class attributeKey, class primaryKey>
void SecondaryIndexKey<attributeKey,primaryKey>::setKey(const attributeKey & a,const primaryKey & p){
	delete att;
	delete pri;
	att=new attributeKey(a);
	pri=new primaryKey(p);
	updateString();
}

template <class attributeKey, class primaryKey>
void SecondaryIndexKey<attributeKey,primaryKey>::setKey(const Record::Key & rk){
	const SecondaryIndexKey<attributeKey,primaryKey> & secKey= dynamic_cast<const SecondaryIndexKey<attributeKey,primaryKey> &>(rk);
	setKey(secKey.getAttribute(),secKey.getPrimary());
}

template <class attributeKey, class primaryKey>
void  SecondaryIndexKey<attributeKey,primaryKey>::updateString(){

	if(attributeKey::isString)
		stringKey="att->getString";
	else{
		char str [256];
		sprintf(str,"%u",att->getUint());
		stringKey=str;
	}
	stringKey.append("|");
	if(primaryKey::isString)
		stringKey.append(pri->getString());
	else{
		char str [256];
		sprintf(str,"%u",pri->getUint());
		stringKey.append(str);
	}

}

template <class attributeKey, class primaryKey>
const std::string &  SecondaryIndexKey<attributeKey,primaryKey>::getKey()const{
	return stringKey;
}

template <class attributeKey, class primaryKey>
void SecondaryIndexKey<attributeKey,primaryKey>::setAttribute(const attributeKey & a){
	delete att;
	att=new attributeKey(a);
	updateString();
}

template <class attributeKey, class primaryKey>
void SecondaryIndexKey<attributeKey,primaryKey>::setPrimary(const primaryKey & p){
	delete pri;
	pri=new primaryKey(p);
	updateString();
}

template <class attributeKey, class primaryKey>
const attributeKey & SecondaryIndexKey<attributeKey,primaryKey>::getAttribute()const{
	return *att;
}

template <class attributeKey, class primaryKey>
const primaryKey & SecondaryIndexKey<attributeKey,primaryKey>::getPrimary()const{
	return *pri;
}

template <class attributeKey, class primaryKey>
void SecondaryIndexKey<attributeKey,primaryKey>::read(char ** input){
	delete att;
	delete pri;
	att=new attributeKey(input);
	pri=new primaryKey(input);
	updateString();
}

template <class attributeKey, class primaryKey>
void SecondaryIndexKey<attributeKey,primaryKey>::write(char ** output)const{
	att->write(output);
	pri->write(output);
}

template <class attributeKey, class primaryKey>
bool SecondaryIndexKey<attributeKey,primaryKey>::operator <(const Record::Key &k)const{
	const SecondaryIndexKey<attributeKey,primaryKey> & secKey= dynamic_cast<const SecondaryIndexKey<attributeKey,primaryKey> &>(k);
	if((*att)<(*secKey.att))
		return true;
	else if((*att)==(*secKey.att))
		return (*pri)<(*secKey.pri);
	return false;

}

template <class attributeKey, class primaryKey>
bool SecondaryIndexKey<attributeKey,primaryKey>::operator ==(const Record::Key &k)const{
	const SecondaryIndexKey<attributeKey,primaryKey> & secKey= dynamic_cast<const SecondaryIndexKey<attributeKey,primaryKey> &>(k);
	return ((*att)==(*secKey.att) && (*pri)==(*secKey.pri));
}
template <class attributeKey, class primaryKey>
bool SecondaryIndexKey<attributeKey,primaryKey>::operator <=(const Record::Key & k)const{
	const SecondaryIndexKey<attributeKey,primaryKey> & secKey= dynamic_cast<const SecondaryIndexKey<attributeKey,primaryKey> &>(k);
	return ((*this)<secKey || (*this)==secKey );
}
template <class attributeKey, class primaryKey>
bool SecondaryIndexKey<attributeKey,primaryKey>::operator >(const Record::Key &k)const{
	const SecondaryIndexKey<attributeKey,primaryKey> & secKey= dynamic_cast<const SecondaryIndexKey<attributeKey,primaryKey> &>(k);
	return !((*this)<=secKey );
}
template <class attributeKey, class primaryKey>
bool SecondaryIndexKey<attributeKey,primaryKey>::operator !=(const Record::Key &k)const{
	const SecondaryIndexKey<attributeKey,primaryKey> & secKey= dynamic_cast<const SecondaryIndexKey<attributeKey,primaryKey> &>(k);
	return ((*this)<secKey || (*this)>secKey );
}
template <class attributeKey, class primaryKey>
bool SecondaryIndexKey<attributeKey,primaryKey>::operator >=(const Record::Key & k)const{
	const SecondaryIndexKey<attributeKey,primaryKey> & secKey= dynamic_cast<const SecondaryIndexKey<attributeKey,primaryKey> &>(k);
	return !((*this)<secKey );
}

template <class attributeKey, class primaryKey>
SecondaryIndexKey<attributeKey,primaryKey> & SecondaryIndexKey<attributeKey,primaryKey>::operator=(const SecondaryIndexKey<attributeKey,primaryKey> & k){
	if(this == &k)
		return *this;
	setKey(k);
	return *this;
}

template <class attributeKey, class primaryKey>
unsigned int SecondaryIndexKey<attributeKey,primaryKey>::size()const{
	return att->size()+pri->size();
}

template <class attributeKey, class primaryKey>
SecondaryIndexKey<attributeKey,primaryKey>::~SecondaryIndexKey(){
	delete att;
	delete pri;
}


#endif /* SECONDARYINDEXKEY_H_ */

/*
 * SecundaryIndexRecord.h
 *
 *  Created on: 14/10/2011
 *      Author: fernando
 */

#ifndef SECUNDARYINDEXRECORD_H_
#define SECUNDARYINDEXRECORD_H_

#include "Record.h"
#include "SecundaryIndexKey.h"

template<class attributeKey,class primaryKey>
class SecundaryIndexRecord: public Record{
public:
	class Key:public SecundaryIndexKey<attributeKey,primaryKey>{
	public:
		 Key(char ** input):SecundaryIndexKey<attributeKey,primaryKey>(input){}
		 Key(const attributeKey & at,const primaryKey &pr):
			 SecundaryIndexKey<attributeKey,primaryKey>(at,pr){
		 }
		 Key(const Key & k):SecundaryIndexKey<attributeKey,primaryKey>(k){}
		 ~Key(){}
	};
	SecundaryIndexRecord(char ** input);
	SecundaryIndexRecord(const SecundaryIndexRecord<attributeKey,primaryKey> & );
	SecundaryIndexRecord(const attributeKey& a , const primaryKey & p);

	const typename SecundaryIndexRecord<attributeKey,primaryKey>::Key & getKey()const;
	void setKey(const SecundaryIndexRecord<attributeKey,primaryKey>::Key & k);

	void setKey(const attributeKey& a , const primaryKey & p);

	void read(char ** input);
	void write(char ** output);
	unsigned int size()const;

	void setAttribute(const attributeKey& a )const;
    void setPrimary(const primaryKey& a )const;
    const attributeKey& getAttribute();
    const primaryKey& getPrimary();
    SecundaryIndexRecord<attributeKey,primaryKey>& operator=(const SecundaryIndexRecord<attributeKey,primaryKey>&);
    static const bool isVariable=true;

	virtual ~SecundaryIndexRecord();
};

template<class attributeKey,class primaryKey>
SecundaryIndexRecord<attributeKey,primaryKey>::SecundaryIndexRecord(char ** input){
	key_=new Key(input);
}

template<class attributeKey,class primaryKey>
SecundaryIndexRecord<attributeKey,primaryKey>::SecundaryIndexRecord(const SecundaryIndexRecord<attributeKey,primaryKey> & rec){
	key_=new Key(*((Key*)rec.key_));
}
template<class attributeKey,class primaryKey>
SecundaryIndexRecord<attributeKey,primaryKey>::SecundaryIndexRecord(const attributeKey& a , const primaryKey & p){
	key_=new Key(a,p);
}

template<class attributeKey,class primaryKey>
const typename SecundaryIndexRecord<attributeKey,primaryKey>::Key &
SecundaryIndexRecord<attributeKey,primaryKey>::getKey()const{
	return *key_;
}

template<class attributeKey,class primaryKey>
void SecundaryIndexRecord<attributeKey,primaryKey>::setKey(const SecundaryIndexRecord<attributeKey,primaryKey>::Key & k){
	((Key*)key_)->setKey(k);
}

template<class attributeKey,class primaryKey>
void SecundaryIndexRecord<attributeKey,primaryKey>::setKey(const attributeKey& a , const primaryKey & p){
	((Key*)key_)->setKey(a,p);
}

template<class attributeKey,class primaryKey>
void SecundaryIndexRecord<attributeKey,primaryKey>::read(char ** input){
	key_->read(input);
}
template<class attributeKey,class primaryKey>
void SecundaryIndexRecord<attributeKey,primaryKey>::write(char ** output){
	key_->write(output);
}

template<class attributeKey,class primaryKey>
unsigned int SecundaryIndexRecord<attributeKey,primaryKey>::size()const{
	return key_->size();
}

template<class attributeKey,class primaryKey>
void SecundaryIndexRecord<attributeKey,primaryKey>::setAttribute(const attributeKey& a )const{
	((Key*)key_)->setAttribute(a);
}

template<class attributeKey,class primaryKey>
void SecundaryIndexRecord<attributeKey,primaryKey>::setPrimary(const primaryKey& p )const{
	((Key*)key_)->setPrimary(p);
}

template<class attributeKey,class primaryKey>
const attributeKey& SecundaryIndexRecord<attributeKey,primaryKey>::getAttribute(){
	return ((Key*)key_)->getAttribute();
}

template<class attributeKey,class primaryKey>
const primaryKey& SecundaryIndexRecord<attributeKey,primaryKey>::getPrimary(){
	return ((Key*)key_)->getPrimary();
}

template<class attributeKey,class primaryKey>
SecundaryIndexRecord<attributeKey,primaryKey>& SecundaryIndexRecord<attributeKey,primaryKey>::operator=(const SecundaryIndexRecord<attributeKey,primaryKey>& rec){
	if(this==&rec)
		return *this;
	key_=new Key(*((Key*)rec.key_));

}

template<class attributeKey,class primaryKey>
SecundaryIndexRecord<attributeKey,primaryKey>::~SecundaryIndexRecord(){

}

#endif /* SECUNDARYINDEXRECORD_H_ */

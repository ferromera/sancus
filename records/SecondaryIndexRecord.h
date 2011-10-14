/*
 * SecondaryIndexRecord.h
 *
 *  Created on: 14/10/2011
 *      Author: fernando
 */

#ifndef SECONDARYINDEXRECORD_H_
#define SECONDARYINDEXRECORD_H_

#include "Record.h"
#include "SecondaryIndexKey.h"

template<class attributeKey,class primaryKey>
class SecondaryIndexRecord: public Record{
public:
	class Key:public SecondaryIndexKey<attributeKey,primaryKey>{
	public:
		 Key(char ** input):SecondaryIndexKey<attributeKey,primaryKey>(input){}
		 Key(const attributeKey & at,const primaryKey &pr):
			 SecondaryIndexKey<attributeKey,primaryKey>(at,pr){
		 }
		 Key(const Key & k):SecondaryIndexKey<attributeKey,primaryKey>(k){}
		 ~Key(){}
	};
	SecondaryIndexRecord(char ** input);
	SecondaryIndexRecord(const SecondaryIndexRecord<attributeKey,primaryKey> & );
	SecondaryIndexRecord(const attributeKey& a , const primaryKey & p);

	const typename SecondaryIndexRecord<attributeKey,primaryKey>::Key & getKey()const;
	void setKey(const SecondaryIndexRecord<attributeKey,primaryKey>::Key & k);

	void setKey(const attributeKey& a , const primaryKey & p);

	void read(char ** input);
	void write(char ** output);
	unsigned int size()const;

	void setAttribute(const attributeKey& a )const;
    void setPrimary(const primaryKey& a )const;
    const attributeKey& getAttribute();
    const primaryKey& getPrimary();
    SecondaryIndexRecord<attributeKey,primaryKey>& operator=(const SecondaryIndexRecord<attributeKey,primaryKey>&);
    static const bool isVariable=true;

	virtual ~SecondaryIndexRecord();
};

template<class attributeKey,class primaryKey>
SecondaryIndexRecord<attributeKey,primaryKey>::SecondaryIndexRecord(char ** input){
	key_=new Key(input);
}

template<class attributeKey,class primaryKey>
SecondaryIndexRecord<attributeKey,primaryKey>::SecondaryIndexRecord(const SecondaryIndexRecord<attributeKey,primaryKey> & rec){
	key_=new Key(*((Key*)rec.key_));
}
template<class attributeKey,class primaryKey>
SecondaryIndexRecord<attributeKey,primaryKey>::SecondaryIndexRecord(const attributeKey& a , const primaryKey & p){
	key_=new Key(a,p);
}

template<class attributeKey,class primaryKey>
const typename SecondaryIndexRecord<attributeKey,primaryKey>::Key &
SecondaryIndexRecord<attributeKey,primaryKey>::getKey()const{
	return *((Key*)key_);
}

template<class attributeKey,class primaryKey>
void SecondaryIndexRecord<attributeKey,primaryKey>::setKey(const SecondaryIndexRecord<attributeKey,primaryKey>::Key & k){
	((Key*)key_)->setKey(k);
}

template<class attributeKey,class primaryKey>
void SecondaryIndexRecord<attributeKey,primaryKey>::setKey(const attributeKey& a , const primaryKey & p){
	((Key*)key_)->setKey(a,p);
}

template<class attributeKey,class primaryKey>
void SecondaryIndexRecord<attributeKey,primaryKey>::read(char ** input){
	key_->read(input);
}
template<class attributeKey,class primaryKey>
void SecondaryIndexRecord<attributeKey,primaryKey>::write(char ** output){
	key_->write(output);
}

template<class attributeKey,class primaryKey>
unsigned int SecondaryIndexRecord<attributeKey,primaryKey>::size()const{
	return key_->size();
}

template<class attributeKey,class primaryKey>
void SecondaryIndexRecord<attributeKey,primaryKey>::setAttribute(const attributeKey& a )const{
	((Key*)key_)->setAttribute(a);
}

template<class attributeKey,class primaryKey>
void SecondaryIndexRecord<attributeKey,primaryKey>::setPrimary(const primaryKey& p )const{
	((Key*)key_)->setPrimary(p);
}

template<class attributeKey,class primaryKey>
const attributeKey& SecondaryIndexRecord<attributeKey,primaryKey>::getAttribute(){
	return ((Key*)key_)->getAttribute();
}

template<class attributeKey,class primaryKey>
const primaryKey& SecondaryIndexRecord<attributeKey,primaryKey>::getPrimary(){
	return ((Key*)key_)->getPrimary();
}

template<class attributeKey,class primaryKey>
SecondaryIndexRecord<attributeKey,primaryKey>& SecondaryIndexRecord<attributeKey,primaryKey>::operator=(const SecondaryIndexRecord<attributeKey,primaryKey>& rec){
	if(this==&rec)
		return *this;
	key_=new Key(*((Key*)rec.key_));

}

template<class attributeKey,class primaryKey>
SecondaryIndexRecord<attributeKey,primaryKey>::~SecondaryIndexRecord(){

}

#endif /* SECONDARYINDEXRECORD_H_ */

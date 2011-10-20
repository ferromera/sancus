/*
 * PrimaryIndexRecord.h
 *
 *  Created on: 14/10/2011
 *      Author: fernando
 */

#ifndef PRIMARYINDEXRECORD_H_
#define PRIMARYINDEXRECORD_H_

#include "Record.h"

template<class primaryKey>
class PrimaryIndexRecord: public Record{
	unsigned int blockNumber_;
public:
	class Key: public primaryKey{
	public:
		Key():primaryKey(){}
		Key(char** input):primaryKey(input){}
		Key(const Key & k):primaryKey(k){}
		Key(const primaryKey & k):primaryKey(k){}
		Key& operator=(const Key &k){
			primaryKey::operator=(k);
			return *this;
		}
		~Key(){}
	};
	PrimaryIndexRecord(char **);
	PrimaryIndexRecord(const PrimaryIndexRecord &);
	PrimaryIndexRecord(const primaryKey&, unsigned int block);

	const typename PrimaryIndexRecord<primaryKey>::Key & getKey()const;
	void setKey(const PrimaryIndexRecord<primaryKey>::Key & k);
	void setKey(const primaryKey& p );

	void read(char ** input);
	void write(char ** output);
	unsigned int size()const;

	void setBlockNumber(unsigned int blockNumber );
	unsigned int getBlockNumber();

	const primaryKey& getPrimary();
	PrimaryIndexRecord<primaryKey>& operator=(const PrimaryIndexRecord<primaryKey>&);
	static const bool isVariable=true;

	virtual ~PrimaryIndexRecord(){}


};

template<class primaryKey>
PrimaryIndexRecord<primaryKey>::PrimaryIndexRecord(char ** input){
	key_= new Key(input);
	memcpy(&blockNumber_,*input,4);
	(*input)+=4;
}

template<class primaryKey>
PrimaryIndexRecord<primaryKey>::PrimaryIndexRecord(const PrimaryIndexRecord<primaryKey> & rec){
	key_=new Key(rec.getKey());
	blockNumber_=rec.blockNumber_;
}
template<class primaryKey>
PrimaryIndexRecord<primaryKey>::PrimaryIndexRecord(const primaryKey& key, unsigned int block){
	blockNumber_=block;
	key_=new Key(key);
}

template<class primaryKey>
const typename PrimaryIndexRecord<primaryKey>::Key & PrimaryIndexRecord<primaryKey>::getKey()const{
	return *((Key*)key_);
}
template<class primaryKey>
void PrimaryIndexRecord<primaryKey>::setKey(const PrimaryIndexRecord<primaryKey>::Key & k){
	delete key_;
	key_=new Key(k);
}
template<class primaryKey>
void PrimaryIndexRecord<primaryKey>::setKey(const primaryKey& p ){
	delete key_;
	key_=new Key(p);
}

template<class primaryKey>
void PrimaryIndexRecord<primaryKey>::read(char ** input){
	key_->read(input);
	memcpy(&blockNumber_,*input,4);
	(*input)+=4;
}

template<class primaryKey>
void PrimaryIndexRecord<primaryKey>::write(char ** output){
	key_->write(output);
	memcpy(*output,&blockNumber_,4);
	(*output)+=4;
}

template<class primaryKey>
unsigned int  PrimaryIndexRecord<primaryKey>::size()const{
	return key_->size()+4;
}

template<class primaryKey>
void PrimaryIndexRecord<primaryKey>::setBlockNumber(unsigned int blockNumber ){
	blockNumber_=blockNumber;
}

template<class primaryKey>
unsigned int PrimaryIndexRecord<primaryKey>::getBlockNumber(){
	return blockNumber_;
}

template<class primaryKey>
const primaryKey& PrimaryIndexRecord<primaryKey>::getPrimary(){
	return *((primaryKey*)key_);
}

template<class primaryKey>
PrimaryIndexRecord<primaryKey>& PrimaryIndexRecord<primaryKey>::operator=(const PrimaryIndexRecord<primaryKey>& rec){
	if(this==&rec)
		return *this;
	setKey(rec.getKey());
	blockNumber_=rec.blockNumber_;
	return *this;
}




#endif /* PRIMARYINDEXRECORD_H_ */

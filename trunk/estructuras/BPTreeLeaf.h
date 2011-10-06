/*
 * BPTreeLeaf.h
 *
 *  Created on: 14/09/2011
 *      Author: fernando
 */

#include "BPTreeNode.h"
#include "stdint.h"
#include <list>



template<class TRecord,unsigned int blockSize>
class BPTreeLeaf: public BPTreeNode<TRecord,blockSize>{
protected:
	std::list<TRecord> records_;
	uint32_t next_;
	typename std::list<TRecord>::iterator search (const TRecord &);
public:
	BPTreeLeaf(File & file);
	BPTreeLeaf(File & file,unsigned long blockNumber);


	const typename std::list<TRecord>& getRecords();
	uint32_t next()const;
	void next(uint32_t n);
	virtual BPTreeLeaf<TRecord,blockSize> * nextLeaf()=0;
	bool isLeaf()const;
	virtual void clear();
	~BPTreeLeaf();

};



template<class TRecord,unsigned int blockSize>
BPTreeLeaf<TRecord,blockSize>::BPTreeLeaf(File & file):
BPTreeNode<TRecord,blockSize>(file),next_(0){
	BPTreeNode<TRecord,blockSize>::level_=0;
}

template<class TRecord,unsigned int blockSize>
BPTreeLeaf<TRecord,blockSize>::BPTreeLeaf(File & file,unsigned long blockNumber):
BPTreeNode<TRecord,blockSize>(file,blockNumber),next_(0){
	BPTreeNode<TRecord,blockSize>::level_=0;
}

template<class TRecord,unsigned int blockSize>
typename std::list<TRecord>::iterator BPTreeLeaf<TRecord,blockSize>::search (const TRecord & rec){
	typename std::list<TRecord>::iterator it;
	for(it=records_.begin();it!=records_.end();it++){
       if(rec <= (*it))
	       break;
	}
	return it;
}

template<class TRecord,unsigned int blockSize>
bool BPTreeLeaf<TRecord,blockSize>::isLeaf()const{
	return true;
}
template<class TRecord,unsigned int blockSize>
const typename std::list<TRecord> &  BPTreeLeaf<TRecord,blockSize>::getRecords()
{
	return records_;
}
template<class TRecord,unsigned int blockSize>
uint32_t  BPTreeLeaf<TRecord,blockSize>::next()const
{
	return next_;
}
template<class TRecord,unsigned int blockSize>
void  BPTreeLeaf<TRecord,blockSize>::next(uint32_t n)
{
	next_=n;
}

template<class TRecord,unsigned int blockSize>
void  BPTreeLeaf<TRecord,blockSize>::clear(){
	records_.clear();
}

template<class TRecord,unsigned int blockSize>
BPTreeLeaf<TRecord,blockSize>::~BPTreeLeaf(){}

/*
 * BPTreeLeaf.h
 *
 *  Created on: 14/09/2011
 *      Author: fernando
 */

#include "BPTreeNode.h"
#include "stdint.h"
#include <list>



template<class Record,unsigned int blockSize>
class BPTreeLeaf: public BPTreeNode<Record,blockSize>{
protected:
	std::list<Record> records_;
	uint32_t next_;
	unsigned int capacity_;
	typename std::list<Record>::iterator search (const Record &);
public:
	BPTreeLeaf(File & file);
	BPTreeLeaf(File & file,unsigned long blockNumber);

	BPTreeLeaf(unsigned int capacity,File & file);
	BPTreeLeaf(unsigned int capacity,File & file,unsigned long blockNumber);

	virtual BPTreeLeaf<Record,blockSize> * nextLeaf()=0;
	bool isLeaf()const;
	~BPTreeLeaf();

};



template<class Record,unsigned int blockSize>
BPTreeLeaf<Record,blockSize>::BPTreeLeaf(File & file):
BPTreeNode<Record,blockSize>(file),next_(0){
	BPTreeNode<Record,blockSize>::level_=0;
}

template<class Record,unsigned int blockSize>
BPTreeLeaf<Record,blockSize>::BPTreeLeaf(File & file,unsigned long blockNumber):
BPTreeNode<Record,blockSize>(file,blockNumber),next_(0){
	BPTreeNode<Record,blockSize>::level_=0;
}

template<class Record,unsigned int blockSize>
BPTreeLeaf<Record,blockSize>::BPTreeLeaf(unsigned int capacity,File & file):
BPTreeNode<Record,blockSize>(file),next_(0),capacity_(capacity){
	BPTreeNode<Record,blockSize>::level_=0;
}

template<class Record,unsigned int blockSize>
BPTreeLeaf<Record,blockSize>::BPTreeLeaf(unsigned int capacity,File & file,unsigned long blockNumber):
BPTreeNode<Record,blockSize>(file,blockNumber),next_(0),capacity_(capacity){
	BPTreeNode<Record,blockSize>::level_=0;
}


template<class Record,unsigned int blockSize>
typename std::list<Record>::iterator BPTreeLeaf<Record,blockSize>::search (const Record & rec){
	typename std::list<Record>::iterator it;
	for(it=records_.begin();it!=records_.end();it++){
       if(rec <= (*it))
	       break;
	}
	return it;
}

template<class Record,unsigned int blockSize>
bool BPTreeLeaf<Record,blockSize>::isLeaf()const{
	return true;
}

template<class Record,unsigned int blockSize>
BPTreeLeaf<Record,blockSize>::~BPTreeLeaf(){}

/*
 * BPTreeLeaf.cpp
 *
 *  Created on: 14/09/2011
 *      Author: fernando
 */

#include "BPTreeLeaf.h"

using namespace std;

template<class Record,unsigned int blockSize>
BPTreeLeaf<Record,blockSize>::BPTreeLeaf(File & file):
BPTreeNode<Record,blockSize>(file),next_(0){
	level_=0;
}

template<class Record,unsigned int blockSize>
BPTreeLeaf<Record,blockSize>::BPTreeLeaf(File & file,unsigned int pos):
BPTreeNode<Record,blockSize>(file,pos),next_(0){
	level_=0;
}

template<class Record,unsigned int blockSize>
BPTreeLeaf<Record,blockSize>::BPTreeLeaf(unsigned int capacity,File & file):
BPTreeNode<Record,blockSize>(file),next_(0),capacity_(capacity){
	level_=0;
}

template<class Record,unsigned int blockSize>
BPTreeLeaf<Record,blockSize>::BPTreeLeaf(unsigned int capacity,File & file,unsigned int pos):
BPTreeNode<Record,blockSize>(file,pos),next_(0),capacity_(capacity){
	level_=0;
}


template<class Record,unsigned int blockSize>
typename list<Record>::iterator BPTreeLeaf<Record,blockSize>::search (const Record & rec){
	typename list<Record>::iterator it;
	for(it=records_.begin();it<records_.end();it++){
       if(rec <= (**it))
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

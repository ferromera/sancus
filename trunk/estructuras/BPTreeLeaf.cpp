/*
 * BPTreeLeaf.cpp
 *
 *  Created on: 14/09/2011
 *      Author: fernando
 */

#include "BPTreeLeaf.h"

using namespace std;

template<class Record,unsigned int blockSize>
BPTreeLeaf<Record,blockSize>::BPTreeLeaf():next_(0){}

template<class Record,unsigned int blockSize>
BPTreeLeaf<Record,blockSize>::BPTreeLeaf(unsigned int capacity):next_(0),capacity_(capacity){}

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
BPTreeLeaf<Record,blockSize>::~BPTreeLeaf(){}

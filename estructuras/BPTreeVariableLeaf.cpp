/*
 * BPTreeVariableLeaf.cpp
 *
 *  Created on: 14/09/2011
 *      Author: fernando
 */

#include "BPTreeVariableLeaf.h"

using namespace std;

template<class Record,unsigned int blockSize>
BPTreeVariableLeaf<Record,blockSize>::BPTreeVariableLeaf(unsigned int capacity,File & file):
BPTreeLeaf<Record,blockSize>(capacity,file){
}

template<class Record,unsigned int blockSize>
BPTreeVariableLeaf<Record,blockSize>::BPTreeVariableLeaf(unsigned int capacity,File & file,unsigned int pos):
BPTreeLeaf<Record,blockSize>(capacity,file,pos){
}

template<class Record,unsigned int blockSize>
void BPTreeVariableLeaf<Record,blockSize>::read(){
	// No Implementado
}

template<class Record,unsigned int blockSize>
void BPTreeVariableLeaf<Record,blockSize>::write(){
	// No Implementado
}

template<class Record,unsigned int blockSize>
void BPTreeVariableLeaf<Record,blockSize>::insert(Record &){
	// No Implementado
}

template<class Record,unsigned int blockSize>
void BPTreeVariableLeaf<Record,blockSize>::remove(Record &){
	// No Implementado
}
template<class Record,unsigned int blockSize>
BPTreeVariableLeaf<Record,blockSize>::~BPTreeVariableLeaf(){
}

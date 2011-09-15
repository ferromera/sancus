/*
 * BPTreeVariableLeaf.h
 *
 *  Created on: 14/09/2011
 *      Author: fernando
 */

#include "BPTreeLeaf.h"
#include "BPTreeVariableLeafBlock.h"

template<class Record,unsigned int blockSize>
class BPTreeVariableLeaf: public BPTreeLeaf<Record,blockSize>{
	std::list<unsigned int> blockNumbers;
	void readRecords(BPTreeVariableLeafBlock<blockSize> *);
public:
	BPTreeVariableLeaf(unsigned int capacity,File & file);
	BPTreeVariableLeaf(unsigned int capacity,File & file,unsigned int pos);
	void read();
	void write();
	void insert(Record &);
	void remove(Record &);
	~BPTreeVariableLeaf();

};

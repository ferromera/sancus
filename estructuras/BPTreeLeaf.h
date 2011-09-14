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
	BPTreeLeaf();
	BPTreeLeaf(unsigned int capacity);
	virtual BPTreeLeaf * nextLeaf()=0;
	~BPTreeLeaf();

};

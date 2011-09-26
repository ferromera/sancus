/*
 * BPTreeInternalNode.h
 *
 *  Created on: 24/09/2011
 *      Author: juanmromera
 */

#ifndef BPTREEINTERNALNODE_H_
#define BPTREEINTERNALNODE_H_

#include "BPTreeNode.h"
#include "Record.h"
#include <list>

template<class Record,unsigned int blockSize>
class BPTreeInternalNode: public BPTreeNode<Record, blockSize> {
protected:
	typename std::list<typename Record::Key> keys_;
	std::list<unsigned int> children_;
	unsigned int capacity_;
	unsigned int leafCapacity_;
	typename std::list<typename Record::Key>::iterator search(const typename Record::Key & key);
	std::list<unsigned int>::iterator getLeftChild(const typename Record::Key &);
	std::list<unsigned int>::iterator getRightChild(const typename Record::Key &);
public:
	BPTreeInternalNode(unsigned int level,File & file);
	BPTreeInternalNode(File & file,unsigned long blockNumber);

	BPTreeInternalNode(unsigned int capacity,unsigned int leafCapacity_,unsigned int level,File & file);
	BPTreeInternalNode(unsigned int capacity,unsigned int leafCapacity_,File & file,unsigned long blockNumber);

	bool isLeaf()const;
	~BPTreeInternalNode();
};

template<class Record,unsigned int blockSize>
BPTreeInternalNode<Record,blockSize>::BPTreeInternalNode(unsigned int level,File & file):BPTreeNode<Record,blockSize>(file)
{
	BPTreeNode<Record,blockSize>::level_ = level;
}
template<class Record,unsigned int blockSize>
BPTreeInternalNode<Record,blockSize>::BPTreeInternalNode(File & file, unsigned long blockNumber):
BPTreeNode<Record,blockSize>(file,blockNumber)
{
}
template<class Record,unsigned int blockSize>
BPTreeInternalNode<Record,blockSize>::BPTreeInternalNode(unsigned int capacity, unsigned int leafCapacity_,unsigned int level,File & file):
BPTreeNode<Record,blockSize>(file),capacity_(capacity),leafCapacity_(leafCapacity_)
{
	BPTreeNode<Record,blockSize>::level_ = level;
}

template<class Record,unsigned int blockSize>
BPTreeInternalNode<Record,blockSize>::BPTreeInternalNode(unsigned int capacity,unsigned int leafCapacity_, File & file,unsigned long blockNumber):
BPTreeNode<Record,blockSize>(file,blockNumber),capacity_(capacity),leafCapacity_(leafCapacity_)
{
}
template<class Record,unsigned int blockSize>
bool BPTreeInternalNode<Record,blockSize>::isLeaf()const{
	return false;
}
template<class Record,unsigned int blockSize>
typename std::list<typename Record::Key>::iterator BPTreeInternalNode<Record,blockSize>::search( const typename Record::Key & key)
{
		typename std::list<typename Record::Key>::iterator it;
		for(it=keys_.begin();it!=keys_.end();it++){
	       if(key <= (*it))
		       break;
		}
		return it;

}
template<class Record,unsigned int blockSize>
std::list<unsigned int>::iterator BPTreeInternalNode<Record,blockSize>::getLeftChild(const typename Record::Key & key)
{
	typename std::list<typename Record::Key>::iterator itKeys;
	typename std::list<unsigned int>::iterator itChildren = children_.begin();
	for(itKeys=keys_.begin();itKeys!=keys_.end();itKeys++,itChildren++){
       if(key <= (*itKeys))
	       break;
	}

	return itChildren;
}
template<class Record,unsigned int blockSize>
std::list<unsigned int>::iterator BPTreeInternalNode<Record,blockSize>::getRightChild(const typename Record::Key & key)
{

	typename std::list<typename Record::Key>::iterator itKeys;
	typename std::list<unsigned int>::iterator itChildren = children_.begin();
	for(itKeys=keys_.begin();itKeys!=keys_.end();itKeys++,itChildren++){
       if(key <= (*itKeys))
	       break;
	}
	itChildren++;
	return itChildren;
}

template<class Record,unsigned int blockSize>
BPTreeInternalNode<Record,blockSize>::~BPTreeInternalNode(){}



#endif /* BPTREEINTERNALNODE_H_ */

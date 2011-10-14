/*
 * BPTreeInternalNode.h
 *
 *  Created on: 24/09/2011
 *      Author: juanmromera
 */

#ifndef BPTREEINTERNALNODE_H_
#define BPTREEINTERNALNODE_H_

#include "BPTreeNode.h"
#include "../records/Record.h"
#include <list>

template<class TRecord,unsigned int blockSize>
class BPTreeInternalNode: public BPTreeNode<TRecord, blockSize> {
protected:
	typename std::list<typename TRecord::Key> keys_;
	std::list<unsigned int> children_;

	typename std::list<typename TRecord::Key>::iterator itSearch(const typename TRecord::Key & key);
	std::list<unsigned int>::iterator getLeftChild(const typename TRecord::Key &);
	std::list<unsigned int>::iterator getRightChild(const typename TRecord::Key &);
public:
	BPTreeInternalNode();
	BPTreeInternalNode(unsigned int level,File & file);
	BPTreeInternalNode(File & file,unsigned long blockNumber);

	const typename std::list<typename TRecord::Key>& getKeys()const;
	const std::list<unsigned int>& getChildren()const;
	void setFirstChild(unsigned int);
	unsigned int getFirstChild();
	virtual void clear();

	bool isLeaf()const;
	~BPTreeInternalNode();
};

template<class TRecord,unsigned int blockSize>
BPTreeInternalNode<TRecord,blockSize>::BPTreeInternalNode():BPTreeNode<TRecord,blockSize>()
{
	BPTreeNode<TRecord,blockSize>::level_ = 1;
}

template<class TRecord,unsigned int blockSize>
BPTreeInternalNode<TRecord,blockSize>::BPTreeInternalNode(unsigned int level,File & file):BPTreeNode<TRecord,blockSize>(file)
{
	BPTreeNode<TRecord,blockSize>::level_ = level;
}
template<class TRecord,unsigned int blockSize>
BPTreeInternalNode<TRecord,blockSize>::BPTreeInternalNode(File & file, unsigned long blockNumber):
BPTreeNode<TRecord,blockSize>(file,blockNumber)
{
}

template<class TRecord,unsigned int blockSize>
bool BPTreeInternalNode<TRecord,blockSize>::isLeaf()const{
	return false;
}
template<class TRecord,unsigned int blockSize>
typename std::list<typename TRecord::Key>::iterator BPTreeInternalNode<TRecord,blockSize>::itSearch( const typename TRecord::Key & key)
{
		typename std::list<typename TRecord::Key>::iterator it;
		for(it=keys_.begin();it!=keys_.end();it++){
	       if(key <= (*it))
		       break;
		}
		return it;

}
template<class TRecord,unsigned int blockSize>
std::list<unsigned int>::iterator BPTreeInternalNode<TRecord,blockSize>::getLeftChild(const typename TRecord::Key & key)
{
	typename std::list<typename TRecord::Key>::iterator itKeys;
	typename std::list<unsigned int>::iterator itChildren = children_.begin();
	for(itKeys=keys_.begin();itKeys!=keys_.end();itKeys++,itChildren++){
       if(key <= (*itKeys))
	       break;
	}

	return itChildren;
}
template<class TRecord,unsigned int blockSize>
std::list<unsigned int>::iterator BPTreeInternalNode<TRecord,blockSize>::getRightChild(const typename TRecord::Key & key)
{

	typename std::list<typename TRecord::Key>::iterator itKeys;
	typename std::list<unsigned int>::iterator itChildren = children_.begin();
	for(itKeys=keys_.begin();itKeys!=keys_.end();itKeys++,itChildren++){
       if(key <= (*itKeys))
	       break;
	}
	itChildren++;
	return itChildren;
}

template<class TRecord,unsigned int blockSize>
const typename std::list<typename TRecord::Key>& BPTreeInternalNode<TRecord,blockSize>::getKeys()const{
	return keys_;
}
template<class TRecord,unsigned int blockSize>
const std::list<unsigned int>& BPTreeInternalNode<TRecord,blockSize>::getChildren()const{
	return children_;
}

template<class TRecord,unsigned int blockSize>
void BPTreeInternalNode<TRecord,blockSize>::setFirstChild(unsigned int child){
	if(!children_.empty())
		children_.pop_front();
	children_.push_front(child);
}

template<class TRecord,unsigned int blockSize>
unsigned int BPTreeInternalNode<TRecord,blockSize>::getFirstChild(){
	return children_.front();
}

template<class TRecord,unsigned int blockSize>
void BPTreeInternalNode<TRecord,blockSize>::clear(){
	keys_.clear();
	children_.clear();
}

template<class TRecord,unsigned int blockSize>
BPTreeInternalNode<TRecord,blockSize>::~BPTreeInternalNode(){}



#endif /* BPTREEINTERNALNODE_H_ */

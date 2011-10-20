/*
 * BPTreeLeaf.h
 *
 *  Created on: 14/09/2011
 *      Author: fernando
 */

#include "BPTreeNode.h"
#include "stdint.h"
#include <list>
#include <iostream>

template<class TRecord, unsigned int blockSize>
class BPTreeLeaf: public BPTreeNode<TRecord, blockSize> {
protected:
	std::list<TRecord> records_;
	uint32_t next_;
	typename std::list<TRecord>::iterator searchIterator;
	typename std::list<TRecord>::iterator itSearch(const TRecord &);
public:
	BPTreeLeaf(File & file);
	BPTreeLeaf(File & file, unsigned long blockNumber);
	BPTreeLeaf(const BPTreeLeaf<TRecord, blockSize> &);

	const typename std::list<TRecord>& getRecords();
	uint32_t next() const;
	void next(uint32_t n);
	virtual BPTreeLeaf<TRecord, blockSize> * nextLeaf()=0;
	bool isLeaf() const;
	virtual void clear();
	TRecord * search(const TRecord & rec);
	TRecord * nextRecord();
	void saveToSequential(File &, VariableSequentialBlock<blockSize>*, char**);
	const TRecord & getFirstRecord() const;
	const TRecord & getLastRecord() const;
	~BPTreeLeaf();

};

template<class TRecord, unsigned int blockSize>
BPTreeLeaf<TRecord, blockSize>::BPTreeLeaf(File & file) :
	BPTreeNode<TRecord, blockSize> (file), next_(0) {
	BPTreeNode<TRecord, blockSize>::logKey = "Hoja ";
	char intStr[20];
	sprintf(intStr, "%lu", BPTreeNode<TRecord, blockSize>::blockNumber_);
	BPTreeNode<TRecord, blockSize>::logKey.append(intStr);
	BPTreeNode<TRecord, blockSize>::logKey.append(" :");
	BPTreeNode<TRecord, blockSize>::level_ = 0;
	searchIterator = records_.begin();
}

template<class TRecord, unsigned int blockSize>
BPTreeLeaf<TRecord, blockSize>::BPTreeLeaf(File & file, unsigned long blockNumber) :
	BPTreeNode<TRecord, blockSize> (file, blockNumber), next_(0) {
	BPTreeNode<TRecord, blockSize>::logKey = "Hoja ";
	char intStr[20];
	sprintf(intStr, "%lu", BPTreeNode<TRecord, blockSize>::blockNumber_);
	BPTreeNode<TRecord, blockSize>::logKey.append(intStr);
	BPTreeNode<TRecord, blockSize>::logKey.append(" :");
	BPTreeNode<TRecord, blockSize>::level_ = 0;
	searchIterator = records_.begin();
}

template<class TRecord, unsigned int blockSize>
BPTreeLeaf<TRecord, blockSize>::BPTreeLeaf(const BPTreeLeaf<TRecord, blockSize> &leaf) :
	BPTreeNode<TRecord, blockSize> (leaf), records_(leaf.records_), next_(leaf.next_) {
	BPTreeNode<TRecord, blockSize>::logKey = "Hoja ";
	char intStr[20];
	sprintf(intStr, "%lu", BPTreeNode<TRecord, blockSize>::blockNumber_);
	BPTreeNode<TRecord, blockSize>::logKey.append(intStr);
	BPTreeNode<TRecord, blockSize>::logKey.append(" :");
	searchIterator = records_.begin();
	typename std::list<TRecord>::const_iterator it = leaf.records_.begin();
	for (; it != leaf.searchIterator; it++, searchIterator++)
		;

}

template<class TRecord, unsigned int blockSize>
typename std::list<TRecord>::iterator BPTreeLeaf<TRecord, blockSize>::itSearch(const TRecord & rec) {
	typename std::list<TRecord>::iterator it;
	for (it = records_.begin(); it != records_.end(); it++) {
		if (rec <= (*it))
			break;
	}
	return it;
}

template<class TRecord, unsigned int blockSize>
bool BPTreeLeaf<TRecord, blockSize>::isLeaf() const {
	return true;
}
template<class TRecord, unsigned int blockSize>
const typename std::list<TRecord> & BPTreeLeaf<TRecord, blockSize>::getRecords() {
	return records_;
}
template<class TRecord, unsigned int blockSize>
uint32_t BPTreeLeaf<TRecord, blockSize>::next() const {
	return next_;
}
template<class TRecord, unsigned int blockSize>
void BPTreeLeaf<TRecord, blockSize>::next(uint32_t n) {
	next_ = n;
}

template<class TRecord, unsigned int blockSize>
void BPTreeLeaf<TRecord, blockSize>::clear() {
	records_.clear();
}

template<class TRecord, unsigned int blockSize>
TRecord * BPTreeLeaf<TRecord, blockSize>::search(const TRecord & rec) {
	BPTreeNode<TRecord, blockSize>::logString = "Buscando registro con clave : ";
	if (TRecord::Key::isString)
		BPTreeNode<TRecord, blockSize>::logString.append(rec.getKey().getString());
	else {
		char intStr[20];
		sprintf(intStr, "%u", rec.getKey().getUint());
		BPTreeNode<TRecord, blockSize>::logString.append(intStr);
	}
	searchIterator = itSearch(rec);

	if (searchIterator == records_.end())
		throw LeafRecordNotFoundException();
	TRecord * found = new TRecord(*searchIterator);
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Registro encontrado.");
	return found;
}

template<class TRecord, unsigned int blockSize>
TRecord * BPTreeLeaf<TRecord, blockSize>::nextRecord() {
	searchIterator++;
	if (searchIterator == records_.end())
		throw LeafRecordNotFoundException();
	TRecord * found = new TRecord(*searchIterator);
	return found;

}

template<class TRecord, unsigned int blockSize>
void BPTreeLeaf<TRecord, blockSize>::saveToSequential(File & file, VariableSequentialBlock<blockSize>* block,
		char ** lastPosition) {
	typename std::list<TRecord>::iterator it = records_.begin();

	for (; it != records_.end(); it++) {
		if (block->freeSpace < it->size()) {
			file.write((char*) block, blockSize);
			block->freeSpace = blockSize - VARIABLE_SEQUENTIAL_CONTROL_BYTES;
			(*lastPosition) = block->bytes;
		}
		it->write(lastPosition);
		block->freeSpace -= it->size();
	}

}

template<class TRecord, unsigned int blockSize>
const TRecord & BPTreeLeaf<TRecord, blockSize>::getFirstRecord() const {
	return records_.front();
}

template<class TRecord, unsigned int blockSize>
const TRecord & BPTreeLeaf<TRecord, blockSize>::getLastRecord() const {
	return records_.back();
}

template<class TRecord, unsigned int blockSize>
BPTreeLeaf<TRecord, blockSize>::~BPTreeLeaf() {
}

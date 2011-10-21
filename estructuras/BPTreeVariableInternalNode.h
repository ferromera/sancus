/*
 * BPTreeVariableInternalNode.h
 *
 *  Created on: 24/09/2011
 *      Author: juanmromera
 */

#ifndef BPTREEVARIABLEINTERNALNODE_H_
#define BPTREEVARIABLEINTERNALNODE_H_

#include "BPTreeInternalNode.h"
#include "BPTreeVariableNodeBlock.h"
#include "BPTreeVariableLeaf.h"
#include <cstring>

using namespace std;

template<class TRecord, unsigned int blockSize>
class BPTreeVariableInternalNode: public BPTreeInternalNode<TRecord, blockSize> {
	unsigned int freeSpace;

	void readRecords(BPTreeVariableNodeBlock<blockSize> *);
	void writeRecords(BPTreeVariableNodeBlock<blockSize> *);
	void handleLeafOverflow(BPTreeVariableLeaf<TRecord, blockSize> *, const TRecord &);
	void handleNodeOverflow(BPTreeVariableInternalNode<TRecord, blockSize>*,
			NodeOverflowException<typename TRecord::Key> );
	void handleLoadLeafOverflow(BPTreeVariableLeaf<TRecord, blockSize> *, const TRecord &, float);
	void handleLoadNodeOverflow(BPTreeVariableInternalNode<TRecord, blockSize>*,
			NodeOverflowException<typename TRecord::Key> , float);
	void handleLeafUnderflow(BPTreeVariableLeaf<TRecord, blockSize>*,
			typename std::list<typename TRecord::Key>::iterator, std::list<unsigned int>::iterator);
	void handleNodeUnderflow(BPTreeVariableInternalNode<TRecord, blockSize>*,
			typename std::list<typename TRecord::Key>::iterator, std::list<unsigned int>::iterator);
public:
	BPTreeVariableInternalNode();
	BPTreeVariableInternalNode(unsigned int level, File & file);
	BPTreeVariableInternalNode(File & file, unsigned long blockNumber);

	void read();
	void write();
	void insert(const TRecord &);
	void loadInsert(const TRecord &, float);
	void remove(const TRecord &);
	void insertInNode(typename TRecord::Key &, unsigned int);
	void loadInsertInNode(typename TRecord::Key &, unsigned int, float);
	void removeInNode(typename TRecord::Key &);
	void clear();
	unsigned int usedSpace();
	void free();
	TRecord * search(const TRecord & rec, BPTreeVariableLeaf<TRecord, blockSize> **);
	void update(const TRecord &);
	void preOrderReport(File & reportFile, unsigned int);
	void saveToSequential(File & file, VariableSequentialBlock<blockSize>*block, char** lastPosicion);

	virtual ~BPTreeVariableInternalNode() {
	}
};

template<class TRecord, unsigned int blockSize>
BPTreeVariableInternalNode<TRecord, blockSize>::BPTreeVariableInternalNode() :
	BPTreeInternalNode<TRecord, blockSize> () {
	BPTreeNode<TRecord, blockSize>::logKey = "Nodo Interno ";
	char intStr[20];
	sprintf(intStr, "%lu", BPTreeNode<TRecord, blockSize>::blockNumber_);
	BPTreeNode<TRecord, blockSize>::logKey.append(intStr);
	BPTreeNode<TRecord, blockSize>::logKey.append(" :");

	freeSpace = blockSize - VARIABLE_NODE_CONTROL_BYTES;
}

template<class TRecord, unsigned int blockSize>
BPTreeVariableInternalNode<TRecord, blockSize>::BPTreeVariableInternalNode(unsigned int level, File & file) :
	BPTreeInternalNode<TRecord, blockSize> (level, file) {
	BPTreeNode<TRecord, blockSize>::logKey = "Nodo Interno ";
	char intStr[20];
	sprintf(intStr, "%lu", BPTreeNode<TRecord, blockSize>::blockNumber_);
	BPTreeNode<TRecord, blockSize>::logKey.append(intStr);
	BPTreeNode<TRecord, blockSize>::logKey.append(" :");

	freeSpace = blockSize - VARIABLE_NODE_CONTROL_BYTES;
}

template<class TRecord, unsigned int blockSize>
BPTreeVariableInternalNode<TRecord, blockSize>::BPTreeVariableInternalNode(File & file, unsigned long blockNumber) :
	BPTreeInternalNode<TRecord, blockSize> (file, blockNumber) {
	BPTreeNode<TRecord, blockSize>::logKey = "Nodo Interno ";
	char intStr[20];
	sprintf(intStr, "%lu", BPTreeNode<TRecord, blockSize>::blockNumber_);
	BPTreeNode<TRecord, blockSize>::logKey.append(intStr);
	BPTreeNode<TRecord, blockSize>::logKey.append(" :");

	read();
}
template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::read() {
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Leyendo nodo.");

	if (BPTreeNode<TRecord, blockSize>::isFree_)
		throw ReadInAFreeNodeException();

	BPTreeVariableNodeBlock<blockSize> * block = new BPTreeVariableNodeBlock<blockSize> ;

	File * file = BPTreeNode<TRecord, blockSize>::file_;
	unsigned long & blockNumber = BPTreeNode<TRecord, blockSize>::blockNumber_;

	BPTreeInternalNode<TRecord, blockSize>::keys_.clear();

	unsigned long pos = blockNumber * blockSize;
	file->seek(pos, File::BEG);
	file->read((char*) block, blockSize);
	BPTreeNode<TRecord, blockSize>::level_ = block->level;
	BPTreeInternalNode<TRecord, blockSize>::children_.push_back(block->leftChild);

	freeSpace = block->freeSpace;
	readRecords(block);

	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Nodo leido.");

}
template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::readRecords(BPTreeVariableNodeBlock<blockSize> *block) {
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Leyendo claves.");
	char * currentPos = block->bytes;
	unsigned int readSpace = 0;
	unsigned int usedSpace = blockSize - VARIABLE_NODE_CONTROL_BYTES - freeSpace;

	while (usedSpace > readSpace) {
		typename TRecord::Key * newKey = new typename TRecord::Key();
		newKey->read(&currentPos);
		BPTreeInternalNode<TRecord, blockSize>::keys_.push_back(*newKey);
		unsigned int rightChild;
		memcpy(&rightChild, currentPos, 4);
		currentPos += 4;
		BPTreeInternalNode<TRecord, blockSize>::children_.push_back(rightChild);
		readSpace += 4 + newKey->size();
		delete newKey;
	}
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Claves leidas.");
}
template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::write() {
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Escribiendo nodo.");

	if (BPTreeNode<TRecord, blockSize>::isFree_)
		throw WriteInAFreeNodeException();
	BPTreeVariableNodeBlock<blockSize> * block = new BPTreeVariableNodeBlock<blockSize> ;
	File * file = BPTreeNode<TRecord, blockSize>::file_;

	block->freeSpace = freeSpace;
	block->level = BPTreeNode<TRecord, blockSize>::level_;
	block->leftChild = BPTreeInternalNode<TRecord, blockSize>::children_.front();

	writeRecords(block);
	file->seek(BPTreeNode<TRecord, blockSize>::blockNumber_ * blockSize, File::BEG);
	file->write(block, blockSize);

	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Nodo escrito.");

}
template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::writeRecords(BPTreeVariableNodeBlock<blockSize> * block) {
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Escribiendo claves.");

	typename std::list<typename TRecord::Key>::iterator itKeys = BPTreeInternalNode<TRecord, blockSize>::keys_.begin();
	std::list<unsigned int>::iterator itChildren = BPTreeInternalNode<TRecord, blockSize>::children_.begin();
	itChildren++;

	char * currentPos = block->bytes;
	while (itKeys != BPTreeInternalNode<TRecord, blockSize>::keys_.end()) {
		itKeys->write(&currentPos);
		memcpy(currentPos, &(*itChildren), 4);
		currentPos += 4;
		itKeys++;
		itChildren++;
	}
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Claves escritas.");
}
template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::insert(const TRecord & record) {
	BPTreeNode<TRecord, blockSize>::logString = "Insertando registro con clave : ";
	if (TRecord::Key::isString)
		BPTreeNode<TRecord, blockSize>::logString.append(record.getKey().getString());
	else {
		char intStr[20];
		sprintf(intStr, "%u", record.getKey().getUint());
		BPTreeNode<TRecord, blockSize>::logString.append(intStr);
	}
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey,
			BPTreeNode<TRecord, blockSize>::logString);

	typename TRecord::Key key = record.getKey();
	//Busco la clave en la lista
	typename std::list<typename TRecord::Key>::iterator itKey = BPTreeInternalNode<TRecord, blockSize>::itSearch(key);
	std::list<unsigned int>::iterator itChildren;

	//Obtengo el hijo donde se debera insertar

	if (itKey == BPTreeInternalNode<TRecord, blockSize>::keys_.end()) {
		itKey--;
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getRightChild(*itKey);
	} else if (*itKey == key)
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getRightChild(*itKey);
	else
		// (*itKey)>key
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getLeftChild(*itKey);

	if (BPTreeNode<TRecord, blockSize>::level_ == 1) {
		BPTreeVariableLeaf<TRecord, blockSize> *childLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (
				*BPTreeNode<TRecord, blockSize>::file_, *itChildren);

		try {
			childLeaf->insert(record);
			childLeaf->write();
			delete childLeaf;
		} catch (LeafOverflowException ovException) {
			handleLeafOverflow(childLeaf, record);

		} catch (LeafUnicityException exception) {
			delete childLeaf;
			throw;
		}
	} else //level > 1
	{
		BPTreeVariableInternalNode<TRecord, blockSize>
				* childNode = new BPTreeVariableInternalNode<TRecord, blockSize> (
						*BPTreeNode<TRecord, blockSize>::file_, *itChildren);
		try {
			childNode->insert(record);
			delete childNode;
		} catch (NodeOverflowException<typename TRecord::Key> ovException) {
			handleNodeOverflow(childNode, ovException);

		} catch (LeafUnicityException exception) {
			delete childNode;
			throw;
		}

	}
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Registro insertado.");

}
template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::loadInsert(const TRecord & record, float compression) {
	typename TRecord::Key key = record.getKey();
	//Busco la clave en la lista
	typename std::list<typename TRecord::Key>::iterator itKey = BPTreeInternalNode<TRecord, blockSize>::itSearch(key);
	std::list<unsigned int>::iterator itChildren;

	//Obtengo el hijo donde se debera insertar

	if (itKey == BPTreeInternalNode<TRecord, blockSize>::keys_.end()) {
		itKey--;
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getRightChild(*itKey);
	} else if (*itKey == key)
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getRightChild(*itKey);
	else
		// (*itKey)>key
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getLeftChild(*itKey);

	if (BPTreeNode<TRecord, blockSize>::level_ == 1) {
		BPTreeVariableLeaf<TRecord, blockSize> *childLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (
				*BPTreeNode<TRecord, blockSize>::file_, *itChildren);

		try {
			childLeaf->loadInsert(record, compression);
			childLeaf->write();
			delete childLeaf;
		} catch (LeafOverflowException ovException) {
			handleLoadLeafOverflow(childLeaf, record, compression);
			return;
		}
	} else //level > 1
	{
		BPTreeVariableInternalNode<TRecord, blockSize>
				* childNode = new BPTreeVariableInternalNode<TRecord, blockSize> (
						*BPTreeNode<TRecord, blockSize>::file_, *itChildren);
		try {
			childNode->loadInsert(record, compression);
			delete childNode;
		} catch (NodeOverflowException<typename TRecord::Key> ovException) {
			handleLoadNodeOverflow(childNode, ovException, compression);
		}
	}
}

template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::handleLeafOverflow(
		BPTreeVariableLeaf<TRecord, blockSize> * childLeaf, const TRecord & record) {
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Manejando Overflow de hoja.");

	typename std::list<TRecord> recordList = childLeaf->getRecords();
	typename std::list<TRecord>::iterator recordIt = recordList.begin();

	BPTreeVariableLeaf<TRecord, blockSize> *newLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (
			*BPTreeNode<TRecord, blockSize>::file_);

	unsigned int halfSize = (blockSize - VARIABLE_NODE_CONTROL_BYTES) / 2;

	for (; recordIt != recordList.end(); recordIt++)
		if ((*recordIt) > record)
			break;
	recordList.insert(recordIt, record);

	childLeaf->clear();

	recordIt = recordList.begin();
	for (unsigned int currSize = 0; currSize < halfSize; recordIt++) {
		childLeaf->insert(*recordIt);
		currSize += recordIt->size();
	}
	for (; recordIt != recordList.end(); recordIt++)
		newLeaf->insert(*recordIt);

	newLeaf->next(childLeaf->next());
	childLeaf->next(newLeaf->blockNumber());
	childLeaf->write();
	newLeaf->write();

	TRecord firstRecord = newLeaf->getFirstRecord();
	typename TRecord::Key newKey = firstRecord.getKey();
	unsigned int newBlockNumber = newLeaf->blockNumber();
	delete newLeaf;
	delete childLeaf;

	insertInNode(newKey, newBlockNumber);
	write();
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Overflow de hoja resuelto.");
}
template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::handleNodeOverflow(
		BPTreeVariableInternalNode<TRecord, blockSize>*childNode,
		NodeOverflowException<typename TRecord::Key> ovException) {
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey,
			"Manejando Overflow de nodo interno.");

	BPTreeVariableInternalNode<TRecord, blockSize>* newNode = new BPTreeVariableInternalNode<TRecord, blockSize> (
			BPTreeNode<TRecord, blockSize>::level_ - 1, *BPTreeNode<TRecord, blockSize>::file_);

	typename std::list<typename TRecord::Key> childKeys = childNode->getKeys();
	std::list<unsigned int> childChildren = childNode->getChildren();

	//Busco donde voy a insertar la clave que subio y donde voy a insertar su hijo derecho.
	typename std::list<typename TRecord::Key>::iterator itKey = childKeys.begin();
	std::list<unsigned int>::iterator itChildren = childChildren.begin();
	for (itChildren++; itKey != childKeys.end(); itKey++, itChildren++) {
		if (ovException.key <= (*itKey))
			break;
	}
	childKeys.insert(itKey, ovException.key);
	childChildren.insert(itChildren, ovException.child);

	unsigned int halfSize = (blockSize - VARIABLE_NODE_CONTROL_BYTES) / 2;
	childNode->clear();
	itKey = childKeys.begin();
	itChildren = childChildren.begin();
	childNode->setFirstChild(*itChildren);
	itChildren++;
	for (unsigned int currSize = 0; currSize < halfSize; itKey++, itChildren++) {
		childNode->insertInNode(*itKey, *itChildren);
		currSize += itKey->size() + 4;
	}
	typename TRecord::Key middleKey = *itKey;
	itKey++;
	newNode->setFirstChild(*itChildren);
	itChildren++;
	for (; itKey != childKeys.end(); itKey++, itChildren++)
		newNode->insertInNode(*itKey, *itChildren);

	childNode->write();
	newNode->write();

	unsigned int newBlockNumber = newNode->blockNumber();
	delete newNode;
	delete childNode;

	insertInNode(middleKey, newBlockNumber);
	write();

	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey,
			"Overflow de nodo interno resuelto.");
}

template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::handleLoadLeafOverflow(
		BPTreeVariableLeaf<TRecord, blockSize> *childLeaf, const TRecord & record, float compression) {

	BPTreeVariableLeaf<TRecord, blockSize> *newLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (
			*BPTreeNode<TRecord, blockSize>::file_);

	newLeaf->insert(record);

	newLeaf->next(childLeaf->next());
	childLeaf->next(newLeaf->blockNumber());
	childLeaf->write();
	newLeaf->write();

	typename TRecord::Key newKey = record.getKey();
	unsigned int newBlockNumber = newLeaf->blockNumber();
	delete newLeaf;
	delete childLeaf;

	loadInsertInNode(newKey, newBlockNumber, compression);
	write();

}

template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::handleLoadNodeOverflow(
		BPTreeVariableInternalNode<TRecord, blockSize>*childNode,
		NodeOverflowException<typename TRecord::Key> ovException, float compression) {

	BPTreeVariableInternalNode<TRecord, blockSize>* newNode = new BPTreeVariableInternalNode<TRecord, blockSize> (
			BPTreeNode<TRecord, blockSize>::level_ - 1, *BPTreeNode<TRecord, blockSize>::file_);

	typename std::list<typename TRecord::Key> childKeys = childNode->getKeys();
	std::list<unsigned int> childChildren = childNode->getChildren();

	//Busco donde voy a insertar la clave que subio y donde voy a insertar su hijo derecho.
	typename std::list<typename TRecord::Key>::iterator itKey = --childKeys.end();
	std::list<unsigned int>::iterator itChildren = --childChildren.end();

	childNode->removeInNode(*itKey);

	typename TRecord::Key middleKey = *itKey;

	newNode->setFirstChild(*itChildren);

	newNode->insertInNode(ovException.key, ovException.child);

	childNode->write();
	newNode->write();

	unsigned int newBlockNumber = newNode->blockNumber();
	delete newNode;
	delete childNode;

	loadInsertInNode(middleKey, newBlockNumber, compression);
	write();

}

template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::insertInNode(typename TRecord::Key & key, unsigned int child) {
	BPTreeNode<TRecord, blockSize>::logString = "Insertando en nodo clave : ";
	if (TRecord::Key::isString)
		BPTreeNode<TRecord, blockSize>::logString.append(key.getString());
	else {
		char intStr[20];
		sprintf(intStr, "%u", key.getUint());
		BPTreeNode<TRecord, blockSize>::logString.append(intStr);
	}
	BPTreeNode<TRecord, blockSize>::logString = " con hijo derecho : ";
	char intStr[20];
	sprintf(intStr, "%u", child);
	BPTreeNode<TRecord, blockSize>::logString.append(intStr);
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey,
			BPTreeNode<TRecord, blockSize>::logString);

	if (freeSpace < key.size() + 4)
		throw NodeOverflowException<typename TRecord::Key> (child, key);

	typename std::list<typename TRecord::Key>::iterator itKey = BPTreeInternalNode<TRecord, blockSize>::itSearch(key);

	typename std::list<unsigned int>::iterator itChildren;

	if (itKey != BPTreeInternalNode<TRecord, blockSize>::keys_.end())
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getRightChild(*itKey);
	else
		itChildren = BPTreeInternalNode<TRecord, blockSize>::children_.end();

	BPTreeInternalNode<TRecord, blockSize>::keys_.insert(itKey, key);
	BPTreeInternalNode<TRecord, blockSize>::children_.insert(itChildren, child);
	freeSpace -= key.size() + 4;

	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Clave e hijo insertados.");

}

template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::loadInsertInNode(typename TRecord::Key & key, unsigned int child,
		float compression) {
	if ((float) freeSpace < ((float) (blockSize - VARIABLE_NODE_CONTROL_BYTES)) * (1.0 - compression)
			+ (float) (key.size() + 4))
		throw NodeOverflowException<typename TRecord::Key> (child, key);

	typename std::list<typename TRecord::Key>::iterator itKey = BPTreeInternalNode<TRecord, blockSize>::itSearch(key);

	typename std::list<unsigned int>::iterator itChildren;

	if (itKey != BPTreeInternalNode<TRecord, blockSize>::keys_.end())
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getRightChild(*itKey);
	else
		itChildren = BPTreeInternalNode<TRecord, blockSize>::children_.end();

	BPTreeInternalNode<TRecord, blockSize>::keys_.insert(itKey, key);
	BPTreeInternalNode<TRecord, blockSize>::children_.insert(itChildren, child);
	freeSpace -= key.size() + 4;

}

template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::remove(const TRecord & record) {
	BPTreeNode<TRecord, blockSize>::logString = "Borrando registro con clave : ";
	if (TRecord::Key::isString)
		BPTreeNode<TRecord, blockSize>::logString.append(record.getKey().getString());
	else {
		char intStr[20];
		sprintf(intStr, "%u", record.getKey().getUint());
		BPTreeNode<TRecord, blockSize>::logString.append(intStr);
	}
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey,
			BPTreeNode<TRecord, blockSize>::logString);

	typename TRecord::Key key = record.getKey();
	//Busco la clave en la lista
	typename std::list<typename TRecord::Key>::iterator itKey = BPTreeInternalNode<TRecord, blockSize>::itSearch(key);
	std::list<unsigned int>::iterator itChildren;

	//Obtengo el hijo donde voy a  borrar

	if (itKey == BPTreeInternalNode<TRecord, blockSize>::keys_.end()) {
		itKey--;
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getRightChild(*itKey);
		itKey++;
	} else if (*itKey == key) {
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getRightChild(*itKey);
		itKey++;
	} else { // (*itKey)>key
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getLeftChild(*itKey);
	}
	//itKey queda en el padre derecho del hijo por el que bajo.

	if (BPTreeNode<TRecord, blockSize>::level_ == 1) {
		BPTreeVariableLeaf<TRecord, blockSize> *childLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (
				*BPTreeNode<TRecord, blockSize>::file_, *itChildren);

		try {
			childLeaf->remove(record);
			childLeaf->write();
			delete childLeaf;

		} catch (LeafUnderflowException ufException) {
			handleLeafUnderflow(childLeaf, itKey, itChildren);

		}
	} else //level > 1
	{
		BPTreeVariableInternalNode<TRecord, blockSize>
				* childNode = new BPTreeVariableInternalNode<TRecord, blockSize> (
						*BPTreeNode<TRecord, blockSize>::file_, *itChildren);
		try {
			childNode->remove(record);
			childNode->write();
			delete childNode;
		} catch (NodeUnderflowException ufException) {
			handleNodeUnderflow(childNode, itKey, itChildren);
		}
	}
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Registro borrado.");
}
template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::handleLeafUnderflow(
		BPTreeVariableLeaf<TRecord, blockSize>* childLeaf, typename std::list<typename TRecord::Key>::iterator itKey,
		std::list<unsigned int>::iterator itChildren) {

	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Manejando underflow de hoja.");

	BPTreeVariableLeaf<TRecord, blockSize>* sibling;
	std::list<unsigned int>::iterator itSibling = itChildren;
	itSibling++;
	bool rightSibling = true;
	if (itSibling == BPTreeInternalNode<TRecord, blockSize>::children_.end()) {
		//No hay hermano derecho, tomo el izquierdo
		itSibling--;
		itSibling--;
		//Hago que itKey apunte al padre izquierdo del hijo por el que bajo.
		itKey--;
		rightSibling = false;
	}
	sibling = new BPTreeVariableLeaf<TRecord, blockSize> (*BPTreeNode<TRecord, blockSize>::file_, *itSibling);
	typename std::list<TRecord> mixedRecords = childLeaf->getRecords();
	typename std::list<TRecord> siblingRecords = sibling->getRecords();

	if (rightSibling) {
		//agrego al final los records del hermano.
		mixedRecords.insert(mixedRecords.end(), siblingRecords.begin(), siblingRecords.end());
	} else {
		//agrego al principio los records del hermano.
		mixedRecords.insert(mixedRecords.begin(), siblingRecords.begin(), siblingRecords.end());
	}

	if (sibling->hasMinimumCapacity()) {
		//fusion
		BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Aplicando fusion.");
		if (rightSibling) {
			childLeaf->next(sibling->next());
			sibling->free();
			childLeaf->clear();
			typename std::list<TRecord>::iterator itMixedRecords = mixedRecords.begin();
			for (; itMixedRecords != mixedRecords.end(); itMixedRecords++)
				childLeaf->insert(*itMixedRecords);
			childLeaf->write();
		} else {
			sibling->next(childLeaf->next());
			childLeaf->free();
			sibling->clear();
			typename std::list<TRecord>::iterator itMixedRecords = mixedRecords.begin();
			for (; itMixedRecords != mixedRecords.end(); itMixedRecords++)
				sibling->insert(*itMixedRecords);
			sibling->write();
		}

		//remuevo la clave de la hoja que libere.
		delete childLeaf;
		delete sibling;
		removeInNode(*itKey);
		write();
		return;
	} else {
		//balanceo
		BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Aplicando balanceo.");
		childLeaf->clear();
		sibling->clear();
		unsigned int childBytes = 0, siblingBytes = 0;
		typename std::list<TRecord>::iterator itFront = mixedRecords.begin();
		typename std::list<TRecord>::iterator itBack = --mixedRecords.end();
		while (itFront != itBack) {
			if (childBytes <= siblingBytes) {
				if (rightSibling) {
					childLeaf->insert(*itFront);
					childBytes += itFront->size();
					itFront++;
				} else {
					childLeaf->insert(*itBack);
					childBytes += itBack->size();
					itBack--;
				}
			} else {
				if (rightSibling) {
					sibling->insert(*itBack);
					siblingBytes += itBack->size();
					itBack--;
				} else {
					sibling->insert(*itFront);
					siblingBytes += itFront->size();
					itFront++;
				}
			}
		}
		if (childBytes <= siblingBytes)
			childLeaf->insert(*itFront);
		else
			sibling->insert(*itFront);
		childLeaf->write();
		sibling->write();
		if (rightSibling)
			(*itKey) = sibling->getFirstRecord().getKey();
		else
			(*itKey) = childLeaf->getFirstRecord().getKey();
		delete childLeaf;
		delete sibling;
		write();

		BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey,
				"Underflow de hoja resuelto.");
		return;

	}
}
template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::handleNodeUnderflow(
		BPTreeVariableInternalNode<TRecord, blockSize>* childNode,
		typename std::list<typename TRecord::Key>::iterator itKey, std::list<unsigned int>::iterator itChildren) {

	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey,
			"Manejando underflow de nodo interno.");

	BPTreeVariableInternalNode<TRecord, blockSize>* sibling;
	std::list<unsigned int>::iterator itSibling = itChildren;
	itSibling++;
	bool rightSibling = true;
	if (itSibling == BPTreeInternalNode<TRecord, blockSize>::children_.end()) {
		//No hay hermano derecho, tomo el izquierdo
		itSibling--;
		itSibling--;
		//Hago que itKey apunte al padre izquierdo del hijo por el que bajo.
		itKey--;
		rightSibling = false;
	}
	sibling = new BPTreeVariableInternalNode<TRecord, blockSize> (*BPTreeNode<TRecord, blockSize>::file_, *itSibling);

	typename std::list<typename TRecord::Key> mixedKeys = childNode->getKeys();
	std::list<unsigned int> mixedChildren = childNode->getChildren();
	typename std::list<typename TRecord::Key> siblingKeys = sibling->getKeys();
	std::list<unsigned int> siblingChildren = sibling->getChildren();
	mixedKeys.push_back(*itKey);

	if (rightSibling) {
		//agrego al final las keys y los children del hermano.
		mixedKeys.insert(mixedKeys.end(), siblingKeys.begin(), siblingKeys.end());
		mixedChildren.insert(mixedChildren.end(), siblingChildren.begin(), siblingChildren.end());
	} else {
		//agrego al principio las keys y los children del hermano.
		mixedKeys.insert(mixedKeys.begin(), siblingKeys.begin(), siblingKeys.end());
		mixedChildren.insert(mixedChildren.begin(), siblingChildren.begin(), siblingChildren.end());
	}
	if (childNode->usedSpace() + sibling->usedSpace() + itKey->size() + 4 <= blockSize - VARIABLE_NODE_CONTROL_BYTES) {
		//fusion
		BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Aplicando fusion.");
		sibling->BPTreeNode<TRecord, blockSize>::free();
		childNode->clear();

		typename std::list<typename TRecord::Key>::iterator itMixedKeys = mixedKeys.begin();
		std::list<unsigned int>::iterator itMixedChildren = mixedChildren.begin();

		childNode->setFirstChild(*itMixedChildren);
		itMixedChildren++;
		for (; itMixedKeys != mixedKeys.end(); itMixedKeys++, itMixedChildren++)
			childNode->insertInNode(*itMixedKeys, *itMixedChildren);
		childNode->write();

		//remuevo la clave del hermano que libere.
		if (itKey == BPTreeInternalNode<TRecord, blockSize>::keys_.begin())
			setFirstChild(childNode->blockNumber());
		else if (!rightSibling)
			itKey--; //Posiciono al padre izquierdo del sibling.

		delete childNode;
		delete sibling;
		removeInNode(*itKey);
		write();
		return;
	} else {
		//balanceo
		BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Aplicando balanceo.");
		childNode->clear();
		sibling->clear();
		unsigned int childBytes = 0, siblingBytes = 0;
		typename std::list<typename TRecord::Key>::iterator itFrontKeys = mixedKeys.begin();
		typename std::list<typename TRecord::Key>::iterator itBackKeys = --mixedKeys.end();
		std::list<unsigned int>::iterator itFrontChildren = mixedChildren.begin();
		std::list<unsigned int>::iterator itBackChildren = --mixedChildren.end();
		if (rightSibling) {
			childNode->setFirstChild(*itFrontChildren);
			itFrontChildren++;
		} else {
			sibling->setFirstChild(*itFrontChildren);
			itFrontChildren++;
		}
		while (itFrontKeys != itBackKeys) {
			if (childBytes <= siblingBytes) {
				if (rightSibling) {
					childNode->insertInNode(*itFrontKeys, *itFrontChildren);
					childBytes += itFrontKeys->size() + 4;
					itFrontKeys++;
					itFrontChildren++;
				} else {
					childNode->insertInNode(*itBackKeys, *itBackChildren);
					childBytes += itBackKeys->size() + 4;
					itBackKeys++;
					itBackChildren++;
				}
			} else {
				if (rightSibling) {
					sibling->insertInNode(*itBackKeys, *itBackChildren);
					siblingBytes += itBackKeys->size() + 4;
					itBackKeys++;
					itBackChildren++;
				} else {
					sibling->insertInNode(*itFrontKeys, *itFrontChildren);
					siblingBytes += itFrontKeys->size() + 4;
					itFrontKeys++;
					itFrontChildren++;
				}
			}
		}

		if (rightSibling)
			sibling->setFirstChild(*itFrontChildren);
		else
			childNode->setFirstChild(*itFrontChildren);

		childNode->write();
		sibling->write();

		//guardo la mediana.
		(*itKey) = *itFrontKeys;

		delete childNode;
		delete sibling;
		write();
		BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey,
				"Underflow de nodo interno resuelto.");

	}
}
template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::removeInNode(typename TRecord::Key & key) {
	BPTreeNode<TRecord, blockSize>::logString = "Borrando en nodo clave : ";
	if (TRecord::Key::isString)
		BPTreeNode<TRecord, blockSize>::logString.append(key.getString());
	else {
		char intStr[20];
		sprintf(intStr, "%u", key.getUint());
		BPTreeNode<TRecord, blockSize>::logString.append(intStr);
	}
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey,
			BPTreeNode<TRecord, blockSize>::logString);

	typename std::list<typename TRecord::Key>::iterator itRemovePos = itSearch(key);
	if (itRemovePos == BPTreeInternalNode<TRecord, blockSize>::keys_.end() || (*itRemovePos) != key)
		throw NodeKeyNotFoundException();

	std::list<unsigned int>::iterator itRightChild = getRightChild(key);

	if (freeSpace > (blockSize - VARIABLE_NODE_CONTROL_BYTES) / 2) {
		freeSpace += key.size() + 4;
		BPTreeInternalNode<TRecord, blockSize>::children_.erase(itRightChild);
		BPTreeInternalNode<TRecord, blockSize>::keys_.erase(itRemovePos);
		throw NodeUnderflowException();
	}

	freeSpace += key.size() + 4;
	BPTreeInternalNode<TRecord, blockSize>::children_.erase(itRightChild);
	BPTreeInternalNode<TRecord, blockSize>::keys_.erase(itRemovePos);

	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Clave borrada.");

}
template<class TRecord, unsigned int blockSize>
unsigned int BPTreeVariableInternalNode<TRecord, blockSize>::usedSpace() {
	return blockSize - VARIABLE_NODE_CONTROL_BYTES - freeSpace;
}
template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::clear() {
	BPTreeInternalNode<TRecord, blockSize>::clear();
	freeSpace = blockSize - VARIABLE_NODE_CONTROL_BYTES;

}
template<class TRecord, unsigned int blockSize>
TRecord * BPTreeVariableInternalNode<TRecord, blockSize>::search(const TRecord & rec,
		BPTreeVariableLeaf<TRecord, blockSize> ** searchLeaf) {

	BPTreeNode<TRecord, blockSize>::logString = "Buscando registro con clave : ";
	if (TRecord::Key::isString)
		BPTreeNode<TRecord, blockSize>::logString.append(rec.getKey().getString());
	else {
		char intStr[20];
		sprintf(intStr, "%u", rec.getKey().getUint());
		BPTreeNode<TRecord, blockSize>::logString.append(intStr);
	}
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey,
			BPTreeNode<TRecord, blockSize>::logString);

	typename TRecord::Key key = rec.getKey();
	//Busco la clave en la lista
	typename std::list<typename TRecord::Key>::iterator itKey = BPTreeInternalNode<TRecord, blockSize>::itSearch(key);
	std::list<unsigned int>::iterator itChildren;

	//Obtengo el hijo donde voy a  buscar

	if (itKey == BPTreeInternalNode<TRecord, blockSize>::keys_.end()) {
		itKey--;
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getRightChild(*itKey);
	} else if (*itKey == key) {
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getRightChild(*itKey);
	} else { // (*itKey)>key
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getLeftChild(*itKey);
	}

	if (BPTreeNode<TRecord, blockSize>::level_ == 1) {
		TRecord * found;
		BPTreeVariableLeaf<TRecord, blockSize> *childLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (
				*BPTreeNode<TRecord, blockSize>::file_, *itChildren);
		try {
			found = childLeaf->search(rec);
			delete (*searchLeaf);
			*searchLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (*childLeaf);
			delete childLeaf;
			return found;
		} catch (LeafRecordNotFoundException e) {
			try {
				BPTreeVariableLeaf<TRecord, blockSize> *nxtLeaf =
						(BPTreeVariableLeaf<TRecord, blockSize> *) childLeaf->nextLeaf();
				found = nxtLeaf->search(rec);
				delete (*searchLeaf);
				*searchLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (*nxtLeaf);
				delete childLeaf;
				delete nxtLeaf;
				return found;
			} catch (ThereIsNoNextLeafException &) {
				delete childLeaf;
				throw;
			}
		}
	} else {
		BPTreeVariableInternalNode<TRecord, blockSize>
				*childNode = new BPTreeVariableInternalNode<TRecord, blockSize> (
						*BPTreeNode<TRecord, blockSize>::file_, *itChildren);
		try {
			TRecord * found = childNode->search(rec, searchLeaf);
			delete childNode;
			return found;
		} catch (ThereIsNoNextLeafException &e) {
			delete childNode;
			throw;
		}
	}
}
template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::update(const TRecord & record) {
	BPTreeNode<TRecord, blockSize>::logString = "Actualizando registro con clave : ";
	if (TRecord::Key::isString)
		BPTreeNode<TRecord, blockSize>::logString.append(record.getKey().getString());
	else {
		char intStr[20];
		sprintf(intStr, "%u", record.getKey().getUint());
		BPTreeNode<TRecord, blockSize>::logString.append(intStr);
	}
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey,
			BPTreeNode<TRecord, blockSize>::logString);

	typename TRecord::Key key = record.getKey();
	//Busco la clave en la lista
	typename std::list<typename TRecord::Key>::iterator itKey = BPTreeInternalNode<TRecord, blockSize>::itSearch(key);
	std::list<unsigned int>::iterator itChildren;

	//Obtengo el hijo donde voy a actualizar

	if (itKey == BPTreeInternalNode<TRecord, blockSize>::keys_.end()) {
		itKey--;
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getRightChild(*itKey);
		itKey++;
	} else if (*itKey == key) {
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getRightChild(*itKey);
		itKey++;
	} else { // (*itKey)>key
		itChildren = BPTreeInternalNode<TRecord, blockSize>::getLeftChild(*itKey);
	}
	//itKey queda en el padre derecho del hijo por el que bajo.
	if (BPTreeNode<TRecord, blockSize>::level_ == 1) {
		BPTreeVariableLeaf<TRecord, blockSize> *childLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (
				*BPTreeNode<TRecord, blockSize>::file_, *itChildren);
		try {
			childLeaf->update(record);
			childLeaf->write();
			delete childLeaf;

		} catch (LeafUnderflowException ufException) {
			handleLeafUnderflow(childLeaf, itKey, itChildren);

		} catch (LeafOverflowException ovException) {
			handleLeafOverflow(childLeaf, record);

		}
	} else //level > 1
	{
		BPTreeVariableInternalNode<TRecord, blockSize>
				* childNode = new BPTreeVariableInternalNode<TRecord, blockSize> (
						*BPTreeNode<TRecord, blockSize>::file_, *itChildren);
		try {
			childNode->update(record);
			delete childNode;

		} catch (NodeUnderflowException ufException) {
			handleNodeUnderflow(childNode, itKey, itChildren);
		} catch (NodeOverflowException<typename TRecord::Key> ovException) {
			handleNodeOverflow(childNode, ovException);
		}
	}
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Registro Actualizado.");

}
template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::preOrderReport(File & reportFile, unsigned int treeLevel) {
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Reportando en pre orden.");

	for (unsigned int i = BPTreeNode<TRecord, blockSize>::level_; i < treeLevel; i++)
		reportFile << "|\t";
	reportFile << "Node " << BPTreeNode<TRecord, blockSize>::blockNumber_ << " : ";
	reportFile << "(" << BPTreeNode<TRecord, blockSize>::level_ << ")(" << freeSpace << ") -- ";
	std::list<unsigned int>::iterator itChildren = BPTreeInternalNode<TRecord, blockSize>::children_.begin();
	typename std::list<typename TRecord::Key>::iterator itKeys = BPTreeInternalNode<TRecord, blockSize>::keys_.begin();
	reportFile << *itChildren;
	for (itChildren++; itChildren != BPTreeInternalNode<TRecord, blockSize>::children_.end(); itChildren++, itKeys++) {
		reportFile << "(" << itKeys->getKey() << ")" << *itChildren;
	}
	reportFile << "\n";
	if (BPTreeNode<TRecord, blockSize>::level_ == 1) {
		BPTreeVariableLeaf<TRecord, blockSize> * leaf;
		itChildren = BPTreeInternalNode<TRecord, blockSize>::children_.begin();
		for (; itChildren != BPTreeInternalNode<TRecord, blockSize>::children_.end(); itChildren++) {
			leaf = new BPTreeVariableLeaf<TRecord, blockSize> (*BPTreeNode<TRecord, blockSize>::file_, *itChildren);
			leaf->preOrderReport(reportFile, treeLevel);
			delete leaf;
		}
	} else {
		BPTreeVariableInternalNode<TRecord, blockSize> * internal;
		itChildren = BPTreeInternalNode<TRecord, blockSize>::children_.begin();
		for (; itChildren != BPTreeInternalNode<TRecord, blockSize>::children_.end(); itChildren++) {
			internal = new BPTreeVariableInternalNode<TRecord, blockSize> (*BPTreeNode<TRecord, blockSize>::file_,
					*itChildren);
			internal->preOrderReport(reportFile, treeLevel);
			delete internal;
		}
	}
	BPTreeNode<TRecord, blockSize>::log->insert(BPTreeNode<TRecord, blockSize>::logKey, "Nodo reportado.");
}

template<class TRecord, unsigned int blockSize>
void BPTreeVariableInternalNode<TRecord, blockSize>::saveToSequential(File & file,
		VariableSequentialBlock<blockSize>*block, char** lastPosicion) {
	std::list<unsigned int>::iterator itChildren;

	if (BPTreeNode<TRecord, blockSize>::level_ == 1) {
		BPTreeVariableLeaf<TRecord, blockSize> * leaf;
		for (itChildren = BPTreeInternalNode<TRecord, blockSize>::children_.begin(); itChildren != BPTreeInternalNode<
				TRecord, blockSize>::children_.end(); itChildren++) {
			leaf = new BPTreeVariableLeaf<TRecord, blockSize> (*BPTreeNode<TRecord, blockSize>::file_, *itChildren);
			leaf->saveToSequential(file, block, lastPosicion);
			delete leaf;
		}
	} else {
		BPTreeVariableInternalNode<TRecord, blockSize> * node;
		for (itChildren = BPTreeInternalNode<TRecord, blockSize>::children_.begin(); itChildren != BPTreeInternalNode<
				TRecord, blockSize>::children_.end(); itChildren++) {
			node = new BPTreeVariableInternalNode<TRecord, blockSize> (*BPTreeNode<TRecord, blockSize>::file_,
					*itChildren);
			node->saveToSequential(file, block, lastPosicion);
			delete node;
		}
	}

}

#endif /* BPTREEVARIABLEINTERNALNODE_H_ */

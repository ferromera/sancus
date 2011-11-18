#ifndef BPLUSTREE_H_INCLUDED
#define BPLUSTREE_H_INCLUDED

#include "BPlusTreeExceptions.h"
#include "BPTreeVariableLeaf.h"
#include "BPTreeVariableInternalNode.h"
#include "VariableSequentialBlock.h"
#include "../utils/Logger.h"
#include "File.h"
#include <list>
#include <string>
#include <iostream>

#define BPTREE_LOAD_COMPRESSION_RATIO 0.8

template<class TRecord, unsigned int blockSize>
class BPlusTree {
private:
	BPTreeNode<TRecord, blockSize> * root;
	BPTreeVariableLeaf<TRecord, blockSize> * searchLeaf;
	File *file_;
	std::string dataPath;
	std::string reportPath;
	TRecord * found;
	Logger* log;
	std::string logString;
	std::string logKey;
	BPlusTree() {
	}
	void create();
	void load();
	void loadFromSequential(const std::string &);
	void saveToSequential(const std::string &);
	void handleLeafOverflow(const TRecord & rec);
	void handleNodeOverflow(const TRecord & rec, NodeOverflowException<typename TRecord::Key> );
	void handleLoadLeafOverflow(const TRecord & rec);
	void handleLoadNodeOverflow(const TRecord & rec, NodeOverflowException<typename TRecord::Key> );
	void handleNodeUnderflow();
	void loadInsert(const TRecord & rec, float);

public:

	BPlusTree(const std::string & path);
	BPlusTree(const std::string & treePath, const std::string & sequentialPath);

	void insert(const TRecord & rec);
	void remove(const TRecord & rec);
	const TRecord & search(const TRecord & rec);
	void update(const TRecord & rec);
	const TRecord & next();
	void preOrderReport();
	void restructure();
	virtual ~BPlusTree() {
		delete root;
		delete searchLeaf;
		delete file_;
		delete found;
	}
};

template<class TRecord, unsigned int blockSize>
BPlusTree<TRecord, blockSize>::BPlusTree(const std::string & path) :
	root(NULL), searchLeaf(NULL), file_(NULL), dataPath(path), found(NULL) {
	log = Logger::getInstance();
	logKey = "Arbol B+ (";
	logKey.append(dataPath);
	logKey.append("):");
	reportPath = dataPath;
	std::string::iterator it = reportPath.end();
	for (; (*it) != '.' && (*it) != '/'; it--)
		;
	if (*it == '.')
		reportPath.erase(it, reportPath.end());
	reportPath.append("_report.txt");

	try {
		load();
	} catch (OpenFileException & e) {
		log->insert(logKey, "El archivo fallo al abrirse.");
		create();
	}

}
template<class TRecord, unsigned int blockSize>
BPlusTree<TRecord, blockSize>::BPlusTree(const std::string & treePath, const std::string & sequentialPath) :
	root(NULL), searchLeaf(NULL), file_(NULL), dataPath(treePath), found(NULL) {
	log = Logger::getInstance();
	logKey = "Arbol B+ (";
	logKey.append(dataPath);
	logKey.append("):");
	reportPath = dataPath;
	std::string::iterator it = reportPath.end;
	for (; (*it) != '.' && (*it) != '/'; it--)
		;
	if (*it == '.')
		reportPath.erase(it, reportPath.end());
	reportPath.append("_report.txt");

	loadFromSequential(sequentialPath);

}
template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::create() {
	delete file_;
	delete root;
	delete searchLeaf;

	logString = "Intentando crear archivo: ";
	logString.append(dataPath);
	log->insert(logKey, logString);

	file_ = new File(dataPath, File::NEW | File::BIN | File::IO);

	log->insert(logKey, "El archivo se creo correctamente.");

	FreeSpaceStackBlock<blockSize> * fblock = new FreeSpaceStackBlock<blockSize> ;
	fblock->blockNumber = 1;
	fblock->inFile = 0;
	file_->write((char*) fblock, blockSize);
	delete fblock;

	root = new BPTreeVariableLeaf<TRecord, blockSize> (*file_);
	root->write();
	searchLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (*file_, 1);

}
template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::load() {
	delete file_;
	delete root;
	delete searchLeaf;

	logString = "Intentando abrir archivo: ";
	logString.append(dataPath);
	log->insert(logKey, logString);

	file_ = new File(dataPath, File::BIN | File::IO);

	log->insert(logKey, "El archivo se abrio correctamente.");

	BPTreeVariableLeafBlock<blockSize> *block = new BPTreeVariableLeafBlock<blockSize> ;
	file_->seek(blockSize, File::BEG);
	file_->read((char*) block, blockSize);
	if (block->level == 0)
		root = new BPTreeVariableLeaf<TRecord, blockSize> (*file_, 1);
	else
		root = new BPTreeVariableInternalNode<TRecord, blockSize> (*file_, 1);
	delete block;

}
template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::loadFromSequential(const std::string & sequentialPath) {
	create();
	File sequentialFile(sequentialPath, File::BIN | File::IO);

	VariableSequentialBlock<blockSize> * seqBlock = new VariableSequentialBlock<blockSize> ;
	unsigned int freeSpace;
	char * bytes;
	TRecord * rec;
	while (1) {
		try {
			sequentialFile.read((char*) seqBlock, blockSize);
		} catch (EndOfFileException e) {
			delete seqBlock;
			return;
		}
		freeSpace = blockSize - VARIABLE_SEQUENTIAL_CONTROL_BYTES;
		bytes = seqBlock->bytes;
		while (freeSpace > seqBlock->freeSpace) {
			rec = new TRecord(&bytes);
			loadInsert(*rec, BPTREE_LOAD_COMPRESSION_RATIO);
			freeSpace -= rec->size();
			delete rec;
		}

	}
	delete seqBlock;

}

template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::insert(const TRecord & rec) {
	delete searchLeaf;
	searchLeaf = NULL;
	logString = "Insertando registro con clave: ";
	if (TRecord::Key::isString)
		logString.append(rec.getKey().getString());
	else {
		char intStr[20];
		sprintf(intStr, "%u", rec.getKey().getUint());
		logString.append(intStr);
	}
	log->insert(logKey, logString);
	try {
		root->insert(rec);
		if(root->level()==0)
			root->write();
	} catch (LeafOverflowException e) {
		handleLeafOverflow(rec);
	} catch (NodeOverflowException<typename TRecord::Key> e) {
		handleNodeOverflow(rec, e);
	}
}
template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::loadInsert(const TRecord & rec, float compression) {
	delete searchLeaf;
	searchLeaf = NULL;
	if (TRecord::Key::isString)
		logString.append(rec.getKey().getString());
	else {
		char intStr[20];
		sprintf(intStr, "%u", rec.getKey().getUint());
		logString.append(intStr);
	}
	log->insert(logKey, logString);
	try {
		root->loadInsert(rec, compression);
	} catch (LeafOverflowException e) {
		handleLoadLeafOverflow(rec);
	} catch (NodeOverflowException<typename TRecord::Key> e) {
		handleLoadNodeOverflow(rec, e);
	}
}
template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::remove(const TRecord & rec) {
	delete searchLeaf;
	searchLeaf = NULL;
	logString = "Borrando registro con clave: ";
	if (TRecord::Key::isString)
		logString.append(rec.getKey().getString());
	else {
		char intStr[20];
		sprintf(intStr, "%u", rec.getKey().getUint());
		logString.append(intStr);
	}
	log->insert(logKey, logString);
	try {
		root->remove(rec);
		if(root->level()==0)
			root->write();
	} catch (LeafUnderflowException e) {
		root->write();
	} catch (NodeUnderflowException e) {
		if (((BPTreeVariableInternalNode<TRecord, blockSize>*) root)->usedSpace() == 0)
			handleNodeUnderflow();
		else
			root->write();
	}
}
template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::handleLeafOverflow(const TRecord & record) {

	BPTreeVariableLeaf<TRecord, blockSize>* rootAsLeaf = (BPTreeVariableLeaf<TRecord, blockSize>*) root;

	typename std::list<TRecord> recordList = rootAsLeaf->getRecords();
	typename std::list<TRecord>::iterator recordIt = recordList.begin();

	BPTreeVariableLeaf<TRecord, blockSize> *leftLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (*file_);
	BPTreeVariableLeaf<TRecord, blockSize> *rightLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (*file_);

	unsigned int halfSize = (blockSize - VARIABLE_NODE_CONTROL_BYTES) / 2;

	for (; recordIt != recordList.end(); recordIt++)
		if ((*recordIt) > record)
			break;
	recordList.insert(recordIt, record);

	recordIt = recordList.begin();
	for (unsigned int currSize = 0; currSize < halfSize; recordIt++) {
		leftLeaf->insert(*recordIt);
		currSize += recordIt->size();
	}
	for (; recordIt != recordList.end(); recordIt++)
		rightLeaf->insert(*recordIt);

	rightLeaf->next(0);
	leftLeaf->next(rightLeaf->blockNumber());
	leftLeaf->write();
	rightLeaf->write();

	TRecord firstRecord = rightLeaf->getFirstRecord();
	typename TRecord::Key newKey = firstRecord.getKey();
	unsigned int leftBlockNumber = leftLeaf->blockNumber();
	unsigned int rightBlockNumber = rightLeaf->blockNumber();
	delete rightLeaf;
	delete leftLeaf;

	delete root;

	root = new BPTreeVariableInternalNode<TRecord, blockSize> ();
	BPTreeVariableInternalNode<TRecord, blockSize>* rootAsInternal =
			(BPTreeVariableInternalNode<TRecord, blockSize>*) root;

	root->file(*file_);
	root->level(1);
	rootAsInternal->setFirstChild(leftBlockNumber);
	rootAsInternal->insertInNode(newKey, rightBlockNumber);
	root->write();

}
template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::handleNodeOverflow(const TRecord & rec,
		NodeOverflowException<typename TRecord::Key> ovException) {

	BPTreeVariableInternalNode<TRecord, blockSize>* leftNode = new BPTreeVariableInternalNode<TRecord, blockSize> (
			root->level(), *file_);
	BPTreeVariableInternalNode<TRecord, blockSize>* rightNode = new BPTreeVariableInternalNode<TRecord, blockSize> (
			root->level(), *file_);

	BPTreeVariableInternalNode<TRecord, blockSize>* castedRoot = (BPTreeVariableInternalNode<TRecord, blockSize>*) root;
	typename std::list<typename TRecord::Key> childKeys = castedRoot->getKeys();
	std::list<unsigned int> childChildren = castedRoot->getChildren();

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
	itKey = childKeys.begin();
	itChildren = childChildren.begin();
	leftNode->setFirstChild(*itChildren);
	itChildren++;
	for (unsigned int currSize = 0; currSize < halfSize; itKey++, itChildren++) {
		leftNode->insertInNode(*itKey, *itChildren);
		currSize += itKey->size() + 4;
	}
	typename TRecord::Key middleKey = *itKey;
	itKey++;
	rightNode->setFirstChild(*itChildren);
	itChildren++;
	for (; itKey != childKeys.end(); itKey++, itChildren++)
		rightNode->insertInNode(*itKey, *itChildren);

	leftNode->write();
	rightNode->write();

	unsigned int leftBlockNumber = leftNode->blockNumber();
	unsigned int rightBlockNumber = rightNode->blockNumber();
	delete leftNode;
	delete rightNode;

	unsigned int newLevel = root->level() + 1;
	delete root;

	root = new BPTreeVariableInternalNode<TRecord, blockSize> ();
	BPTreeVariableInternalNode<TRecord, blockSize>* rootAsInternal =
			(BPTreeVariableInternalNode<TRecord, blockSize>*) root;
	root->file(*file_);
	root->level(newLevel);
	rootAsInternal->setFirstChild(leftBlockNumber);
	rootAsInternal->insertInNode(middleKey, rightBlockNumber);
	root->write();

}
template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::handleNodeUnderflow() {

	BPTreeVariableInternalNode<TRecord, blockSize>* rootAsInternal =
			(BPTreeVariableInternalNode<TRecord, blockSize>*) root;
	if (root->level() == 1) {
		BPTreeVariableLeaf<TRecord, blockSize>* leftLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (*file_,
				rootAsInternal->getFirstChild());
		leftLeaf->becomeRoot();
		leftLeaf->write();
		delete root;
		root = leftLeaf;

	} else {
		BPTreeVariableInternalNode<TRecord, blockSize>* leftNode = new BPTreeVariableInternalNode<TRecord, blockSize> (
				*file_, rootAsInternal->getFirstChild());
		leftNode->becomeRoot();
		leftNode->write();
		delete root;
		root = leftNode;
	}
}

template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::handleLoadLeafOverflow(const TRecord & rec) {

	BPTreeVariableLeaf<TRecord, blockSize>* rootAsLeaf = (BPTreeVariableLeaf<TRecord, blockSize>*) root;

	typename std::list<TRecord> recordList = rootAsLeaf->getRecords();
	typename std::list<TRecord>::iterator recordIt = recordList.begin();

	BPTreeVariableLeaf<TRecord, blockSize> *leftLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (*file_);
	BPTreeVariableLeaf<TRecord, blockSize> *rightLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (*file_);
	for (; recordIt != recordList.end(); recordIt++)
		leftLeaf->insert(*recordIt);
	rightLeaf->insert(rec);

	rightLeaf->next(0);
	leftLeaf->next(rightLeaf->blockNumber());
	leftLeaf->write();
	rightLeaf->write();

	typename TRecord::Key newKey = rec.getKey();
	unsigned int leftBlockNumber = leftLeaf->blockNumber();
	unsigned int rightBlockNumber = rightLeaf->blockNumber();
	delete rightLeaf;
	delete leftLeaf;

	delete root;
	root = new BPTreeVariableInternalNode<TRecord, blockSize> ();
	BPTreeVariableInternalNode<TRecord, blockSize>* rootAsInternal =
			(BPTreeVariableInternalNode<TRecord, blockSize>*) root;

	root->file(*file_);
	root->level(1);
	rootAsInternal->setFirstChild(leftBlockNumber);
	rootAsInternal->insertInNode(newKey, rightBlockNumber);
	root->write();

}

template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::handleLoadNodeOverflow(const TRecord & rec,
		NodeOverflowException<typename TRecord::Key> ovException) {

	BPTreeVariableInternalNode<TRecord, blockSize>* leftNode = new BPTreeVariableInternalNode<TRecord, blockSize> (
			root->level(), *file_);
	BPTreeVariableInternalNode<TRecord, blockSize>* rightNode = new BPTreeVariableInternalNode<TRecord, blockSize> (
			root->level(), *file_);

	BPTreeVariableInternalNode<TRecord, blockSize>* castedRoot = (BPTreeVariableInternalNode<TRecord, blockSize>*) root;
	typename std::list<typename TRecord::Key> childKeys = castedRoot->getKeys();
	std::list<unsigned int> childChildren = castedRoot->getChildren();

	typename std::list<typename TRecord::Key>::iterator itKey = childKeys.begin();
	std::list<unsigned int>::iterator itChildren = childChildren.begin();

	leftNode->setFirstChild(*itChildren);
	for (itChildren++; (++itKey) != childKeys.end(); itKey++, itChildren++) {
		itKey--;
		leftNode->insertInNode(*itKey, *itChildren);
	}
	typename TRecord::Key middleKey = *itKey;
	rightNode->setFirstChild(*itChildren);
	rightNode->insertInNode(ovException.key, ovException.child);

	leftNode->write();
	rightNode->write();

	unsigned int leftBlockNumber = leftNode->blockNumber();
	unsigned int rightBlockNumber = rightNode->blockNumber();
	delete leftNode;
	delete rightNode;
	unsigned int newLevel = root->level() + 1;
	delete root;

	root = new BPTreeVariableInternalNode<TRecord, blockSize> ();
	BPTreeVariableInternalNode<TRecord, blockSize>* rootAsInternal =
			(BPTreeVariableInternalNode<TRecord, blockSize>*) root;
	root->file(*file_);
	root->level(newLevel);
	rootAsInternal->setFirstChild(leftBlockNumber);
	rootAsInternal->insertInNode(middleKey, rightBlockNumber);
	root->write();

}

template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::preOrderReport() {
	logString = "Reportando Arbol en Pre Orden en archivo: ";
	logString.append(reportPath);
	log->insert(logKey, logString);

	File reportFile(reportPath, File::NEW);
	std::string header = "Pre-Order Traversal of B+ Tree : ";
	header.append(dataPath);
	header.append("\n\n");

	reportFile << "Pre-Order Traversal of B+ Tree : " << dataPath << "\n\n";
	reportFile << "Internal Node Format :" << "\n";
	reportFile << "Node N : (level)(free space) -- child 0(key 0)child 1(key 1)child 2(key 2)child 3 ..." << "\n\n";
	reportFile << "Leaf Format :" << "\n";
	reportFile << "Node N : (0)(free space)(next leaf) -- (key 0)(key 1)(key 2)(key 3) ..." << "\n\n";

	root->preOrderReport(reportFile, root->level());
}
template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::update(const TRecord & rec) {
	delete searchLeaf;
	searchLeaf = NULL;

	logString = "Actualizando registro con clave: ";
	if (TRecord::Key::isString)
		logString.append(rec.getKey().getString());
	else {
		char intStr[20];
		sprintf(intStr, "%u", rec.getKey().getUint());
		logString.append(intStr);
	}
	log->insert(logKey, logString);
	try {
		root->update(rec);
		if(root->level()==0)
			root->write();
	} catch (LeafOverflowException e) {
		handleLeafOverflow(rec);
	} catch (NodeOverflowException<typename TRecord::Key> e) {
		handleNodeOverflow(rec, e);
	} catch (LeafUnderflowException e) {
		root->write();
	} catch (NodeUnderflowException e) {
		if (((BPTreeVariableInternalNode<TRecord, blockSize>*) root)->usedSpace() == 0)
			handleNodeUnderflow();
		else
			root->write();
	}
}

template<class TRecord, unsigned int blockSize>
const TRecord & BPlusTree<TRecord, blockSize>::search(const TRecord & rec) {
	delete found;
	found = NULL;
	logString = "Buscando registro con clave: ";
	if (TRecord::Key::isString)
		logString.append(rec.getKey().getString());
	else {
		char intStr[20];
		sprintf(intStr, "%u", rec.getKey().getUint());
		logString.append(intStr);
	}
	log->insert(logKey, logString);
	if (root->level() == 0) {
		BPTreeVariableLeaf<TRecord, blockSize> * rootAsLeaf = (BPTreeVariableLeaf<TRecord, blockSize>*) root;
		try {
			found = rootAsLeaf ->search(rec);
			delete searchLeaf;
			searchLeaf = new BPTreeVariableLeaf<TRecord, blockSize> (*rootAsLeaf);

		} catch (LeafRecordNotFoundException e) {
			rootAsLeaf->nextLeaf();//Lanza excepcion ThereIsNoNextLeafException;
			throw;
		} catch (ThereIsNoNextLeafException&) {
			throw;
		}
	} else {
		found = ((BPTreeVariableInternalNode<TRecord, blockSize>*) root)->search(rec, &searchLeaf);
	}
	if(rec.getKey()==found->getKey())
		log->insert(logKey, "Registro encontrado.");
	else
		log->insert(logKey, "Registro no encontrado, retorna mayor inmediato.");
	return *found;
}

template<class TRecord, unsigned int blockSize>
const TRecord & BPlusTree<TRecord, blockSize>::next() {
	delete found;
	found=NULL;
	log->insert(logKey, "Buscando siguiente registro.");
	if (searchLeaf == NULL)
		throw BPlusTreeNextException();
	try {
		found = searchLeaf->nextRecord();

	} catch (LeafRecordNotFoundException e) {
		BPTreeVariableLeaf<TRecord, blockSize> * oldSearchLeaf = searchLeaf;
		try {
			searchLeaf = (BPTreeVariableLeaf<TRecord, blockSize> *) searchLeaf->nextLeaf();
			delete oldSearchLeaf;
		} catch (ThereIsNoNextLeafException) {
			log->insert(logKey, "No hay siguiente registro, se alcanzo el final del arbol.");
			throw;
		}
		found = new TRecord(searchLeaf->getFirstRecord());
	}
	log->insert(logKey, "Siguiente registro encontrado.");
	return *found;
}
template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::saveToSequential(const std::string & sequentialPath) {
	File sequentialFile(sequentialPath, File::NEW | File::BIN | File::IO);
	VariableSequentialBlock<blockSize> * block = new VariableSequentialBlock<blockSize> ;
	block->freeSpace = blockSize - VARIABLE_SEQUENTIAL_CONTROL_BYTES;
	char * lastPosition = block->bytes;
	root->saveToSequential(sequentialFile, block, &lastPosition);
	sequentialFile.write((char*) block, blockSize);
	delete block;
}

template<class TRecord, unsigned int blockSize>
void BPlusTree<TRecord, blockSize>::restructure() {
	std::string sequentialPath = dataPath;
	std::string::iterator it = sequentialPath.end();
	for (; (*it) != '.' && (*it) != '/'; it--)
		;
	if (*it == '.')
		sequentialPath.erase(it, sequentialPath.end());
	sequentialPath.append("_sequential.bin");
	saveToSequential(sequentialPath);
	loadFromSequential(sequentialPath);
}

#endif //BPLUSTREE_H_INCLUDED

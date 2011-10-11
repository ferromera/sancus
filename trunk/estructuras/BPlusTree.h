#ifndef BPLUSTREE_H_INCLUDED
#define BPLUSTREE_H_INCLUDED


#include "BPlusTreeExceptions.h"
#include "BPTreeVariableLeaf.h"
#include "BPTreeVariableInternalNode.h"
#include "File.h"
#include <list>
#include <string>


template<class TRecord,unsigned int blockSize>
class BPlusTree{
public:
	static const char CREATE=0;
	static const char LOAD=1;

private:
    BPTreeNode<TRecord,blockSize> * root;
    BPTreeVariableLeaf<TRecord,blockSize> * lastLeaf;
    File *file_;
    std::string dataPath;
    std::string reportPath;
   // BPlusTree(){}
    void create();
    void load();
    void loadFromSecuential(const std::string & );
    unsigned int getFreeBlock();
    void handleLeafOverflow(const TRecord & rec);
    void handleNodeOverflow(const TRecord & rec,NodeOverflowException<typename TRecord::Key> );
    void handleNodeUnderflow();

    BPTreeVariableLeaf<TRecord,blockSize> * getFristLeaf();

public:

    BPlusTree(const std::string & path, char creationMode);
    BPlusTree(const std::string & treePath,const std::string & secuentialPath);

    void insert(const TRecord & rec);
    void remove(const TRecord & rec);
    const TRecord & search(const TRecord & rec);
    void update(const TRecord & rec);
    const TRecord & next();
    void preOrderReport();
};


template <class TRecord,unsigned int blockSize>
BPlusTree<TRecord,blockSize>::BPlusTree(const std::string & path, char creationMode):
root(NULL),lastLeaf(NULL),file_(NULL),dataPath(path){
	reportPath=dataPath;
	std::string::iterator it=reportPath.end();
	for(;(*it)!='.'&&(*it)!='/';it--);
	if(*it=='.')
		reportPath.erase(it,reportPath.end());
	reportPath.append("_report.txt");

	switch(creationMode){
	case CREATE: 	create();
					break;
	case LOAD:		load();
					break;
	default:		throw BPTreeInvalidCreationModeException();
	}

}
template<class TRecord,unsigned int blockSize>
BPlusTree<TRecord,blockSize>::BPlusTree(const std::string & treePath,const std::string & secuentialPath):
root(NULL),lastLeaf(NULL),file_(NULL),dataPath(treePath){
	reportPath=dataPath;
	std::string::iterator it=reportPath.end;
	for(;(*it)!='.'&&(*it)!='/';it--);
	if(*it=='.')
		reportPath.erase(it,reportPath.end());
	reportPath.append("_report.txt");

	create();
	loadFromSecuential(secuentialPath);

}
template<class TRecord,unsigned int blockSize>
void BPlusTree<TRecord,blockSize>::create(){
	delete file_;
	delete root;
	delete lastLeaf;
	file_=new File(dataPath,File::NEW|File::BIN|File::IO);
	FreeSpaceStackBlock<blockSize> * fblock=new FreeSpaceStackBlock<blockSize>;
	fblock->blockNumber=1;
	fblock->inFile=0;
	file_->write((char*)fblock,blockSize);
	delete fblock;

	root=new BPTreeVariableLeaf<TRecord,blockSize>(*file_);
	root->write();
	lastLeaf=new BPTreeVariableLeaf<TRecord,blockSize>(*file_,1);

}
template<class TRecord,unsigned int blockSize>
void BPlusTree<TRecord,blockSize>::load(){
	delete file_;
	delete root;
	delete lastLeaf;
	file_=new File(dataPath,File::BIN|File::IO);

	root=new BPTreeVariableLeaf<TRecord,blockSize>(*file_,1);

	lastLeaf= getFristLeaf();
}
template<class TRecord,unsigned int blockSize>
void BPlusTree<TRecord,blockSize>::loadFromSecuential(const std::string & secuentialPath){/*
	delete file_;
	delete root;
	delete lastLeaf;
	file_=new File(dataPath,File::BIN|File::IO);
	File secuentialFile(secuentialPath,File::BIN|File::IO);

	SecuentialBlock<blockSize> * secBlock= new SecuentialBlock<blockSize>;
	unsigned int freeSpace;
	char * bytes;
	TRecord * rec;
	while(! secuentialFile.reachEnd()){
		secuentialFile.read((char*)secBlock,blockSize);
		freeSpace=blockSize-VAR_SECUENTIAL_CONTROL_BYTES;
		bytes=secBlock->bytes;
		for(; freeSpace>secBlock->freeSpace;freeSpace-=rec->size()){
			rec= new TRecord(&bytes);
			loadInsert(*rec);
			delete rec;
		}
		delete secBlock;
	}

*/

}

template<class TRecord,unsigned int blockSize>
BPTreeVariableLeaf<TRecord,blockSize> * BPlusTree<TRecord,blockSize>::getFristLeaf(){
	unsigned int level = root -> level();
	BPTreeInternalNode<TRecord,blockSize> * prevNode;
	BPTreeInternalNode<TRecord,blockSize> * nextNode;
	BPTreeVariableLeaf<TRecord,blockSize> * firstLeaf;
	if(root->level()==0){

		firstLeaf= new BPTreeVariableLeaf<TRecord,blockSize>(*file_,1);
		return firstLeaf;
	}
	prevNode=(BPTreeInternalNode<TRecord,blockSize> *)root;
	while(level >1 ){
		nextNode = new BPTreeVariableInternalNode<TRecord,blockSize>(*file_,prevNode->getFirstChild());

		if(prevNode!=root)
			delete prevNode;
		level= nextNode->level();
		prevNode=nextNode;
	}

	firstLeaf=new BPTreeVariableLeaf<TRecord,blockSize>(*file_,prevNode->getFirstChild());

	if(prevNode!=root)
		delete prevNode;
	return firstLeaf;
}
template<class TRecord,unsigned int blockSize>
unsigned int BPlusTree<TRecord,blockSize>::getFreeBlock(){
	unsigned int newBlockNumber;
	FreeSpaceStackBlock<blockSize> *freeBlock= new FreeSpaceStackBlock<blockSize>;
	file_->seek(0,File::BEG);
	file_->read((char *)freeBlock,blockSize);
	newBlockNumber=freeBlock->blockNumber;
	unsigned long pos= newBlockNumber * blockSize;
	if(freeBlock->inFile){
		file_->seek(pos,File::BEG);
		file_->read((char *)freeBlock,blockSize);
		file_->seek(0,File::BEG);
		file_->write((char *)freeBlock,blockSize);
	}else{
		freeBlock->blockNumber++;
		file_->seek(0,File::BEG);
		file_->write((char *)freeBlock,blockSize);
	}
	delete freeBlock;
	return newBlockNumber;
}
template<class TRecord,unsigned int blockSize>
void BPlusTree<TRecord,blockSize>::insert(const TRecord & rec){
	delete lastLeaf;
	lastLeaf=NULL;
	try{
		root->insert(rec);
	}catch(LeafOverflowException e){
		handleLeafOverflow(rec);
	}
	catch(NodeOverflowException<typename TRecord::Key> e){
		handleNodeOverflow(rec,e);
	}
}
template<class TRecord,unsigned int blockSize>
void BPlusTree<TRecord,blockSize>::remove(const TRecord & rec){
	delete lastLeaf;
	lastLeaf=NULL;
	try{
		root->remove(rec);
	}catch(LeafUnderflowException e){
		root->write();
	}
	catch(NodeUnderflowException e){
		if(((BPTreeVariableInternalNode<TRecord,blockSize>*)root)->usedSpace()==0)
			handleNodeUnderflow();
		else
			root->write();

	}
}
template<class TRecord,unsigned int blockSize>
void BPlusTree<TRecord,blockSize>::handleLeafOverflow(const TRecord & record){
	BPTreeVariableLeaf<TRecord,blockSize>* rootAsLeaf=(BPTreeVariableLeaf<TRecord,blockSize>*)root;

	typename std::list<TRecord> recordList = rootAsLeaf->getRecords();
	typename std::list<TRecord>::iterator recordIt = recordList.begin();

	BPTreeVariableLeaf<TRecord,blockSize> *leftLeaf = new BPTreeVariableLeaf<TRecord,blockSize>(*file_);
	BPTreeVariableLeaf<TRecord,blockSize> *rightLeaf = new BPTreeVariableLeaf<TRecord,blockSize>(*file_);


	unsigned int halfSize = (blockSize - VARIABLE_NODE_CONTROL_BYTES)/2;

	for( ;recordIt != recordList.end();recordIt++)
			if((*recordIt)>record)
				break;
	recordList.insert(recordIt,record);

	recordIt = recordList.begin();
	for(unsigned int currSize=0;currSize<halfSize;recordIt++){
			leftLeaf->insert(*recordIt);
			currSize+=recordIt->size();
	}
	for(;recordIt!=recordList.end();recordIt++)
		rightLeaf->insert(*recordIt);

	rightLeaf->next(0);
	leftLeaf->next(rightLeaf->blockNumber());
	leftLeaf->write();
	rightLeaf->write();

	TRecord firstRecord = rightLeaf->getFirstRecord();
	typename TRecord::Key newKey = firstRecord.getKey();
	unsigned int leftBlockNumber=leftLeaf->blockNumber();
	unsigned int rightBlockNumber=rightLeaf->blockNumber();
	delete rightLeaf;
	delete leftLeaf;

	delete root;

	root=new BPTreeVariableInternalNode<TRecord,blockSize>();
	BPTreeVariableInternalNode<TRecord,blockSize>* rootAsInternal=(BPTreeVariableInternalNode<TRecord,blockSize>*)root;

	root->file(*file_);
	root->level(1);
	rootAsInternal->setFirstChild(leftBlockNumber);
	rootAsInternal->insertInNode(newKey,rightBlockNumber);
	root->write();

}
template<class TRecord,unsigned int blockSize>
void BPlusTree<TRecord,blockSize>::handleNodeOverflow(const TRecord & rec,NodeOverflowException<typename TRecord::Key> ovException){
	BPTreeVariableInternalNode<TRecord,blockSize>* leftNode=new BPTreeVariableInternalNode<TRecord,blockSize>(root->level(),
							*file_);
	BPTreeVariableInternalNode<TRecord,blockSize>* rightNode=new BPTreeVariableInternalNode<TRecord,blockSize>(root->level(),
								*file_);

	BPTreeVariableInternalNode<TRecord,blockSize>* castedRoot=(BPTreeVariableInternalNode<TRecord,blockSize>*)root;
	typename std::list<typename TRecord::Key> childKeys=castedRoot->getKeys();
	std::list <unsigned int> childChildren = castedRoot->getChildren();

	//Busco donde voy a insertar la clave que subio y donde voy a insertar su hijo derecho.
	typename std::list<typename TRecord::Key>::iterator itKey=childKeys.begin() ;
	std::list<unsigned int>::iterator itChildren=childChildren.begin()  ;
	for(itChildren++;itKey!=childKeys.end();itKey++,itChildren++){
		if(ovException.key <= (*itKey))
		      break;
	}
	childKeys.insert(itKey,ovException.key);
	childChildren.insert(itChildren,ovException.child);

	unsigned int halfSize = (blockSize - VARIABLE_NODE_CONTROL_BYTES)/2;
	itKey = childKeys.begin();
	itChildren = childChildren.begin();
	leftNode->setFirstChild(*itChildren);
	itChildren++;
	for(unsigned int currSize=0 ; currSize  < halfSize ; itKey++ , itChildren++){
		leftNode->insertInNode(*itKey,*itChildren);
		currSize+=itKey->size() + 4;
	}
	typename TRecord::Key middleKey=*itKey;
	itKey++;
	rightNode->setFirstChild(*itChildren);
	itChildren++;
	for(;itKey!=childKeys.end();itKey++ , itChildren++)
		rightNode->insertInNode(*itKey,*itChildren);

	leftNode->write();
	rightNode->write();

	unsigned int leftBlockNumber=leftNode->blockNumber();
	unsigned int rightBlockNumber=rightNode->blockNumber();
	delete leftNode;
	delete rightNode;
	unsigned int newLevel=root->level()+1;
	delete root;

	root=new BPTreeVariableInternalNode<TRecord,blockSize>();
	BPTreeVariableInternalNode<TRecord,blockSize>* rootAsInternal = (BPTreeVariableInternalNode<TRecord,blockSize>*)root;
	root->file(*file_);
	root->level(newLevel);
	rootAsInternal->setFirstChild(leftBlockNumber);
	rootAsInternal->insertInNode(middleKey,rightBlockNumber);
	root->write();

}
template<class TRecord,unsigned int blockSize>
void BPlusTree<TRecord,blockSize>::handleNodeUnderflow(){
	BPTreeVariableInternalNode<TRecord,blockSize>* rootAsInternal = (BPTreeVariableInternalNode<TRecord,blockSize>*)root;
	BPTreeVariableInternalNode<TRecord,blockSize>* leftNode=new BPTreeVariableInternalNode<TRecord,blockSize>(
								*file_, rootAsInternal->getFirstChild());
	leftNode->becomeRoot();
	leftNode->write();
	delete root;
	root=leftNode;
}

template<class TRecord,unsigned int blockSize>
void BPlusTree<TRecord,blockSize>::preOrderReport(){
	File reportFile (reportPath,File::NEW);
	std::string header="Pre-Order Traversal of B+ Tree : ";
	header.append(dataPath);
	header.append("\n\n");

	reportFile<<"Pre-Order Traversal of B+ Tree : "<<dataPath<<"\n\n";
	reportFile<<"Internal Node Format :"<<"\n";
	reportFile<<"Node N : (level)(free space) -- (child 0)key 0(child 1)key 1(child 2)key 2(child 3) ..."<<"\n\n";
	reportFile<<"Leaf Format :"<<"\n";
	reportFile<<"Node N : (0)(free space)(next leaf) -- |key 0|key 1|key 2|key 3| ..."<<"\n\n";

	root->preOrderReport(reportFile,root->level());
}
template<class TRecord,unsigned int blockSize>
void BPlusTree<TRecord,blockSize>::update(const TRecord & rec){
	delete lastLeaf;
	lastLeaf=NULL;
	try{
		root->update(rec);
	}catch(LeafOverflowException e){
		handleLeafOverflow(rec);
	}
	catch(NodeOverflowException<typename TRecord::Key> e){
		handleNodeOverflow(rec,e);
	}
	catch(LeafUnderflowException e){
			root->write();
	}
	catch(NodeUnderflowException e){
		if(((BPTreeVariableInternalNode<TRecord,blockSize>*)root)->usedSpace()==0)
			handleNodeUnderflow();
		else
			root->write();
}

}
#endif //BPLUSTREE_H_INCLUDED

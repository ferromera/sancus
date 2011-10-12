#ifndef BPTREENODE_H_INCLUDED
#define BPTREENODE_H_INCLUDED

#include "FreeSpaceStackBlock.h"
#include "File.h"
#include "BPlusTreeExceptions.h"

template<class TRecord,unsigned int blockSize>
class BPTreeNode{
protected:
            File * file_;
            unsigned long  blockNumber_;
            unsigned int level_;
            bool isFree_;


            unsigned int getFreeBlock();

public:
            BPTreeNode();
            BPTreeNode(File & file);
            BPTreeNode(File & file,unsigned long pos);
            BPTreeNode(const BPTreeNode<TRecord,blockSize> & );

            unsigned int level()const;
            unsigned long blockNumber()const;
            void level(unsigned int);
            void file(File &);
            void free();
            void becomeRoot();

            virtual void read()=0;
            virtual void write()=0;
            virtual void insert(const TRecord &)=0;
            virtual void remove(const TRecord &)=0;
            virtual bool isLeaf()const=0;
            virtual void update(const TRecord &)=0;
            virtual void preOrderReport(File & ,unsigned int)=0;

            virtual ~BPTreeNode();

};
template<class TRecord,unsigned int blockSize>
BPTreeNode<TRecord,blockSize>::BPTreeNode():
file_(NULL),isFree_(false){
	blockNumber_=1;
}
template<class TRecord,unsigned int blockSize>
BPTreeNode<TRecord,blockSize>::BPTreeNode(File & file,unsigned long blockNum):
file_(&file),blockNumber_(blockNum),isFree_(false){
}
template<class TRecord,unsigned int blockSize>
BPTreeNode<TRecord,blockSize>::BPTreeNode(File & file):
file_(&file),isFree_(false){
	blockNumber_=getFreeBlock();
}

template<class TRecord,unsigned int blockSize>
BPTreeNode<TRecord,blockSize>::BPTreeNode(const BPTreeNode<TRecord,blockSize> & node):
file_(node.file_),blockNumber_(node.blockNumber_),level_(node.level_),isFree_(node.isFree_){

}

template<class TRecord,unsigned int blockSize>
unsigned int BPTreeNode<TRecord,blockSize>::getFreeBlock(){
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
void BPTreeNode<TRecord,blockSize>::free(){
	FreeSpaceStackBlock<blockSize> *freeBlock= new FreeSpaceStackBlock<blockSize>;
	file_->seek(0,File::BEG);
	file_->read((char *)freeBlock,blockSize);
	unsigned long pos= blockNumber_ * blockSize;
	file_->seek(pos,File::BEG);
	file_->write((char *)freeBlock,blockSize);
	freeBlock->blockNumber=blockNumber_;
	freeBlock->inFile=1;
	file_->seek(0,File::BEG);
	file_->write((char *)freeBlock,blockSize);

	delete freeBlock;
	isFree_=true;

}

template<class TRecord,unsigned int blockSize>
unsigned int BPTreeNode<TRecord,blockSize>::level()const{
    return level_;
}


template<class TRecord,unsigned int blockSize>
unsigned long BPTreeNode<TRecord,blockSize>::blockNumber()const{

   return blockNumber_;
}

template<class TRecord,unsigned int blockSize>
void BPTreeNode<TRecord,blockSize>::level(unsigned int l){
    level_=l;
}


template<class TRecord,unsigned int blockSize>
void BPTreeNode<TRecord,blockSize>::file(File & f){
    file_=&f;
}
template<class TRecord,unsigned int blockSize>
void BPTreeNode<TRecord,blockSize>::becomeRoot(){
	free();
	isFree_=false;
	blockNumber_=1;
}

template<class TRecord,unsigned int blockSize>
BPTreeNode<TRecord,blockSize>::~BPTreeNode(){
}

#endif // BPTREENODE_H_INCLUDED

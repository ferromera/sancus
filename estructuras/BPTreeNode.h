#ifndef BPTREENODE_H_INCLUDED
#define BPTREENODE_H_INCLUDED

#include "FreeSpaceStackBlock.h"
#include "File.h"
#include "BPlusTreeExceptions.h"

template<class Record,unsigned int blockSize>
class BPTreeNode{
protected:
            File * file_;
            unsigned long  blockNumber_;
            unsigned int count_;
            unsigned int level_;

            BPTreeNode(){}
            unsigned int getFreeBlock();

public:
            BPTreeNode(File & file);
            BPTreeNode(File & file,unsigned long pos);

            unsigned int level()const;
            unsigned int count()const;
            unsigned long blockNumber()const;
            void level(unsigned int);
            void count(unsigned int);
            void file(File &);
            void blockNumber(unsigned long);
            virtual void read()=0;
            virtual void write()=0;
            virtual void insert(Record &)=0;
            virtual void remove(Record &)=0;
            virtual bool isLeaf()const=0;
            virtual ~BPTreeNode();
};

template<class Record,unsigned int blockSize>
BPTreeNode<Record,blockSize>::BPTreeNode(File & file,unsigned long blockNum):
file_(&file),blockNumber_(blockNum),count_(0){
}
template<class Record,unsigned int blockSize>
BPTreeNode<Record,blockSize>::BPTreeNode(File & file):
file_(&file),count_(0){
	blockNumber_=getFreeBlock();
}

template<class Record,unsigned int blockSize>
unsigned int BPTreeNode<Record,blockSize>::getFreeBlock(){
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

template<class Record,unsigned int blockSize>
unsigned int BPTreeNode<Record,blockSize>::level()const{
    return level_;
}

template<class Record,unsigned int blockSize>
unsigned int BPTreeNode<Record,blockSize>::count()const{
    return count_;
}

template<class Record,unsigned int blockSize>
unsigned long BPTreeNode<Record,blockSize>::blockNumber()const{
   return blockNumber_;
}

template<class Record,unsigned int blockSize>
void BPTreeNode<Record,blockSize>::level(unsigned int l){
    level_=l;
}

template<class Record,unsigned int blockSize>
void BPTreeNode<Record,blockSize>::count(unsigned int c){
    count_=c;
}

template<class Record,unsigned int blockSize>
void BPTreeNode<Record,blockSize>::file(File & f){
    file_=&f;
}

template<class Record,unsigned int blockSize>
void BPTreeNode<Record,blockSize>::blockNumber(unsigned long blockNum){
	blockNumber_=blockNum;
}

template<class Record,unsigned int blockSize>
BPTreeNode<Record,blockSize>::~BPTreeNode(){
}

#endif // BPTREENODE_H_INCLUDED

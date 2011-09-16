#ifndef BPTREENODE_H_INCLUDED
#define BPTREENODE_H_INCLUDED

#include "FreeSpaceStackBlock.h"
#include "File.h"

template<class Record,unsigned int blockSize>
class BPTreeNode{
protected:
            File & file_;
            unsigned long  pos_;
            unsigned int count_;
            unsigned int level_;
            unsigned int freeSpace_;

            BPTreeNode(){}

public:
            BPTreeNode(File & file);
            BPTreeNode(File & file,unsigned long pos);

            unsigned int level()const;
            unsigned int count()const;
            unsigned long pos()const;
            void level(unsigned int);
            void count(unsigned int);
            void file(File &);
            void pos(unsigned long);
            virtual void read()=0;
            virtual void write()=0;
            virtual void insert(Record &)=0;
            virtual void remove(Record &)=0;
            virtual bool isLeaf()const=0;
            virtual ~BPTreeNode();
};

template<class Record,unsigned int blockSize>
BPTreeNode<Record,blockSize>::BPTreeNode(File & file,unsigned long pos):
file_(file),pos_(pos),count_(0){
}
template<class Record,unsigned int blockSize>
BPTreeNode<Record,blockSize>::BPTreeNode(File & file):
file_(file),count_(0){

	FreeSpaceStackBlock<blockSize> *freeBlock= new FreeSpaceStackBlock<blockSize>;
	file_.seek(0,File::BEG);
	file_.read((char *)freeBlock,blockSize);
	unsigned int blockNumber=freeBlock->blockNumber;
	pos_=((unsigned long)blockNumber)*blockSize;
	if(freeBlock->inFile){
		file_.seek(pos_,File::BEG);
		file_.read((char *)freeBlock,blockSize);
		file_.seek(0,File::BEG);
		file_.write((char *)freeBlock,blockSize);
	}else{
		freeBlock->blockNumber++;
		file_.write((char *)freeBlock,blockSize);
	}
	delete freeBlock;
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
unsigned long BPTreeNode<Record,blockSize>::pos()const{
   return pos_;
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
    file_=f;
}

template<class Record,unsigned int blockSize>
void BPTreeNode<Record,blockSize>::pos(unsigned long p){
    pos_=p;
}

template<class Record,unsigned int blockSize>
BPTreeNode<Record,blockSize>::~BPTreeNode(){
}

#endif // BPTREENODE_H_INCLUDED

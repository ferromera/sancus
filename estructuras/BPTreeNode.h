#ifndef BPTREENODE_H_INCLUDED
#define BPTREENODE_H_INCLUDED

#include "FreeSpaceStackBlock.h"
#include "File.h"

template<class Record>
class BPTreeNode{
protected:
            File & file_;
            unsigned long  pos_;
            unsigned int count_;
            unsigned int level_;
            unsigned int blockSize_;
            unsigned int freeSpace_;
            virtual void readFromBlock(char *)=0;
            virtual char *writeToBlock()=0;
            BPTreeNode(){}
public:
            BPTreeNode(unsigned int blockSize);
            void load(File & file,unsigned long pos);
            unsigned int create(File &);
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
#endif // BPTREENODE_H_INCLUDED

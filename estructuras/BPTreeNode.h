#ifndef BPTREENODE_H_INCLUDED
#define BPTREENODE_H_INCLUDED

#include "FreeSpaceStackBlock.h"
#include <fstream>

template<class Record>
class BPTreeNode{
protected:
            std::fstream & file_;
            unsigned int pos_;
            unsigned int count_;
            unsigned int level_;
            unsigned int blockSize_;
            virtual void updateBlock()=0;
            BPTreeNode(){}
public:
            BPTreeNode(unsigned int blockSize);
            void load(std::fstream & file,unsigned int pos);
            unsigned int create(std::fstream &);
            unsigned int level()const;
            unsigned int count()const;
            unsigned int pos()const;
            void level(unsigned int);
            void count(unsigned int);
            void file(std::fstream &);
            void pos(unsigned int);
            virtual void insert(Record &)=0;
            virtual void remove(Record &)=0;
            virtual bool isLeaf()const=0;
            virtual void read()=0;
            virtual void write()=0;
            virtual ~BPTreeNode();
};
#endif // BPTREENODE_H_INCLUDED

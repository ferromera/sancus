#ifndef BPLUSTREE_H_INCLUDED
#define BPLUSTREE_H_INCLUDED

#include "BPlusTreeExceptions.h"
#include "BPlusBlockBuffer.h"
#include <fstream>
#include <list>


template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize=512>
class BPlusTree{

public: class Node{
        protected:
            BPlusBlockBuffer<Record,typename Record::Key,blockSize> *block;
            std::fstream & file_;
            unsigned int pos_;
            unsigned int count_;
            unsigned int level_;
            virtual void updateBlock()=0;

        public:
            Node();
            Node(std::fstream & file,unsigned int pos);
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
            virtual ~Node();
        };
        class Leaf: public Node{
            unsigned int next_;
            std::list<Record*> records_;
            void updateBlock();
        public:
            Leaf();
            Leaf(std::fstream & file,unsigned int pos);
            typename std::list<Record*>::iterator search(Record&);
            void insert(Record &);
            void remove(Record &);

        };
        class NonLeaf: public Node{
            std::list<uint32_t> childrens_;
            std::list<typename Record::Key*> keys_;
            void updateBlock();
        public:
            NonLeaf(std::fstream & file,unsigned int pos);

            void insert(Record &);
            void remove(Record &);

        };
private:
    Node * root;
    std::fstream & file_;

public:

    BPlusTree();
    BPlusTree(std::fstream & file);
    void insert(const Record & rec);
    void remove(const Record & rec);
    const Record & search(const Record & rec);
    void update(const Record & rec);


};

#endif // BPLUSTREE_H_INCLUDED

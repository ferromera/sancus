#ifndef BPLUSTREE_H_INCLUDED
#define BPLUSTREE_H_INCLUDED

#include "BPlusTreeExceptions.h"
#include "BPlusBlockBuffer.h"
#include "File.h"
#include <list>


template<class Record,unsigned int blockSize=512>
class BPlusTree{
private:
    BPTreeNode<Record,blockSize> * root;
    File *file_;

public:

    BPlusTree(const std::string & path); //Crea
    BPlusTree(File & file);	//Carga

    void insert(const Record & rec);
    void remove(const Record & rec);
    const Record & search(const Record & rec);
    void update(const Record & rec);
};
#endif // BPLUSTREE_H_INCLUDED

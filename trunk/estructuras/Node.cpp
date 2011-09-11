#include "BPlusTree.h"

using namespace std;

template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Node::
Node():pos_(0),count_(0),level_(0){
    block->count=0;
    block->level=0;
}

template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Node::
Node(fstream & file,unsigned int pos):file_(file),pos_(pos){
    block= new BPlusBlockBuffer<Record,typename Record::Key,blockSize>;
    file_.seekg(pos_,ios_base::beg);
    file_.read((char *)block,blockSize);
    count_=block->count;
    level_=block->level;
}
template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
unsigned int BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Node::
level()const{
    return level_;
}
template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
unsigned int BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Node::
count()const{
    return count_;
}
template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
unsigned int BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Node::
pos()const{
    return pos_;
}
template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
void BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Node::
level(unsigned int l){
    level_=l;
    block->level=l;
}
template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
void BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Node::
count(unsigned int c){
    count_=c;
    block->count=c;
}
template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
void BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Node::
file(std::fstream & f){
    file_=f;
}
template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
void BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Node::
pos(unsigned int p){
    pos_=p;
}

template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Node::
~Node(){
    delete block;
}

/////////////////// Leaf Methods Implementation  //////////////////////////////////////////////

template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Leaf::
Leaf():Node(){
    if(Record::isVariable){
        Node::block->Records.Variable.Leaf.freeBytes=blockSize-9;
    }
}


// Create a Leaf by loading it from file.
template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Leaf::
Leaf(std::fstream & file,unsigned int pos):Node(file,pos){
    char *readPtr;
    if(Record::isVariable){
        next_=Node::block->Records.Variable.Leaf.next;
        readPtr=Node::block->Records.Variable.Leaf.bytes;
    }
    else{
        next_=Node::block->Records.Fixed.Leaf.next;
        readPtr=Node::block->Records.Fixed.Leaf.records;
    }
    Record * newRec;
    for(int i=0 ; i<Node::count_;i++){
        newRec= new Record(&readPtr);
        records_.push_back(newRec);
        delete newRec;
    }
}
template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
typename list<Record*>::iterator BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Leaf::
search(Record & rec){
    typename list<Record*>::iterator it1;
    for(it1=records_.begin();it1<records_.end();it1++){
        if(rec <= (**it1))
            break;
    }
    return it1;
}
template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
void BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Leaf::
insert(Record & rec){
    typename list<Record*>::iterator it1=search(rec);
    if((**it1)==rec)
        throw LeafUnicityException();
    Record * rp=new Record(rec);
    records_.insert(it1,rp);
    updateBlock();
    if(Node::count_==leafOrder)
       throw LeafOverflowException();
}

template<class Record,unsigned int leafOrder,unsigned int nonLeafOrder,unsigned int blockSize>
void BPlusTree<Record,leafOrder,nonLeafOrder,blockSize>::Leaf::
remove(Record & rec){
    typename list<Record*>::iterator it1=search(rec);
    if(!((**it1)==rec))
        throw LeafNotFoundException();
    records_.erase(it1);
    updateBlock();
    if(Node::count_>leafOrder/2)
        throw LeafUnderflowException();
}

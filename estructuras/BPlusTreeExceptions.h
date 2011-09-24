#ifndef BPLUSTREEEXCEPTIONS_H_INCLUDED
#define BPLUSTREEEXCEPTIONS_H_INCLUDED

#include <exception>

class BPlusTreeException: public std::exception{};
class LeafOverflowException:public BPlusTreeException{};
class LeafUnderflowException:public BPlusTreeException{};
class LeafUnicityException:public BPlusTreeException{};
class LeafNotFoundException:public BPlusTreeException{};
class BPTreeRecordSizeException: public BPlusTreeException{};
#endif // BPLUSTREEEXCEPTIONS_H_INCLUDED

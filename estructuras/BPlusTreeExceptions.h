#ifndef BPLUSTREEEXCEPTIONS_H_INCLUDED
#define BPLUSTREEEXCEPTIONS_H_INCLUDED

#include <exception>

class BPlusTreeException: public std::exception{};
class LeafOverflowException:public BPlusTreeException{};
class LeafUnderflowException:public BPlusTreeException{};
class LeafUnicityException:public BPlusTreeException{};
class LeafNotFoundException:public BPlusTreeException{};
class BPTreeRecordSizeException: public BPlusTreeException{};
class LeafRecordNotFoundException: public BPlusTreeException{};
template<class TKey>
class NodeOverflowException: public BPlusTreeException
{
public:
	unsigned int child;
	TKey key;
	NodeOverflowException(unsigned int child ,const TKey & key)throw()
	{
		this->child = child;
		this->key = key;
	}
	~NodeOverflowException()throw(){}

};
class NodeUnderflowException:public BPlusTreeException{};
class NodeKeyNotFoundException:public BPlusTreeException{};
class ReadInAFreeNodeException:public BPlusTreeException{};
class WriteInAFreeNodeException:public BPlusTreeException{};
class ThereIsNoNextLeafException:public BPlusTreeException{};
class ThereIsNoGreaterRecordException:public BPlusTreeException{};
class BPTreeInvalidCreationModeException:public BPlusTreeException{};
class BPlusTreeNextMustNotBeCalledAfterAModifyingMethodException :public BPlusTreeException{};
#endif // BPLUSTREEEXCEPTIONS_H_INCLUDED

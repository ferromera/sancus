#ifndef BPLUSBLOCKBUFFER_H_INCLUDED
#define BPLUSBLOCKBUFFER_H_INCLUDED

#include "stdint.h"

#pragma pack(push)
#pragma pack(1)

template <class Record,class Key,unsigned int size=512>
struct BPlusBlockBuffer{
    uint16_t count;
    uint8_t level;
    union{
        union{
            struct{
                uint32_t next;
                Record records[(size-7)/ Record::fixedSize];
                char padding[(size-7)% Record::fixedSize];
            } Leaf;
            struct{
                uint32_t    leftChildren;
                struct{
                    Key key;
                    uint32_t rightChildren;
                } keys[(size-7)/(Key::fixedSize+4)];
                char padding [(size-7)%(Key::fixedSize+4)];
            } NonLeaf;
        }Fixed;
        union{
            struct{
                uint16_t freeBytes;
                uint32_t next;
                char bytes [size-9];
            }Leaf;
            struct{
                uint16_t freeBytes;
                uint32_t leftChildren;
                char bytes [size-9];
            }NonLeaf;
        }Variable;
    }Records;
};
#pragma pack(pop)


#endif // BPLUSBLOCKBUFFER_H_INCLUDED

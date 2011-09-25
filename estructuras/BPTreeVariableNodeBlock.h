/*
 * BPTreeVariableNodeBlock.h
 *
 *  Created on: 24/09/2011
 *      Author: juanmromera
 */

#ifndef BPTREEVARIABLENODEBLOCK_H_
#define BPTREEVARIABLENODEBLOCK_H_

#include "stdint.h"

#define VARIABLE_NODE_RECORDS_SPACE blockSize - 14

#pragma pack(push)
#pragma pack(1)

template<unsigned int blockSize>
struct BPTreeVariableNodeBlock{
	uint8_t nestedBlocks;
	uint16_t freeSpace;
	uint16_t count;
	uint8_t level;
	uint32_t leftChild;
	uint32_t next; //siguiente bloque anidado
	char bytes[VARIABLE_NODE_RECORDS_SPACE];
};

#pragma pack(pop)


#endif /* BPTREEVARIABLENODEBLOCK_H_ */

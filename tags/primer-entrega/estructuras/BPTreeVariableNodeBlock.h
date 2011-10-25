/*
 * BPTreeVariableNodeBlock.h
 *
 *  Created on: 24/09/2011
 *      Author: juanmromera
 */

#ifndef BPTREEVARIABLENODEBLOCK_H_
#define BPTREEVARIABLENODEBLOCK_H_

#include "stdint.h"

#define VARIABLE_NODE_CONTROL_BYTES 7

#pragma pack(push)
#pragma pack(1)

template<unsigned int blockSize>
struct BPTreeVariableNodeBlock{
	uint16_t freeSpace;
	uint8_t level;
	uint32_t leftChild;
	char bytes[blockSize - VARIABLE_NODE_CONTROL_BYTES];
};

#pragma pack(pop)


#endif /* BPTREEVARIABLENODEBLOCK_H_ */

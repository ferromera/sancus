/*
 * BPTreeVariableLeafBlock.h
 *
 *  Created on: 15/09/2011
 *      Author: fernando
 */

#ifndef BPTREEVARIABLELEAFBLOCK_H_
#define BPTREEVARIABLELEAFBLOCK_H_

#include "stdint.h"

#define VARIABLE_LEAF_CONTROL_BYTES 7

#pragma pack(push)
#pragma pack(1)
template<unsigned int blockSize>
struct BPTreeVariableLeafBlock{
	uint16_t freeSpace;
	uint8_t level;
	uint32_t next; //siguiente bloque anidado o siguiente hoja.
	char bytes[blockSize - VARIABLE_LEAF_CONTROL_BYTES];
};

#pragma pack(pop)

#endif /* BPTREEVARIABLELEAFBLOCK_H_ */

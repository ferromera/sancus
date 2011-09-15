/*
 * BPTreeVariableLeafBlock.h
 *
 *  Created on: 15/09/2011
 *      Author: fernando
 */

#ifndef BPTREEVARIABLELEAFBLOCK_H_
#define BPTREEVARIABLELEAFBLOCK_H_

#include "stdint.h"

#pragma pack(push)
#pragma pack(1)
template<unsigned int blockSize>
struct BPTreeVariableLeafBlock{
	uint8_t nestedBlocks;
	uint16_t freeSpace;
	uint16_t count;
	uint32_t next; //siguiente bloque anidado o siguiente hoja.
	char bytes[blockSize-9];
};

#pragma pack(pop)

#endif /* BPTREEVARIABLELEAFBLOCK_H_ */

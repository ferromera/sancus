/*
 * FreeSpaceStackBlock.h
 *
 *  Created on: 12/09/2011
 *      Author: fernando
 */

#ifndef FREESPACESTACKBLOCK_H_
#define FREESPACESTACKBLOCK_H_


#include "stdint.h"

#pragma pack(push)
#pragma pack(1)
template<unsigned int blockSize>
struct FreeSpaceStackBlock{
	uint32_t blockNumber;
	uint8_t inFile;
	char trash[blockSize-5];
};

#pragma pack(pop)

#endif /* FREESPACESTACKBLOCK_H_ */

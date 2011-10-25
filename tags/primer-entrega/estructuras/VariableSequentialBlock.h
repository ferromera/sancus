/*
 * VariableSequentialBlock.h
 *
 *  Created on: 12/10/2011
 *      Author: fernando
 */

#ifndef VARIABLESEQUENTIALBLOCK_H_
#define VARIABLESEQUENTIALBLOCK_H_


#include "stdint.h"

#define VARIABLE_SEQUENTIAL_CONTROL_BYTES 2

#pragma pack(push)
#pragma pack(1)
template<unsigned int blockSize>
struct VariableSequentialBlock{
	uint16_t freeSpace;
	char bytes[blockSize - VARIABLE_SEQUENTIAL_CONTROL_BYTES];
};

#pragma pack(pop)

#endif /* VARIABLESEQUENTIALBLOCK_H_ */

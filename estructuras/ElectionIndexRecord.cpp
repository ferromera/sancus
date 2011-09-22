/*
 * ElectionIndexRecord.cpp
 *
 *  Created on: 21/09/2011
 *      Author: juanmromera
 */

#include "ElectionIndexRecord.h"

ElectionIndexRecord::ElectionIndexRecord() {
	// TODO Auto-generated constructor stub

}

unsigned long ElectionIndexRecord::getBlockNumber() const
{
    return blockNumber;
}


void ElectionIndexRecord::setBlockNumber(unsigned long  blockNumber)
{
    this->blockNumber = blockNumber;
}

ElectionIndexRecord::~ElectionIndexRecord() {
	// TODO Auto-generated destructor stub
}


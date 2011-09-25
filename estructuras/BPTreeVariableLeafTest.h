/*
 * BPTreeVariableLeafTest.h
 *
 *  Created on: 15/09/2011
 *      Author: fernando
 */

#ifndef BPTREEVARIABLELEAFTEST_H_
#define BPTREEVARIABLELEAFTEST_H_

#include "BPTreeVariableLeaf.h"

class BPTreeVariableLeafTest {
public:
	BPTreeVariableLeafTest();
	void run();
	bool testRead1();
	bool testInsert1();
	bool testRemove1();
	virtual ~BPTreeVariableLeafTest();
};

#endif /* BPTREEVARIABLELEAFTEST_H_ */

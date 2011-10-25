/*
 * BPTreeVariableInternalNodeTest.h
 *
 *  Created on: 24/09/2011
 *      Author: juanmromera
 */

#ifndef BPTREEVARIABLEINTERNALNODETEST_H_
#define BPTREEVARIABLEINTERNALNODETEST_H_


#include "BPTreeVariableInternalNode.h"

class BPTreeVariableInternalNodeTest {
public:
	BPTreeVariableInternalNodeTest();
	void run();
	void arrenge1(File & filePath);
	bool testReadWrite1();
	bool testInsert1();
	bool testRemove1();
	bool testSearch1();
	virtual ~BPTreeVariableInternalNodeTest();
};

#endif /* BPTREEVARIABLEINTERNALNODETEST_H_ */

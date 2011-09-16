/*
 * BPTreeVariableLeafTest.cpp
 *
 *  Created on: 15/09/2011
 *      Author: fernando
 */

#include "BPTreeVariableLeafTest.h"
#include "StudentRecord.h"
#include <string>
using namespace std;

BPTreeVariableLeafTest::BPTreeVariableLeafTest() {}

bool BPTreeVariableLeafTest::testRead1(){
	unsigned long t;
	BPTreeVariableLeafBlock<4096> * block = new BPTreeVariableLeafBlock<4096>;
	File file("testRead.bin",File::NEW|File::IO|File::BIN);

	file.write(block,4096);

	block->count=4;
	block->nestedBlocks=0;
	block->next=0;

	StudentRecord * stRec1=new StudentRecord(8405,"Fer");
	StudentRecord * stRec2=new StudentRecord(8405,"Juan");
	StudentRecord * stRec3=new StudentRecord(9999,"Pedro");
	StudentRecord * stRec4=new StudentRecord(6666,"Diablo");

	int size = stRec1->size()+stRec2->size()+stRec3->size()+stRec4->size();

	block->freeSpace=4096-9-size;

	char * currPos=block->bytes;

	stRec1->write(&currPos);
	stRec2->write(&currPos);
	stRec3->write(&currPos);
	stRec4->write(&currPos);
	delete stRec1;
	delete stRec2;
	delete stRec3;
	delete stRec4;

	file.write(block,4096);

	BPTreeNode<StudentRecord,4096> * leaf=new BPTreeVariableLeaf<StudentRecord,4096>(20,file,1);

	char * zeroBytes=new char[4096];
	file.seek(4096,File::BEG);
	file.write(zeroBytes,4096);

	leaf->write();

	file.seek(4096,File::BEG);
	currPos=(char *)block;
	char c1,c2;
	for(int i=0;i<size+9;i++){
		file.read(&c1,1);
		c2=*currPos++;
		if(c1!=c2)
			return false;
	}
	return true;


}

BPTreeVariableLeafTest::~BPTreeVariableLeafTest() {}

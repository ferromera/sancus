/*
 * BPTreeVariableInternalNodeTest.cpp
 *
 *  Created on: 24/09/2011
 *      Author: juanmromera
 */

#include "BPTreeVariableInternalNodeTest.h"
#include "File.h"
#include "StudentRecord.h"
#include <cstring>
#include <iostream>

using namespace std;

BPTreeVariableInternalNodeTest::BPTreeVariableInternalNodeTest() {
}

BPTreeVariableInternalNodeTest::~BPTreeVariableInternalNodeTest() {
}
void BPTreeVariableInternalNodeTest::run(){
	if(testReadWrite1())
		std::cout<<"OK"<<std::endl;
	else
		std::cout<<"FAIL"<<std::endl;

}
bool BPTreeVariableInternalNodeTest::testReadWrite1(){
	BPTreeVariableNodeBlock<512> *block1=new BPTreeVariableNodeBlock<512>;
	BPTreeVariableNodeBlock<512> *block2=new BPTreeVariableNodeBlock<512>;
	BPTreeVariableNodeBlock<512> *block3=new BPTreeVariableNodeBlock<512>;
	uint32_t child=4;
	char * currPos;
	block1->count=80;
	block1->freeSpace=512-14-480;
	block1->nestedBlocks=2;
	block1->level=1;
	block1->leftChild=child++;
	block1->next=2;
	currPos= block1->bytes;
	StudentRecord * stRec;
	for(int i=0;i<80;i++){
		stRec=new StudentRecord(i+2000,"fer");
		stRec->getKey().write(&currPos);
		memcpy(currPos,&child,4);
		currPos+=4;
		child++;
		delete stRec;
	}
	block2->count=82;
	block2->freeSpace=512-14-492;
	block2->nestedBlocks=1;
	block2->level=1;
	block2->leftChild=4;
	block2->next=3;
	currPos= block2->bytes;

	for(int i=0;i<82;i++){
		stRec=new StudentRecord(i+3000,"juan");
		stRec->getKey().write(&currPos);
		memcpy(currPos,&child,4);
		currPos+=4;
		child++;
		delete stRec;
	}
	block3->count=3;
	block3->freeSpace=512-14-18;
	block3->nestedBlocks=0;
	block3->level=1;
	block3->leftChild=4;
	block3->next=0;
	currPos= block3->bytes;

	for(int i=0;i<3;i++){
		stRec=new StudentRecord(i+4000,"gonzalo");
		stRec->write(&currPos);
		memcpy(currPos,&child,4);
		currPos+=4;
		child++;
		delete stRec;
	}
	FreeSpaceStackBlock<512> * fblock=new FreeSpaceStackBlock<512>;
	fblock->blockNumber=3;
	fblock->inFile=0; //No esta en el archivo.
	File file("testInternalNode1.bin",File::BIN|File::NEW|File::IO);
	file.write(fblock,512);
	file.write(block1,512);
	file.write(block2,512);
	file.write(block3,512);
	File outputFile("testInternalNode2.bin",File::BIN|File::NEW|File::IO);
	file.write(fblock,512);
	BPTreeVariableInternalNode<StudentRecord,512> * internal=new BPTreeVariableInternalNode<StudentRecord,512>(200,file,1);

	internal->file(outputFile);
	internal->write();

	bool result=true;
	char *block1Output=new char [512];
	currPos=(char*)block1;
	outputFile.seek(512,File::BEG);
	outputFile.read(block1Output,512);
	for(int i=0;i<494;i++){
		if(block1Output[i]!=currPos[i]){
			result=false;
			break;
		}
	}
	char *block2Output=new char [512];
	currPos=(char*)block2;
	outputFile.seek(1024,File::BEG);
	outputFile.read(block2Output,512);

	for(int i=0;i<506;i++){
		if(block2Output[i]!=currPos[i]){
			result= false;
			break;
		}
	}
	char *block3Output=new char [512];
	currPos=(char*)block3;
	outputFile.seek(1536,File::BEG);
	outputFile.read(block3Output,512);

	for(int i=0;i<32;i++){
		if(block3Output[i]!=currPos[i] && (i <10 || i >13 )){
			result=false;
			//break;
		}
	}
	delete block1;
	delete block2;
	delete block3;
	delete fblock;
	delete internal;
	delete block1Output;
	delete block2Output;
	delete block3Output;

	return result;
}


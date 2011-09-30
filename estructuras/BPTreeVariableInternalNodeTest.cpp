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



BPTreeVariableInternalNodeTest::BPTreeVariableInternalNodeTest() {
}

BPTreeVariableInternalNodeTest::~BPTreeVariableInternalNodeTest() {
}
void BPTreeVariableInternalNodeTest::run(){
	std::cout<<"Testing BPTreeVariableInternalNode ..."<<std::endl;
	bool testOK=true;
	if(testReadWrite1())
		std::cout<<"testReadWrite1: OK"<<std::endl;
	else{
		std::cout<<"testReadWrite1: FAILED"<<std::endl;
		testOK=false;
	}
	if(testInsert1())
		std::cout<<"testInsert1: OK"<<std::endl;
	else{
		std::cout<<"testInsert1: FAIL"<<std::endl;
		testOK=false;
	}
	std::cout<<"End of test of BPTreeVariableInternalNode , result: ";
	if(testOK)
		std::cout<<"ALL OK!"<<std::endl;
	else
		std::cout<<"FAILED"<<std::endl;

}
bool BPTreeVariableInternalNodeTest::testInsert1(){
	//Creo el archivo de entrada
	File file("testInternalNode3.bin",File::BIN|File::NEW|File::IO);
	FreeSpaceStackBlock<512> * fblock=new FreeSpaceStackBlock<512>;
	fblock->blockNumber=9;
	fblock->inFile=0; //No esta en el archivo.
	file.write((char*)fblock,512);
	delete fblock;
	StudentRecord * stRec;
	uint32_t rightChild;
	// Nodo 1 /////////////////////////////////////////////////////////////////////////
	BPTreeVariableNodeBlock<512> *internalBlock=new BPTreeVariableNodeBlock<512>;
	internalBlock->level=2;
	internalBlock->count=1;
	internalBlock->freeSpace=512-14-6;
	internalBlock->leftChild=2;
	internalBlock->nestedBlocks=0;
	internalBlock->next=0;
	char * bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(92,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=3;
	memcpy(bytesPtr,&rightChild,4);
	file.write((char*)internalBlock,512);
	delete stRec;
	delete internalBlock;
	// Nodo 2 /////////////////////////////////////////////////////////////////////////
	internalBlock=new BPTreeVariableNodeBlock<512>;
	internalBlock->level=1;
	internalBlock->count=2;
	internalBlock->freeSpace=512-14-12;
	internalBlock->leftChild=4;
	internalBlock->nestedBlocks=0;
	internalBlock->next=0;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(40,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=5;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(72,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=6;
	memcpy(bytesPtr,&rightChild,4);
	delete stRec;
	file.write((char*)internalBlock,512);
	delete internalBlock;
	// Nodo 3 /////////////////////////////////////////////////////////////////////////
	internalBlock=new BPTreeVariableNodeBlock<512>;
	internalBlock->level=1;
	internalBlock->count=1;
	internalBlock->freeSpace=512-14-6;
	internalBlock->leftChild=7;
	internalBlock->nestedBlocks=0;
	internalBlock->next=0;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(120,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=8;
	memcpy(bytesPtr,&rightChild,4);
	delete stRec;
	file.write((char*)internalBlock,512);
	delete internalBlock;
	// Nodo 4 /////////////////////////////////////////////////////////////////////////
	BPTreeVariableLeafBlock<512> *leafBlock=new BPTreeVariableLeafBlock<512>;
	leafBlock->count=4;
	leafBlock->freeSpace=512-9-24;
	leafBlock->nestedBlocks=0;
	leafBlock->next=5;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(2,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(10,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(15,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(30,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	file.write((char*)leafBlock,512);
	delete leafBlock;
	// Nodo 5 /////////////////////////////////////////////////////////////////////////
	leafBlock=new BPTreeVariableLeafBlock<512>;
	leafBlock->count=3;
	leafBlock->freeSpace=512-9-18;
	leafBlock->nestedBlocks=0;
	leafBlock->next=6;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(40,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(51,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(57,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	file.write((char*)leafBlock,512);
	delete leafBlock;
	// Nodo 6 /////////////////////////////////////////////////////////////////////////
	leafBlock=new BPTreeVariableLeafBlock<512>;
	leafBlock->count=3;
	leafBlock->freeSpace=512-9-18;
	leafBlock->nestedBlocks=0;
	leafBlock->next=7;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(72,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(80,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(85,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	file.write((char*)leafBlock,512);
	delete leafBlock;
	// Nodo 7 /////////////////////////////////////////////////////////////////////////
	leafBlock=new BPTreeVariableLeafBlock<512>;
	leafBlock->count=2;
	leafBlock->freeSpace=512-9-12;
	leafBlock->nestedBlocks=0;
	leafBlock->next=8;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(92,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(104,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	file.write((char*)leafBlock,512);
	delete leafBlock;
	// Nodo 8 /////////////////////////////////////////////////////////////////////////
	leafBlock=new BPTreeVariableLeafBlock<512>;
	leafBlock->count=3;
	leafBlock->freeSpace=512-9-18;
	leafBlock->nestedBlocks=0;
	leafBlock->next=0;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(120,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(130,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(161,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	file.write((char*)leafBlock,512);
	delete leafBlock;

	//Cargo la raiz (Nodo 1)
	BPTreeVariableInternalNode<StudentRecord,512> * root=new BPTreeVariableInternalNode<StudentRecord,512>(3,4,file,1);
	stRec= new StudentRecord(32,"ABC");
	root->insert(*stRec);
	delete stRec;
	stRec= new StudentRecord(121,"ABC");
	root->insert(*stRec);
	delete stRec;
	stRec= new StudentRecord(125,"ABC");
	root->insert(*stRec);
	delete stRec;
	stRec= new StudentRecord(41,"ABC");
	root->insert(*stRec);
	delete stRec;
	stRec= new StudentRecord(60,"ABC");
	root->insert(*stRec);
	delete stRec;
	root->write();

	bool comparisonOK=true;
	char * testBlock;
	char * readBlock= new char[512];
	//Comparo bloque 1
	internalBlock=new BPTreeVariableNodeBlock<512>;
	internalBlock->level=2;
	internalBlock->count=2;
	internalBlock->freeSpace=512-14-12;
	internalBlock->leftChild=2;
	internalBlock->nestedBlocks=0;
	internalBlock->next=0;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(57,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=12;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(92,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=3;
	memcpy(bytesPtr,&rightChild,4);
	delete stRec;

	testBlock=(char*)internalBlock;
	file.seek(512,File::BEG);
	file.read(readBlock,512);
	for(int i=0; i<26 ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete internalBlock;

	//Comparo bloque 2
	internalBlock=new BPTreeVariableNodeBlock<512>;
	internalBlock->level=1;
	internalBlock->count=2;
	internalBlock->freeSpace=512-14-12;
	internalBlock->leftChild=4;
	internalBlock->nestedBlocks=0;
	internalBlock->next=0;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(30,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=9;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(40,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=5;
	memcpy(bytesPtr,&rightChild,4);
	delete stRec;

	testBlock=(char*)internalBlock;
	file.read(readBlock,512);
	for(int i=0; i<26 ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete internalBlock;

	//Comparo bloque 3
	internalBlock=new BPTreeVariableNodeBlock<512>;
	internalBlock->level=1;
	internalBlock->count=2;
	internalBlock->freeSpace=512-14-12;
	internalBlock->leftChild=7;
	internalBlock->nestedBlocks=0;
	internalBlock->next=0;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(120,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=8;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(130,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=10;
	memcpy(bytesPtr,&rightChild,4);
	delete stRec;

	testBlock=(char*)internalBlock;
	file.read(readBlock,512);
	for(int i=0; i<26 ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete internalBlock;

	//Comparo bloque 4
	leafBlock=new BPTreeVariableLeafBlock<512>;
	leafBlock->count=3;
	leafBlock->freeSpace=512-9-18;
	leafBlock->nestedBlocks=0;
	leafBlock->next=9;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(2,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(10,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(15,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,512);
	for(int i=0; i<27 ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 5
	leafBlock=new BPTreeVariableLeafBlock<512>;
	leafBlock->count=3;
	leafBlock->freeSpace=512-9-18;
	leafBlock->nestedBlocks=0;
	leafBlock->next=11;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(40,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(41,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(51,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,512);
	for(int i=0; i<27 ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 6
	leafBlock=new BPTreeVariableLeafBlock<512>;
	leafBlock->count=3;
	leafBlock->freeSpace=512-9-18;
	leafBlock->nestedBlocks=0;
	leafBlock->next=7;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(72,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(80,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(85,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,512);
	for(int i=0; i<27 ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 7
	leafBlock=new BPTreeVariableLeafBlock<512>;
	leafBlock->count=2;
	leafBlock->freeSpace=512-9-12;
	leafBlock->nestedBlocks=0;
	leafBlock->next=8;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(92,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(104,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;


	testBlock=(char*)leafBlock;
	file.read(readBlock,512);
	for(int i=0; i<21 ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 8
	leafBlock=new BPTreeVariableLeafBlock<512>;
	leafBlock->count=3;
	leafBlock->freeSpace=512-9-18;
	leafBlock->nestedBlocks=0;
	leafBlock->next=10;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(120,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(121,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(125,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,512);
	for(int i=0; i<27 ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 9
	leafBlock=new BPTreeVariableLeafBlock<512>;
	leafBlock->count=2;
	leafBlock->freeSpace=512-9-12;
	leafBlock->nestedBlocks=0;
	leafBlock->next=5;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(30,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(32,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,512);
	for(int i=0; i<21 ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 10
	leafBlock=new BPTreeVariableLeafBlock<512>;
	leafBlock->count=2;
	leafBlock->freeSpace=512-9-12;
	leafBlock->nestedBlocks=0;
	leafBlock->next=0;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(130,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(161,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,512);
	for(int i=0; i<21 ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 11
	leafBlock=new BPTreeVariableLeafBlock<512>;
	leafBlock->count=2;
	leafBlock->freeSpace=512-9-12;
	leafBlock->nestedBlocks=0;
	leafBlock->next=6;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(57,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(60,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,512);
	for(int i=0; i<21 ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 12
	internalBlock=new BPTreeVariableNodeBlock<512>;
	internalBlock->level=1;
	internalBlock->count=1;
	internalBlock->freeSpace=512-14-6;
	internalBlock->leftChild=11;
	internalBlock->nestedBlocks=0;
	internalBlock->next=0;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(72,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=6;
	memcpy(bytesPtr,&rightChild,4);
	delete stRec;

	testBlock=(char*)internalBlock;
	file.read(readBlock,512);
	for(int i=0; i<20 ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete internalBlock;

	return comparisonOK;

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
	BPTreeVariableInternalNode<StudentRecord,512> * internal=new BPTreeVariableInternalNode<StudentRecord,512>(200,200,file,1);

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
		if(block3Output[i]!=currPos[i]){
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


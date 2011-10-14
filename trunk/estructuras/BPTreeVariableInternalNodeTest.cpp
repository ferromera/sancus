/*
 * BPTreeVariableInternalNodeTest.cpp
 *
 *  Created on: 24/09/2011
 *      Author: juanmromera
 */

#include "BPTreeVariableInternalNodeTest.h"
#include "File.h"
#include "../records/StudentRecord.h"
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
	if(testRemove1())
		std::cout<<"testRemove1: OK"<<std::endl;
	else{
		std::cout<<"testRemove1: FAIL"<<std::endl;
		testOK=false;
	}
	if(testSearch1())
		std::cout<<"testSearch1: OK"<<std::endl;
	else{
		std::cout<<"testSearch1: FAIL"<<std::endl;
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
	StudentRecord * stRec;
	uint32_t rightChild;
	BPTreeVariableNodeBlock<25> *internalBlock;
	char * bytesPtr;
	BPTreeVariableLeafBlock<25>* leafBlock;

	//Creo un arbol en el archivo.
	arrenge1(file);

	//Cargo la raiz (Nodo 1)
	BPTreeVariableInternalNode<StudentRecord,25> * root=new BPTreeVariableInternalNode<StudentRecord,25>(file,1);
	stRec= new StudentRecord(30,"ABC");
	root->insert(*stRec);
	delete stRec;
	stRec= new StudentRecord(16,"ABC");
	root->insert(*stRec);
	delete stRec;
	stRec= new StudentRecord(17,"ABC");
	root->insert(*stRec);
	delete stRec;

	root->write();

	bool comparisonOK=true;
	char * testBlock;
	char * readBlock= new char[512];
	//Comparo bloque 1
	internalBlock=new BPTreeVariableNodeBlock<25>;
	internalBlock->level=2;
	internalBlock->freeSpace=25-VARIABLE_NODE_CONTROL_BYTES-18;
	internalBlock->leftChild=2;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(40,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=16;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(92,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=3;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(160,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=4;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;

	testBlock=(char*)internalBlock;
	file.seek(25,File::BEG);
	file.read(readBlock,25);
	for(int i=0; i<18+VARIABLE_NODE_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete internalBlock;

	//Comparo bloque 2
	internalBlock=new BPTreeVariableNodeBlock<25>;
	internalBlock->level=1;
	internalBlock->freeSpace=25-VARIABLE_NODE_CONTROL_BYTES-12;
	internalBlock->leftChild=5;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(15,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=14;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(17,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=15;
	memcpy(bytesPtr,&rightChild,4);
	delete stRec;

	testBlock=(char*)internalBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_NODE_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete internalBlock;

	//Comparo bloque 3
	internalBlock=new BPTreeVariableNodeBlock<25>;
	internalBlock->level=1;
	internalBlock->freeSpace=25-VARIABLE_NODE_CONTROL_BYTES-12;
	internalBlock->leftChild=8;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(120,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=9;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(130,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=10;
	memcpy(bytesPtr,&rightChild,4);
	delete stRec;

	testBlock=(char*)internalBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_NODE_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete internalBlock;

	//Comparo bloque 4
	internalBlock=new BPTreeVariableNodeBlock<25>;
	internalBlock->level=1;
	internalBlock->freeSpace=25-VARIABLE_NODE_CONTROL_BYTES-12;
	internalBlock->leftChild=11;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(170,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=12;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(190,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=13;
	memcpy(bytesPtr,&rightChild,4);
	delete stRec;

	testBlock=(char*)internalBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_NODE_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete internalBlock;

	//Comparo bloque 5
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=14;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(2,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(10,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;


	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 6
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=7;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(40,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(51,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 7
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=8;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(72,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(80,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;


	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 8
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=9;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(92,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(104,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 9
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=10;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(120,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(125,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 10
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=11;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(130,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(140,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 11
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=12;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(160,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(165,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 12
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=13;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(170,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(175,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 13
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=0;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(200,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(210,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}

	delete leafBlock;

	//Comparo bloque 14
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=15;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(15,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(16,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}

	delete leafBlock;

	//Comparo bloque 15
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=6;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(17,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(30,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}

	delete leafBlock;

	//Comparo bloque 4
	internalBlock=new BPTreeVariableNodeBlock<25>;
	internalBlock->level=1;
	internalBlock->freeSpace=25-VARIABLE_NODE_CONTROL_BYTES-6;
	internalBlock->leftChild=6;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(72,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=7;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;

	testBlock=(char*)internalBlock;
	file.read(readBlock,25);
	for(int i=0; i<6+VARIABLE_NODE_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete internalBlock;

	return comparisonOK;

}
bool BPTreeVariableInternalNodeTest::testRemove1(){
	//Creo el archivo de entrada
	File file("testInternalNode4.bin",File::BIN|File::NEW|File::IO);
	StudentRecord * stRec;
	uint32_t rightChild;
	BPTreeVariableNodeBlock<25> *internalBlock;
	char * bytesPtr;
	BPTreeVariableLeafBlock<25>* leafBlock;

	//Creo un arbol en el archivo.
	arrenge1(file);


	//Cargo la raiz (Nodo 1)
	BPTreeVariableInternalNode<StudentRecord,25> * root=new BPTreeVariableInternalNode<StudentRecord,25>(file,1);
	stRec= new StudentRecord(170,"ABC");
	root->remove(*stRec);
	delete stRec;
	stRec= new StudentRecord(175,"ABC");
	root->remove(*stRec);
	delete stRec;
	stRec= new StudentRecord(80,"ABC");
	root->remove(*stRec);
	delete stRec;
	stRec= new StudentRecord(72,"ABC");
	root->remove(*stRec);
	delete stRec;
	stRec= new StudentRecord(51,"ABC");
	root->remove(*stRec);
	delete stRec;
	stRec= new StudentRecord(40,"ABC");
	root->remove(*stRec);
	delete stRec;
	stRec= new StudentRecord(10,"ABC");
	root->remove(*stRec);
	delete stRec;
	stRec= new StudentRecord(15,"ABC");
	root->remove(*stRec);
	delete stRec;

	root->write();

	bool comparisonOK=true;
	char * testBlock;
	char * readBlock= new char[25];
	//Comparo bloque 1
	internalBlock=new BPTreeVariableNodeBlock<25>;
	internalBlock->level=2;
	internalBlock->freeSpace=25-VARIABLE_NODE_CONTROL_BYTES-6;
	internalBlock->leftChild=2;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(160,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=4;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;


	testBlock=(char*)internalBlock;
	file.seek(25,File::BEG);
	file.read(readBlock,25);
	for(int i=0; i<6+VARIABLE_NODE_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete internalBlock;

	//Comparo bloque 2
	internalBlock=new BPTreeVariableNodeBlock<25>;
	internalBlock->level=1;
	internalBlock->freeSpace=25-VARIABLE_NODE_CONTROL_BYTES-18;
	internalBlock->leftChild=5;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(92,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=8;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(120,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=9;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(130,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=10;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;

	testBlock=(char*)internalBlock;
	file.read(readBlock,25);
	for(int i=0; i<18+VARIABLE_NODE_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete internalBlock;

	//Comparo bloque 4
	internalBlock=new BPTreeVariableNodeBlock<25>;
	internalBlock->level=1;
	internalBlock->freeSpace=25-VARIABLE_NODE_CONTROL_BYTES-12;
	internalBlock->leftChild=11;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(170,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=12;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(210,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=13;
	memcpy(bytesPtr,&rightChild,4);
	delete stRec;

	testBlock=(char*)internalBlock;
	file.seek(100,File::BEG);
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_NODE_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete internalBlock;


	//Comparo bloque 5
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-6;
	leafBlock->next=8;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(2,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;


	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<6+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 8
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=9;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(92,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(104,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.seek(200,File::BEG);
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 9
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=10;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(120,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(125,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 10
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=11;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(130,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(140,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 11
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=12;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(160,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(165,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<12+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 12
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-6;
	leafBlock->next=13;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(200,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<6+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	//Comparo bloque 13
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-6;
	leafBlock->next=0;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(210,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;

	testBlock=(char*)leafBlock;
	file.read(readBlock,25);
	for(int i=0; i<6+VARIABLE_LEAF_CONTROL_BYTES ; i++){
		if(readBlock[i]!=testBlock[i])
			comparisonOK=false;
	}
	delete leafBlock;

	return comparisonOK;
}
void BPTreeVariableInternalNodeTest::arrenge1( File & file){
	//Creo el archivo de entrada
	FreeSpaceStackBlock<25> * fblock=new FreeSpaceStackBlock<25>;
	fblock->blockNumber=14;
	fblock->inFile=0; //No esta en el archivo.
	file.write((char*)fblock,25);
	delete fblock;
	StudentRecord * stRec;
	uint32_t rightChild;
	// Nodo 1 /////////////////////////////////////////////////////////////////////////
	BPTreeVariableNodeBlock<25> *internalBlock=new BPTreeVariableNodeBlock<25>;
	internalBlock->level=2;
	internalBlock->freeSpace=25-VARIABLE_NODE_CONTROL_BYTES-12;
	internalBlock->leftChild=2;
	char * bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(92,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=3;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(160,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=4;
	memcpy(bytesPtr,&rightChild,4);
	file.write((char*)internalBlock,25);
	delete stRec;
	delete internalBlock;
	// Nodo 2 /////////////////////////////////////////////////////////////////////////
	internalBlock=new BPTreeVariableNodeBlock<25>;
	internalBlock->level=1;
	internalBlock->freeSpace=25-VARIABLE_NODE_CONTROL_BYTES-12;
	internalBlock->leftChild=5;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(40,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=6;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(72,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=7;
	memcpy(bytesPtr,&rightChild,4);
	delete stRec;
	file.write((char*)internalBlock,25);
	delete internalBlock;
	// Nodo 3 /////////////////////////////////////////////////////////////////////////
	internalBlock=new BPTreeVariableNodeBlock<25>;
	internalBlock->level=1;
	internalBlock->freeSpace=25-VARIABLE_NODE_CONTROL_BYTES-12;
	internalBlock->leftChild=8;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(120,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=9;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(130,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=10;
	memcpy(bytesPtr,&rightChild,4);
	delete stRec;
	file.write((char*)internalBlock,25);
	delete internalBlock;
	// Nodo 4 /////////////////////////////////////////////////////////////////////////
	internalBlock=new BPTreeVariableNodeBlock<25>;
	internalBlock->level=1;
	internalBlock->freeSpace=25-VARIABLE_NODE_CONTROL_BYTES-12;
	internalBlock->leftChild=11;
	bytesPtr=internalBlock->bytes;
	stRec = new StudentRecord(170,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=12;
	memcpy(bytesPtr,&rightChild,4);
	bytesPtr+=4;
	delete stRec;
	stRec = new StudentRecord(190,"ABC");
	stRec->getKey().write(&bytesPtr);
	rightChild=13;
	memcpy(bytesPtr,&rightChild,4);
	delete stRec;
	file.write((char*)internalBlock,25);
	delete internalBlock;
	// Nodo 5 /////////////////////////////////////////////////////////////////////////
	BPTreeVariableLeafBlock<25>* leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-18;
	leafBlock->next=6;
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
	file.write((char*)leafBlock,25);
	delete leafBlock;
	// Nodo 6 ///////////////////////////////////////////BPTreeVariableInternalNodeTest//////////////////////////////
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=7;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(40,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(51,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	file.write((char*)leafBlock,25);
	delete leafBlock;
	// Nodo 7 /////////////////////////////////////////////////////////////////////////
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=8;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(72,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(80,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	file.write((char*)leafBlock,25);
	delete leafBlock;
	// Nodo 8 /////////////////////////////////////////////////////////////////////////
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=9;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(92,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(104,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	file.write((char*)leafBlock,25);
	delete leafBlock;
	// Nodo 9 /////////////////////////////////////////////////////////////////////////
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=10;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(120,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(125,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	file.write((char*)leafBlock,25);
	delete leafBlock;
	// Nodo 10 /////////////////////////////////////////////////////////////////////////
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=11;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(130,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(140,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	file.write((char*)leafBlock,25);
	delete leafBlock;
	// Nodo 11 /////////////////////////////////////////////////////////////////////////
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=12;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(160,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(165,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	file.write((char*)leafBlock,25);
	delete leafBlock;
	// Nodo 12 /////////////////////////////////////////////////////////////////////////
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=13;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(170,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(175,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	file.write((char*)leafBlock,25);
	delete leafBlock;
	// Nodo 13 /////////////////////////////////////////////////////////////////////////
	leafBlock=new BPTreeVariableLeafBlock<25>;
	leafBlock->freeSpace=25-VARIABLE_LEAF_CONTROL_BYTES-12;
	leafBlock->next=0;
	bytesPtr=leafBlock->bytes;
	stRec= new StudentRecord(200,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	stRec= new StudentRecord(210,"ABC");
	stRec->write(&bytesPtr);
	delete stRec;
	file.write((char*)leafBlock,25);
	delete leafBlock;
}
bool BPTreeVariableInternalNodeTest::testSearch1(){
	File file("testInternalNode4.bin",File::NEW|File::IO|File::BIN);
	arrenge1(file);
	BPTreeVariableInternalNode<StudentRecord,25> * root=new BPTreeVariableInternalNode<StudentRecord,25>(file,1);
	StudentRecord * stRec,*found;
	BPTreeVariableLeaf<StudentRecord,25> * lastLeaf=new BPTreeVariableLeaf<StudentRecord,25>(file,8);

	stRec= new StudentRecord(170,"FFF");
	found=root->search(*stRec,&lastLeaf);
	delete stRec;
	bool result=true;
	if(found->idNumber()!=170||found->name().compare("ABC")!=0)
		result=false;
	delete found;
	stRec= new StudentRecord(161,"ASD");
	found=root->search(*stRec,&lastLeaf);
	delete stRec;
	if(found->idNumber()!=165||found->name().compare("ABC")!=0)
		result=false;
	delete found;
	stRec= new StudentRecord(176,"ASD");
	found=root->search(*stRec,&lastLeaf);
	delete stRec;
	if(found->idNumber()!=200||found->name().compare("ABC")!=0)
		result=false;
	delete found;
	try{
		stRec= new StudentRecord(220,"ASD");
		found=root->search(*stRec,&lastLeaf);
		delete stRec;
		result=false;
		delete found;
	}catch(ThereIsNoGreaterRecordException e){
		delete stRec;
	}

	return result;
}
bool BPTreeVariableInternalNodeTest::testReadWrite1(){
	BPTreeVariableNodeBlock<512> *block=new BPTreeVariableNodeBlock<512>;

	uint32_t child=2;
	char * currPos;
	block->freeSpace=512-VARIABLE_NODE_CONTROL_BYTES-300;
	block->level=1;
	block->leftChild=child++;
	currPos= block->bytes;
	StudentRecord * stRec;
	for(int i=0;i<50;i++){
		stRec=new StudentRecord(i+2000,"ABC");
		stRec->getKey().write(&currPos);
		memcpy(currPos,&child,4);
		currPos+=4;
		child++;
		delete stRec;
	}

	FreeSpaceStackBlock<512> * fblock=new FreeSpaceStackBlock<512>;
	fblock->blockNumber=2;
	fblock->inFile=0; //No esta en el archivo.
	File file("testInternalNode1.bin",File::BIN|File::NEW|File::IO);
	file.write(fblock,512);
	file.write(block,512);

	File outputFile("testInternalNode2.bin",File::BIN|File::NEW|File::IO);
	file.write(fblock,512);
	BPTreeVariableInternalNode<StudentRecord,512> * internal=new BPTreeVariableInternalNode<StudentRecord,512>(file,1);

	internal->file(outputFile);
	internal->write();

	bool result=true;
	char *blockOutput=new char [512];
	currPos=(char*)block;
	outputFile.seek(512,File::BEG);
	outputFile.read(blockOutput,512);
	for(int i=0;i<300+VARIABLE_NODE_CONTROL_BYTES;i++){
		if(blockOutput[i]!=currPos[i]){
			result=false;
			break;
		}
	}
	delete block;
	delete fblock;
	delete internal;
	delete blockOutput;

	return result;
}


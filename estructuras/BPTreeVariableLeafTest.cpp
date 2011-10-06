/*
 * BPTreeVariableLeafTest.cpp
 *
 *  Created on: 15/09/2011
 *      Author: fernando
 */

#include "BPTreeVariableLeafTest.h"
#include "StudentRecord.h"
#include <iostream>
#include <string>
using namespace std;

BPTreeVariableLeafTest::BPTreeVariableLeafTest() {}

void BPTreeVariableLeafTest::run(){

	std::cout<<"Testing BPTreeVariableLeaf ..."<<std::endl;
	bool testOK=true;
	if(testRead1()){
		std::cout<<"testRead1: OK"<<std::endl;
	}else{
		std::cout<<"testRead1: ERROR"<<std::endl;
		testOK=false;
	}
	if(testInsert1()){
		std::cout<<"testInsert1: OK"<<std::endl;
	}else{
		std::cout<<"testInsert1: ERROR"<<std::endl;
		testOK=false;
	}
	if(testRemove1()){
		std::cout<<"testRemove1: OK"<<std::endl;
	}else{
		std::cout<<"testRemove1: ERROR"<<std::endl;
		testOK=false;
	}


	std::cout<<"End of test of BPTreeVariableLeaf , result: ";
	if(testOK)
			std::cout<<"ALL OK!"<<std::endl;
	else
			std::cout<<"FAILED"<<std::endl;

}

bool BPTreeVariableLeafTest::testRead1(){
	File file("testRead.bin",File::NEW|File::IO|File::BIN);
	//Escribo el bloque de espacio libre 0
	FreeSpaceStackBlock<512> * fblock=new FreeSpaceStackBlock<512>;
	fblock->blockNumber=2;
	fblock->inFile=0; //No esta en el archivo.
	file.write(fblock,512);
	delete fblock;

	BPTreeVariableLeafBlock<512> * block = new BPTreeVariableLeafBlock<512>;

	block->next=0;

	StudentRecord * stRec1=new StudentRecord(8406,"Fer");
	StudentRecord * stRec2=new StudentRecord(8405,"Juan");
	StudentRecord * stRec3=new StudentRecord(9999,"Pedro");
	StudentRecord * stRec4=new StudentRecord(6666,"Diablo");

	int size = stRec1->size()+stRec2->size()+stRec3->size()+stRec4->size();

	block->freeSpace= 512 - VARIABLE_LEAF_CONTROL_BYTES - size;

	char * currPos=block->bytes;

	stRec1->write(&currPos);
	stRec2->write(&currPos);
	stRec3->write(&currPos);
	stRec4->write(&currPos);
	delete stRec1;
	delete stRec2;
	delete stRec3;
	delete stRec4;

	file.write(block,512);

	BPTreeVariableLeaf<StudentRecord,512> * leaf=new BPTreeVariableLeaf<StudentRecord,512>(file,1);

	char * dirtyBytes=new char[512];
	file.seek(512,File::BEG);
	file.write(dirtyBytes,512);
	delete dirtyBytes;

	leaf->write();
	delete leaf;

	file.seek(512,File::BEG);
	char * readBlock=new char [512];
	file.read(readBlock,512);
	currPos=(char *)block;
	bool result=true;
	for(int i=0;i<size+6;i++){
		if(currPos[i]!=readBlock[i])
			result= false;
	}
	delete block;
	delete readBlock;

	return result;


}

bool BPTreeVariableLeafTest::testInsert1(){
	//Escribo el bloque de espacio libre 0
	File file("testInsert1.bin",File::NEW|File::IO|File::BIN);
	FreeSpaceStackBlock<64> * fblock=new FreeSpaceStackBlock<64>;
	fblock->blockNumber=1;
	fblock->inFile=0; //No esta en el archivo.
	file.write(fblock,64);

	BPTreeVariableLeaf<StudentRecord,64>* leaf=new BPTreeVariableLeaf<StudentRecord,64>(file);


	StudentRecord * stRec1=new StudentRecord(8406,"Alfredo");
	StudentRecord * stRec2=new StudentRecord(8407,"Cecilia");
	StudentRecord * stRec3=new StudentRecord(8408,"Juan Manuel");
	unsigned int size=stRec1->size()+stRec2->size()+stRec3->size();

	leaf->insert(*stRec1);
	leaf->insert(*stRec2);
	leaf->insert(*stRec3);
	leaf->write();

	delete leaf;

	//Escribo el bloque de prueba para comparar
	BPTreeVariableLeafBlock<64>* block=new BPTreeVariableLeafBlock<64>;
	block->freeSpace=64- VARIABLE_LEAF_CONTROL_BYTES -size;
	block->next=0;
	char * currPos=block->bytes;
	stRec1->write(&currPos);
	stRec2->write(&currPos);
	stRec3->write(&currPos);
	delete stRec1;
	delete stRec2;
	delete stRec3;


	//comparo currPos con fileBlock
	currPos=(char*)block;
	char * fileBlock=new char[64];
	file.seek(64,File::BEG);
	file.read(fileBlock,64);

	bool result=true;
	for(unsigned int i=0;i< size + VARIABLE_LEAF_CONTROL_BYTES ;i++){
		if(fileBlock[i]!=currPos[i])
			result= false;
	}
	delete block;
	delete fileBlock;

	return result;


}

bool BPTreeVariableLeafTest::testRemove1(){
	//Escribo el bloque de espacio libre 0
	File file("testRemove1.bin",File::NEW|File::IO|File::BIN);
	FreeSpaceStackBlock<512> * fblock=new FreeSpaceStackBlock<512>;
	fblock->blockNumber=1;
	fblock->inFile=0; //No esta en el archivo.
	file.write(fblock,512);
	delete fblock;
	BPTreeVariableLeaf<StudentRecord,512>* leaf=new BPTreeVariableLeaf<StudentRecord,512>(file);

	StudentRecord *stRec;
	for(int i=0;i<35;i++){
		//Record de tamaño 10.
		stRec=new StudentRecord(1000+i,"1234567");
		leaf->insert(*stRec);
		delete stRec;
	}

	stRec=new StudentRecord(1005,"1234567");
	leaf->remove(*stRec);
	delete stRec;
	stRec=new StudentRecord(1012,"1234567");
	leaf->remove(*stRec);
	delete stRec;
	stRec=new StudentRecord(1018,"1234567");
	leaf->remove(*stRec);
	delete stRec;
	stRec=new StudentRecord(1022,"1234567");
	leaf->remove(*stRec);
	delete stRec;
	leaf->write();
	delete leaf;

	BPTreeVariableLeafBlock<512>* block=new BPTreeVariableLeafBlock<512>;
	block->freeSpace=512-VARIABLE_LEAF_CONTROL_BYTES-310;
	block->next=0;
	char * currPos=block->bytes;
	for(int i=0;i<35;i++){
		if(i==5||i==12||i==18||i==22)
			continue;
		stRec=new StudentRecord(1000+i,"1234567");
		stRec->write(&currPos);
		delete stRec;
	}

	currPos=(char*)block;
	char * readBlock = new char[512];
	file.seek(512,File::BEG);
	file.read(readBlock,512);

	bool result=true;
	for(unsigned int i=0;i<VARIABLE_LEAF_CONTROL_BYTES + 350;i++){
		if(currPos[i]!=readBlock[i]){
			result= false;
		}
	}
	delete block;
	delete readBlock;
	return result;

}

BPTreeVariableLeafTest::~BPTreeVariableLeafTest() {}

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
	if(testRead1()){
		cout<<"testRead1: OK"<<endl;
	}else
		cout<<"testRead1: ERROR"<<endl;
	if(testInsert1()){
		cout<<"testInsert1: OK"<<endl;
	}else
		cout<<"testInsert1: ERROR"<<endl;

}

bool BPTreeVariableLeafTest::testRead1(){
	BPTreeVariableLeafBlock<4096> * block = new BPTreeVariableLeafBlock<4096>;
	File file("testRead.bin",File::NEW|File::IO|File::BIN);

	file.write(block,4096);

	block->count=4;
	block->nestedBlocks=0;
	block->next=0;

	StudentRecord * stRec1=new StudentRecord(8406,"Fer");
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

bool BPTreeVariableLeafTest::testInsert1(){
	//Escribo el bloque de espacio libre 0
	File file("testInsert1.bin",File::NEW|File::IO|File::BIN);
	FreeSpaceStackBlock<512> * fblock=new FreeSpaceStackBlock<512>;
	fblock->blockNumber=1;
	fblock->inFile=0; //No esta en el archivo.
	file.write(fblock,512);

	BPTreeVariableLeaf<StudentRecord,512>* leaf=new BPTreeVariableLeaf<StudentRecord,512>(4,file);

	//Creo student records con nombres de 200 caracteres.
	StudentRecord * stRec1=new StudentRecord(8406,"01abcdefgh02abcdefgh03abcdefgh04abcdefgh05abcdefgh06abcdefgh07abcdefgh08abcdefgh09abcdefgh10abcdefgh11abcdefgh12abcdefgh13abcdefgh14abcdefgh15abcdefgh16abcdefgh17abcdefgh18abcdefgh19abcdefgh20abcdefgh");
	StudentRecord * stRec2=new StudentRecord(8407,"01abcdefgh02abcdefgh03abcdefgh04abcdefgh05abcdefgh06abcdefgh07abcdefgh08abcdefgh09abcdefgh10abcdefgh11abcdefgh12abcdefgh13abcdefgh14abcdefgh15abcdefgh16abcdefgh17abcdefgh18abcdefgh19abcdefgh20abcdefgh");
	StudentRecord * stRec3=new StudentRecord(8408,"01abcdefgh02abcdefgh03abcdefgh04abcdefgh05abcdefgh06abcdefgh07abcdefgh08abcdefgh09abcdefgh10abcdefgh11abcdefgh12abcdefgh13abcdefgh14abcdefgh15abcdefgh16abcdefgh17abcdefgh18abcdefgh19abcdefgh20abcdefgh");

	leaf->insert(*stRec1);
	leaf->insert(*stRec2);
	leaf->insert(*stRec3);//Esta insersion deberia anidar otro bloque.
	leaf->write();

	//Escribo los bloques de prueba para comparar
	BPTreeVariableLeafBlock<512>* block1=new BPTreeVariableLeafBlock<512>;
	block1->count=2;
	block1->freeSpace=512-9-406;
	block1->nestedBlocks=1;
	block1->next=2;
	char * currPos=block1->bytes;
	stRec1->write(&currPos);
	stRec2->write(&currPos);
	BPTreeVariableLeafBlock<512>* block2=new BPTreeVariableLeafBlock<512>;
	block2->count=1;
	block2->freeSpace=512-9-203;
	block2->nestedBlocks=0;
	block2->next=0;
	currPos=block2->bytes;
	stRec3->write(&currPos);

	//comparo currPos con fileBlock en el primer bloque
	currPos=(char*)block1;
	char * fileBlock=new char[512];
	file.seek(512,File::BEG);
	file.read(fileBlock,512);

	for(int i=0;i<415;i++){
		if(fileBlock[i]!=currPos[i])
			return false;
	}

	//comparo currPos con fileBlock en el segundo bloque
	currPos=(char*)block2;
	file.seek(1024,File::BEG);
	file.read(fileBlock,512);
	for(int i=0;i<212;i++){
		if(fileBlock[i]!=currPos[i])
			return false;
	}

	return true;


}

BPTreeVariableLeafTest::~BPTreeVariableLeafTest() {}

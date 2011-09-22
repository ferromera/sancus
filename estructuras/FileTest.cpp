/*
 * FileTest.cpp
 *
 *  Created on: 12/09/2011
 *      Author: fernando
 */
#include "FileTest.h"

using namespace std;

bool FileTest::constructorTest(){
	File f1("TestFile.txt",File::NEW|File::IO);
	if(!f1.isOpen())
		return false;
	return true;
}

bool FileTest::writeTest1(){
	File file("writeTest1.txt",File::NEW|File::IO);
	char buff[12]={'h','e','l','l','o',' ','w','o','r','l','d','!'};
	file.write(&buff,12);
	return true;

}
bool FileTest::ioTest1(){
	File file("ioTest1.bin",File::BIN|File::NEW|File::IO);
	char buff[5]={0,1,2,3,4};
		file.write(&buff,5);
		file.seek(0,File::BEG);
	char buff2[5];
		file.read(&buff2,5);
	for(int i=0 ;i<5;i++)
		if(buff[i]!=buff2[i])
			return false;
	return true;

}
bool FileTest::ioTest2(){
	File file("ioTest2.bin",File::BIN|File::NEW|File::IO);
	char * buff= new char[100000];
	for(int i=0;i<100000;i++)
		buff[i]=i;
	file.write(buff,100000);
	file.seek(-80000,File::CUR);
	char * buff2=new char[50000];
	file.read(buff2,50000);
	for(int i=0;i<50000;i++)
		if(buff2[i]!=buff[i+20000])
			return false;
	return true;

}
bool FileTest::truncTest(){
	File file("truncTest.bin",File::BIN|File::NEW|File::IO);
	char * buff= new char[10000];
	for(int i=0;i<10000;i++)
		buff[i]=i;
	file.trunc(2000);
	file.seek(0,File::END);
	int size=file.tell();
	if(size==2000)
		return true;
	return false;

}

/*
 * HashTableTest.h
 *
 *  Created on: Sep 25, 2011
 *      Author: alfredo
 */

#ifndef HASHTABLETEST_H_
#define HASHTABLETEST_H_

#include <iostream>
#include "HashTable.h"
#include "StudentRecord.h"

using namespace std;

class HashTableTest {

private:
	HashTable<StudentRecord,512> * table;

public:
	void run() {
		testCreate();
		testInsertOk();
	}

	void testCreate() {
		File file("hashTableTest.bin", File::NEW | File::IO | File::BIN);

		//this->table = new HashTable<StudentRecord,512>(file,10,100);
	}

	//Inserta con tabla vacia
	void testInsertOk(){
		StudentRecord * student1 = new StudentRecord(256, "Alfredo");

		//table->insert(student1);
	}
};

#endif /* HASHTABLETEST_H_ */

/*
 * HashTableTest.h
 *
 *  Created on: Sep 25, 2011
 *      Author: alfredo
 */

#ifndef HASHTABLETEST_H_
#define HASHTABLETEST_H_

#include "HashTable.h"
#include "../records/StudentRecord.h"

using namespace std;

class HashTableTest {

private:
	HashTable<StudentRecord, 512> * table;

public:

	void assertTrue(bool condition,string testname, string message) {
		if (!condition) {
			cout << testname << " FAIL."<<endl;
			cout << message << endl;
			throw new exception();
		}else{
			cout << testname << " OK."<<endl;
		}
	}

	void assertFalse(bool condition,string testname, string message) {
		assertTrue(!condition,testname, message);
	}

	void testCreate() {
		string path = "hashTableTest.bin";

		table = new HashTable<StudentRecord, 512>(path, 10, 100);

		assertTrue(MathUtils::isPrime(table->getSize()),"testCreate","El tamaño de la tabla debe ser primo");

	}

	void testInsertAndGet() {
		testCreate();

		StudentRecord * record = new StudentRecord(12, "alfredo");

		table->insert(*record);

		StudentRecord * record2 = table->get(record->getKey());

		assertTrue((*record) == (*record2),"testInsertAndGet","Los records no son iguales");
	}

	void testUniquenessValidation() {
		testCreate();

		StudentRecord * record1 = new StudentRecord(12, "alfredo");

		table->insert(*record1);
		table->insert(*record1);
	}

	void testGetRecordWhichIsNotPresent(){
		testCreate();

		StudentRecord::Key * key = new StudentRecord::Key(10);

		table->get(*key);
	}

	void testRemoveRecord(){
		testCreate();

		bool removed = false;

		StudentRecord * record = new StudentRecord(12, "alfredo");
		StudentRecord * record2;

		table->insert(*record);

		table->remove(*record);

		try{
			record2 = table->get(record->getKey());
		}catch (RecordNotFoundException & ex){
			removed = true;
			assertTrue(record2 == NULL, "testRemoveRecord", "El record no es null");
			cout<<"RecordNotFoundException lanzada."<<endl;
		}

		if(!removed){
			cout<<"testRemoveRecord FAIL"<<endl;
		}


	}

};

#endif /* HASHTABLETEST_H_ */

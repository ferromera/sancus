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

	void assertTrue(bool condition, string testname, string message) {
		if (!condition) {
			cout << testname << " FAIL." << endl;
			cout << message << endl;
			throw new exception();
		} else {
			cout << testname << " OK." << endl;
		}
	}

	void assertFalse(bool condition, string testname, string message) {
		assertTrue(!condition, testname, message);
	}

public:
	void testCreate() {
		string path = "hashTableTest.bin";

		table = new HashTable<StudentRecord, 512> (path, 20, 1000);

		assertTrue(MathUtils::isPrime(table->getSize()), "testCreate",
				"El tamaño de la tabla debe ser primo");

	}

	void testInsertAndGet() {
		testCreate();

		StudentRecord * record = new StudentRecord(12, "alfredo");

		table->insert(*record);

		StudentRecord record2 = table->get(record->getKey());

		assertTrue((*record) == (record2), "testInsertAndGet",
				"Los records no son iguales");
	}

	void testRemoveRecord() {
		testCreate();

		bool removed = false;

		StudentRecord * record = new StudentRecord(12, "alfredo");
		StudentRecord record2;

		table->insert(*record);
		table->remove(*record);

		try {
			record2 = table->get(record->getKey());
		} catch (RecordNotFoundException & ex) {
			removed = true;
			cout << "RecordNotFoundException lanzada." << endl;
			cout << "testRemoveRecord OK." << endl;
		}

		if (!removed) {
			cout << "testRemoveRecord FAIL" << endl;
		}
	}

	void testUniquenessValidation() {
		testCreate();

		bool inserted = true;
		StudentRecord * record1 = new StudentRecord(12, "alfredo");

		table->insert(*record1);

		try {
			table->insert(*record1);
		} catch (UniqueViolationException & ex) {
			inserted = false;
			cout << "UniqueViolationException lanzada." << endl;
			cout << "testUniquenessValidation OK." << endl;
		}

		if (inserted) {
			cout << "testRemoveRecord FAIL" << endl;
		}
	}


	void testMassiveInsert(){
		testCreate();
		try{
			for(int i=1; i<1000; i++){
				StudentRecord * record = new StudentRecord(i, "student");
				table->insert(*record);
				delete (record);
			}
		}catch(RehashCountException & ex){
			cout<<"Se lanzo la exception RehashCountException"<<endl;
		}
	}
};

#endif /* HASHTABLETEST_H_ */

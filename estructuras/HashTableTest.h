/*
 * HashTableTest.h
 *
 *  Created on: Sep 25, 2011
 *      Author: alfredo
 */

#ifndef HASHTABLETEST_H_
#define HASHTABLETEST_H_

#include "HashTable.h"
#include "StudentRecord.h"

using namespace std;

class HashTableTest {

private:
	HashTable<StudentRecord, 512> * table;

public:

	void assertTrue(bool condition, string message) {
		if (!condition) {
			cout << message << endl;
			throw new exception();
		}
	}

	void assertFalse(bool condition, string message) {
		assertTrue(!condition, message);
	}

	void testCreate() {
		File file("hashTableTest.bin", File::NEW | File::IO | File::BIN);

		table = new HashTable<StudentRecord, 512>(file, 10, 100);

		assertTrue(MathUtils::isPrime(table->getSize()),
				"El tamaño de la tabla debe ser primo");

	}

	void testInsertAndGet() {
		testCreate();

		StudentRecord * record = new StudentRecord(12, "alfredo");

		table->insert(*record);

		StudentRecord * record2 = table->get(record->getKey());

		assertTrue(record->getKey() == record2->getKey(),
				"Los records no son iguales");
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

};

#endif /* HASHTABLETEST_H_ */

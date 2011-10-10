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
	void run() {
		testCreate();
	}

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

		assertTrue(MathUtils::isPrime(table->getSize()), "El tamaño de la tabla debe ser primo");
	}

};

#endif /* HASHTABLETEST_H_ */

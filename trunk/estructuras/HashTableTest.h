/*
 * HashTableTest.h
 *
 *  Created on: Sep 25, 2011
 *      Author: alfredo
 */

#ifndef HASHTABLETEST_H_
#define HASHTABLETEST_H_

#include "BaseTestCase.h"
#include "HashTable.h"
#include "StudentRecord.h"

class HashTableTest : BaseTestCase {

private:
	HashTable<StudentRecord,512> * table;

public:
	void run() {
		testCreate();
	}

	void testCreate() {
		File file("hashTableTest.bin", File::NEW | File::IO | File::BIN);

		table = new HashTable<StudentRecord,512>(file,10,100);

		assertTrue(MathUtils::isPrime(table->getSize()), "El tamaño de la tabla debe ser primo");
	}

};

#endif /* HASHTABLETEST_H_ */

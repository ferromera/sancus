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
#include "../records/DistrictRecord.h"

using namespace std;

class HashTableTest {

private:
	HashTable<StudentRecord, 512> * table;
	HashTable<DistrictRecord, 4096> * table2;

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

		assertTrue(MathUtils::isPrime(table->getSize()), "testCreate", "El tamaño de la tabla debe ser primo");
	}

	void testLoad() {
		string path = "hashTableTest.bin";

		table = new HashTable<StudentRecord, 512> (path);

		uint16_t id = 500;
		StudentRecord::Key * k = new StudentRecord::Key(id);
		StudentRecord record = table->get(*k);
		cout << "Se recupero el student con id: " << record.idNumber() << endl;
		assertTrue(record.idNumber() == id, "testGet", "No se encontrol el student");
	}

	void testInsertAndGet() {
		testCreate();

		StudentRecord * record = new StudentRecord(12, "alfredo");

		table->insert(*record);

		StudentRecord record2 = table->get(record->getKey());

		assertTrue((*record) == (record2), "testInsertAndGet", "Los records no son iguales");
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

	void testMassiveInsert() {
		testCreate();
		try {
			for (int i = 1; i < 1000; i++) {
				StudentRecord * record = new StudentRecord(i, "student");
				table->insert(*record);
				delete (record);
			}
		} catch (RehashCountException & ex) {
			cout << "Se lanzo la exception RehashCountException" << endl;
		}
	}

	void testMassiveInsertDistrict() {
		try {
			for (int i = 1; i < 1000; i++) {
				DistrictRecord * record = new DistrictRecord("Distrito" + StringUtils::intToString(i), "FATHER" + StringUtils::intToString(i));
				table2->insert(*record);
				delete (record);
			}
		} catch (RehashCountException & ex) {
			cout << "Se lanzo la exception RehashCountException" << endl;
		}
	}

	void testMassiveGet() {
		testMassiveInsert();

		try {
			for (int i = 1; i < 1000; i++) {
				StudentRecord::Key * k = new StudentRecord::Key(i);
				StudentRecord record = table->get(*k);
				cout << "Se recupero el student con id: " << record.idNumber() << endl;
				delete (k);
			}
		} catch (RecordNotFoundException & ex) {
			cout << "Se lanzo la exception RecordNotFoundException" << endl;
		}
	}

	void testUpdate() {
		SecurityStrategy * RSA = new RsaSecurity(10);
		table2 = new HashTable<DistrictRecord, 4096> ("DistrictFileHashTable.bin", 40, 400,RSA);

		testMassiveInsertDistrict();

		try {
			DistrictRecord * record = new DistrictRecord("Pacheco", "Buenos Aires");
			table2->insert(*record);

			DistrictRecord recordFromFile = table2->get(record->getKey());
			assertTrue(recordFromFile.getDistrictName() == record->getDistrictName(), "testGet",
							"Los nombres de los distritos no son iguales");
			assertTrue(recordFromFile.getFatherName() == record->getFatherName(), "testGet",
							"Los nombres de los padres no son iguales");

			recordFromFile.setFatherName("Catamarca");

			table2->update(recordFromFile);

			DistrictRecord updatedRecord = table2->get(recordFromFile.getKey());

			assertTrue(recordFromFile.getDistrictName() == updatedRecord.getDistrictName(), "testUpdate",
							"Los nombres de los distritos no son iguales");
			assertTrue(recordFromFile.getFatherName() == updatedRecord.getFatherName(), "testUpdate",
							"Los nombres de los padres no son iguales");

		} catch (RecordNotFoundException & ex) {
			cout << "Se lanzo la exception RecordNotFoundException" << endl;
		}
	}

	void testDistrictLoad() {
		string path = "DistrictFileHashTable.bin";

		this->table2 = new HashTable<DistrictRecord, 4096> ("DistrictFileHashTable");

		std::string id = "Pacheco";
		DistrictRecord::Key * k = new DistrictRecord::Key(id);
		DistrictRecord record = table2->get(*k);
		std::cout << "Se recupero el district con id: " << record.getDistrictName() << endl;
		assertTrue(record.getDistrictName() == id, "testGet", "No se encontrol el district");
	}
};

#endif /* HASHTABLETEST_H_ */

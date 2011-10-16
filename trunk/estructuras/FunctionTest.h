/*
 * HashTableTest.h
 *
 *  Created on: Sep 25, 2011
 *      Author: alfredo
 */

#ifndef FUNCTIONTEST_H_
#define FUNCTIONTEST_H_

#include "Function.h"
#include "../records/DistrictRecord.h"
#include "../records/StringKey.h"
using namespace std;

class FunctionTest {

private:
	Function<DistrictRecord> * function;

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
	void testHashString() {
		string names[] = { "ALICIA", "ANGEL", "ANTONIA", "ANTONIO", "ARCADIO",
				"CARMEN", "CONCEPCION", "DOLORES", "ELOY", "EMILIA", "EMILIO",
				"ENCARNACION", "ESPERANZA", "FALY", "FERNANDO", "FRANCISCA",
				"FRANCISCO", "FRANCISCO J.", "GONZALO", "GREGORIO", "GUSTAVO",
				"J.JOAQUIN", "JAIME", "JAVIER", "JOAQUIN", "JOSE",
				"JOSE FERNANDO", "JOSE JOAQUIN", "JOSE LUIS", "JOSE M.",
				"JOSE MANUEL", "JOSE RAMON", "JUAN", "JUAN FCO.",
				"JUAN IGNACIO", "JUAN LUIS", "JUAN MANUEL", "LAURA", "LUIS",
				"ROSARIO", "ANGELES", "ASCENSION", "Maria DEL CARMEN",
				"ELISENDA", "INES", "JOSE", "LUISA", "MERCEDES" };

		function = new HashFunction<DistrictRecord> (20);

		for (unsigned int i = 0; i < sizeof(names); i++) {
			DistrictRecord::Key * key = new DistrictRecord::Key(names[i]);
			unsigned int hashResult = function->hash(*key, 0);
			cout << "Name: "<<names[i] << " ,Position: " << hashResult << endl;

			delete (key);
		}
	}

};

#endif

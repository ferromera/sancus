/*
 * Los desbordes se resuelven usando direccionamiento abierto y doble dispersión.
 * Se usa una funcion para obtener la direccion base y otra para dar los saltos
 * en caso de desborde
 *
 * @author Alfredo Scoppa
 * @since Sep 24, 2011
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <iostream>
#include "MathUtils.h"
#include "File.h"
#include "Bucket.h"
#include <list>
#include "Uint16Key.h"
#include "Record.h"
#include "HashTableExceptions.h"

#define PACKAGE_DENSITY 0.7
#define BUCKET_LOAD_FACTOR 0.75

template<class T, unsigned int bucketSize>
class HashTable {

private:
	File & file;

	/* El numero de filas de la tabla*/
	unsigned int size;
	unsigned int maxNumberOfRecords;
	unsigned int maxNumberOfRehashes;

	int insertRecord(T record,int* (*hashFunction)(Record::Key * key), int jump);
	int hash(Record::Key * key);
	int rehash(Record::Key * key);

public:

	/**
	 * Primitiva de creación
	 *
	 * @file
	 * @recordsPerBucket numero de registros por cubeta
	 * @maxNumberOfRecords numero maximo de registros en el archivo (cuando se llegue a este numero se debera
	 * enfrentar una reorganización de la tabla).
	 */
	HashTable(File & file,
				const unsigned int recordsPerBucket,
				const unsigned int maxNumberOfRecords);

	void load(File & file);

	void insert(T & record);

};

#endif /* HASHTABLE_H_ */

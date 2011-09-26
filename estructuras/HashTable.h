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

#include "File.h"

#define PACKAGE_DENSITY 0.7

template<class T>
class HashTable {

private:
	File & file;

	/* El numero de filas de la tabla*/
	unsigned int size;

	unsigned int bucketSize;

	unsigned int maxNumberOfRecords;

	int table[];

public:

	/**
	 * Primitiva de creación
	 *
	 * @file
	 * @recordsPerBucket numero de registros por cubeta
	 * @maxNumberOfRecords numero maximo de registros en el archivo (cuando se llegue a este numero se debera
	 * enfrentar una reorganización de la tabla).
	 */
	HashTable(const File & file, const unsigned int recordsPerBucket, const unsigned int maxNumberOfRecords);

	void load(File & file);
};

#endif /* HASHTABLE_H_ */

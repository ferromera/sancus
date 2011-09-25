/*
 * Representación en memoria de una tabla de hash.
 * Los desbordes se resuelven usando direccionamiento abierto y doble dispersión.
 * Se usa una funcion para obtener la direccion base y otra para dar los saltos
 * en caso de desborde
 *
 * @author Alfredo Scoppa
 * @since Sep 24, 2011
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "RandomAccessFile.h"

#define PACKAGE_DENSITY 0.7

template<class T>
class HashTable {

private:
	File & file;

	/* El numero de filas de la tabla*/
	unsigned int size;

	int table[];

public:

	/**
	 * Primitiva de creación
	 *
	 * @randomAccessFile archivo de acceso aleatorio, vacio
	 * @bucketSize numero de registros por cubeta
	 * @numberOfRecords numero maximo de registros en el archivo (cuando se llegue a este numero se debera
	 * enfrentar una reorganización de la tabla).
	 */
	HashTable(const RandomAccessFile<T> & randomAccessFile, const int bucketSize, const int numberOfRecords);

};

#endif /* HASHTABLE_H_ */

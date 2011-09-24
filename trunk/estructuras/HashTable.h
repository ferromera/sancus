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

template<class T>
class HashTable {

private:
	File & file;

	/* El numero de filas de la tabla*/
	unsigned int size;

public:

	/**
	 * Primitiva de creación
	 *
	 * @randomAccessFile archivo de acceso aleatorio, vacio
	 *
	 * Crea una tabla con un solo bucket apuntando al primer
	 * registro del archivo.r
	 */
	HashTable(const RandomAccessFile<T> & randomAccessFile);

};

#endif /* HASHTABLE_H_ */

/*
 * Implementación de la organización directa.
 *
 * @author Alfredo Scoppa
 * @since Sep 24, 2011
 */

#ifndef RANDOMACCESSFILE_H_
#define RANDOMACCESSFILE_H_

#include "File.h"
#include "HashTable.h"
#include "Record.h"

template<class T>
class RandomAccessFile {

private:
	int bucketSize;
	HashTable* table;
	File * file;

public:

	/**
	 * Primitiva de creación
	 *
	 * @file el archivo esta vacio
	 * @bucketSize tamaño en bytes
	 *
	 *	Crea un archivo de acceso aleatorio apartir del archivo pasado.
	 */
	RandomAccessFile(const File & file, const int bucketSize);

	/**
	 * Primitiva de inserción
	 *
	 * @record
	 */
	void insert(const T & record);

	File & getFile();
};

#endif /* RANDOMACCESSFILE_H_ */

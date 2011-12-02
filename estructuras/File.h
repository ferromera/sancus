/*
 * File.h
 *
 * Se le setea por defecto seguridad RSA pero puede cambiarse.
 *
 *  Created on: 12/09/2011
 *      Author: fernando
 */

#ifndef FILE_H_
#define FILE_H_

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include "FileException.h"
#include "unistd.h"
#include "DummySecurity.h"

class File {
	std::string path_;
	FILE * filep;
	SecurityStrategy * security;

public:
	static const char BIN = 1;
	static const char NEW = 2;
	static const char IO = 4;

	static const char BEG = 0;
	static const char CUR = 1;
	static const char END = 2;

	File(SecurityStrategy * strategy);
	File(const std::string &, char openMode, SecurityStrategy * strategy);
	File();
	File(const std::string &, char openMode=0);
	bool isOpen();
	void seek(long offset, char origin = BEG);
	long tell();
	void read(void *, size_t bytes);

	/**
	 * MODIFICADO POR ALFREDO SCOPPA MARTES 15/11
	 * LA LINEA ERA
	 *
	 * VOID WRITE( CONST VOID *, SIZE_T BYTES)
	 *
	 * NO ESTOY SEGURO DE QUE IMPACTO TENGA SACAR EL CONST
	 * PERO COMPILA Y HACE FALTA PARA PODER MODIFICAR EL BUFFER
	 * AL ENCRIPTARLO ANTES DE ESCRIBIR
	 */
	void write(void *, size_t bytes);
	void close();
	void open(const std::string &, char openMode);
	void flush();
	bool reachEnd();
	void trunc(off_t length);
	const std::string & path() const;
	File & operator<<(const std::string &);
	File & operator<<(unsigned int);
	File & operator>>(std::string &);
	File & operator>>(unsigned int &number);
	~File();
};

#endif /* FILE_H_ */

/*
 * File.h
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


class File{
	std::string path_;
	FILE * filep;
	File(){}
public:
	static const char BIN = 1;
	static const char NEW = 2;
	static const char IO  = 4;

	static const char BEG = 0;
	static const char CUR = 1;
	static const char END = 2;


	File(const std::string &,char openMode);
	bool isOpen();
	void seek(long offset,char origin=BEG);
	long tell();
	void read(void *,size_t bytes);
	void write(const void *,size_t bytes);
	void close();
	void open(const std::string &,char openMode);
	void flush();
	void trunc(off_t length);
	const std::string & path()const;
	~File();

};


#endif /* FILE_H_ */

/*
 * FileException.h
 *
 *  Created on: 12/09/2011
 *      Author: fernando
 */

#ifndef FILEEXCEPTION_H_
#define FILEEXCEPTION_H_
#include <exception>

class	FileException: public std::exception {};
class 	FileNotOpenedException:public FileException{};
class 	BadSeekOriginException:public FileException{};
class	SeekInternalException:public FileException{};
class 	TellException: public FileException{};
class 	ReadFileException: public FileException{};
class 	EndOfFileException: public FileException{};
class	WriteFileException: public FileException{};
class	CloseFileException: public FileException{};
class	OpenFileException: public  FileException{};
class	FlushFileException: public FileException{};
class	TruncateFileException: public FileException{};
class  OpenModeException:public FileException{};


#endif /* FILEEXCEPTION_H_ */

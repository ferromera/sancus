/*
 * File.cpp
 *
 *  Created on: 12/09/2011
 *      Author: fernando
 */

#include "File.h"

using namespace std;

File::File():filep(NULL){}
File::File(const std::string & path,char openMode):path_(path),filep(NULL){
	open(path,openMode);
}
bool File::isOpen(){
	if(filep==NULL)
		return false;
	return true;
}
void File::seek(long offset,char origin){
	int ori;
	if(!isOpen())
			throw FileNotOpenedException();
	switch(origin){
	case BEG: ori=SEEK_SET;
				break;
	case CUR: ori=SEEK_CUR;
					break;
	case END: ori=SEEK_END;
					break;
	default:
			throw BadSeekOriginException();
	}

	if(fseek(filep,offset,ori))
		throw SeekInternalException();

}
long File::tell(){
	if(!isOpen())
			throw FileNotOpenedException();
	long l;
	if((l=ftell(filep))==-1L)
		throw TellException();
	return l;
}
void File::read(void * buffer,size_t bytes){
	if(!isOpen())
			throw FileNotOpenedException();
	if(fread(buffer,bytes,1,filep)!=1){
		if(ferror(filep))
			throw ReadFileException();
		throw EndOfFileException();
	}

}
void File::write(const void * buffer,size_t bytes){
	if(!isOpen())
			throw FileNotOpenedException();
	if(fwrite(buffer,bytes,1,filep)!=1)
		throw WriteFileException();

}
void File::close(){
	if(!isOpen())
		throw FileNotOpenedException();
	if(fclose(filep)==EOF)
		throw CloseFileException();
}
void File::open(const std::string & path,char openMode){
	if(isOpen())
		close();
	path_=path;
	string openModeString;
		switch(openMode){
		case 0: 	openModeString="r";
					break;
		case 1: 	openModeString="rb";
						break;
		case 2: 	openModeString="w";
						break;
		case 3: 	openModeString="wb";
						break;
		case 4: 	openModeString="r+";
						break;
		case 5: 	openModeString="r+b";
						break;
		case 6: 	openModeString="w+";
						break;
		case 7: 	openModeString="w+b";
						break;
		default:	throw OpenModeException();
		}
		filep=fopen(path.c_str(),openModeString.c_str());
		if(filep==NULL)
				throw OpenFileException();

}
void File::flush(){
	if(!isOpen())
			throw FileNotOpenedException();
	if(fflush(filep)==EOF)
		throw FlushFileException();
}
void File::trunc(off_t length){
	if(!isOpen())
		throw FileNotOpenedException();
	if(truncate(path_.c_str(),length))
		throw TruncateFileException();
}
File::~File(){
	if(isOpen())
		close();
}


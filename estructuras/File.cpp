/*
 * File.cpp
 *
 *  Created on: 12/09/2011
 *      Author: fernando
 */

#include "File.h"

using namespace std;

File::File(SecurityStrategy * security):filep(NULL){
	this->security = security;
}

File::File(const std::string & path,char openMode, SecurityStrategy * security):path_(path),filep(NULL){
	this->security = security;
	open(path,openMode);
}

File::File():filep(NULL){
	this->security =  new DummySecurity();
}

File::File(const std::string & path,char openMode):path_(path),filep(NULL){
	this->security = new DummySecurity();

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
	flush();
	if(fread(buffer,bytes,1,filep)!=1){
		if(ferror(filep))
			throw ReadFileException();
		throw EndOfFileException();
	}

	this->security->decrypt((unsigned char * &)buffer,bytes);
}
void File::write(void * buffer,size_t bytes){
	if(!isOpen())
			throw FileNotOpenedException();

	this->security->encrypt((unsigned char * &)buffer,bytes);

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
bool File::reachEnd(){
	if(!isOpen())
		throw FileNotOpenedException();
	if(feof(filep))
		return true;
	return false;
}
File & File::operator<<(const std::string & str){
	if(!isOpen())
		throw FileNotOpenedException();
	if(fputs(str.c_str(),filep)==EOF)
		throw FailedInWritingStringToFileException();
	return *this;
}
File & File::operator<<(unsigned int number){
	if(!isOpen())
		throw FileNotOpenedException();
	if(fprintf(filep,"%u",number)< 0)
		throw FailedInWritingUIntToFileException();
	return *this;
}
File::~File(){
	if(isOpen())
		close();
}

//void File::setSecurityStrategy(SecurityStrategy * security) {
//	this->security = security;
//}

//SecurityStrategy * File::getSecurityStrategy() {
//	return this->security;
//}

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
unsigned int File::tellBlock(unsigned int blockSize){
	unsigned int encryptedBytes=tell();
	return encryptedBytes/security->getEncryptedBytes(blockSize);
}
void File::seekBlock(unsigned int blockNumber,unsigned int blockSize){
		if(!isOpen())
				throw FileNotOpenedException();

		unsigned int seekOffset=security->getEncryptedBytes(blockSize)*blockNumber;
		if(fseek(filep,seekOffset,SEEK_SET))
			throw SeekInternalException();

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

	if(fseek(filep,security->getEncryptedBytes(offset),ori))
		throw SeekInternalException();

}
long File::tell(){
	if(!isOpen())
			throw FileNotOpenedException();
	long l;
	if((l=ftell(filep))==-1L)
		throw TellException();
	return security->getPlainBytes(l);
}
void File::read(void * buffer,size_t bytes){
	if(!isOpen())
			throw FileNotOpenedException();
	flush();
	unsigned char * readBuffer=new unsigned char[security->getEncryptedBytes(bytes)];
	if(fread(readBuffer,security->getEncryptedBytes(bytes),1,filep)!=1){
		delete readBuffer;
		if(ferror(filep))
			throw ReadFileException();
		throw EndOfFileException();
	}
	unsigned char * decryptedBuffer=this->security->decrypt(readBuffer,bytes);
	for(unsigned int i=0;i<bytes;i++)
		((unsigned char*)buffer)[i]=decryptedBuffer[i];
	delete readBuffer;
	delete decryptedBuffer;
}
void File::write(void * buffer,size_t bytes){
	if(!isOpen())
			throw FileNotOpenedException();

	unsigned char * encryptedBuffer=this->security->encrypt((unsigned char * )buffer,bytes);

	if(fwrite(encryptedBuffer,security->getEncryptedBytes(bytes),1,filep)!=1){
		delete encryptedBuffer;
		throw WriteFileException();
	}
	delete encryptedBuffer;

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
File & File::operator>>(std::string & str){
	if(!isOpen())
		throw FileNotOpenedException();
	char * c_str = new char[100000];
	if(fgets(c_str,100000,filep)==NULL){
		delete c_str;
		if(ferror(filep))
			throw ReadFileException();
		throw EndOfFileException();
	}
	str=c_str;
	delete c_str;
	return *this;
}
File & File::operator>>(unsigned int &number){
	if(!isOpen())
		throw FileNotOpenedException();
	if(fscanf(filep,"%u",&number)==EOF)
		throw ReadFileException();
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

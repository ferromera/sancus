/**
 * Clase singleton que permite hacer logging de mensajes
 * sobre uno o varios archivos de log.
 */

#include "Logger.h"
Logger* Logger::logger = NULL;
#define _PROD_ true
#define _DATE_ true
#define _INFO_ true
#define _WARN_ true
#define _ERROR_ true
#define _DEB_ true

Logger::Logger() {
	ioFile.open("votantes.log", std::ios_base::in);
	ioFile.close();
	ioFile.open("votantes.log", std::ios_base::out);
}

Logger* Logger::getInstance(){
	if ( Logger::logger  == NULL )
		Logger::logger = new Logger();
	return Logger::logger;
}

Logger::~Logger(){
	ioFile.close();
}

std::string Logger::getTime(){
	time_t rawtime;
	time(&rawtime);
	struct tm* timestamp = localtime(&rawtime);

	std::stringstream out;
	if (timestamp->tm_mday < 10)
		out << "0" << timestamp->tm_mday << "/" ;
	else out << timestamp->tm_mday << "/" ;
	if (timestamp->tm_mon < 10)
		out << "0" << timestamp->tm_mon + 1 << "/" ;
	else out << timestamp->tm_mon + 1 << "/" ;
	out << timestamp->tm_year + 1900;
	out << " ";
	if (timestamp->tm_hour < 10 )
		out << "0" << timestamp->tm_hour << ":"  ;
	else out << timestamp->tm_hour << ":";
	if (timestamp->tm_min < 10 )
		out << "0" << timestamp->tm_min << ":" ;
	else out << timestamp->tm_min << ":";
	if (timestamp->tm_sec < 10 )
		out << "0" << timestamp->tm_sec ;
	else out << timestamp->tm_sec;
	out << " ";
	return out.str();
}

void Logger::warn(std::string  message){
	std::string key = "WARN :";
	if(_WARN_) Logger::insert(key,message);
}

void Logger::info(std::string  message){
	std::string key = "INFO :";
	if(_INFO_) Logger::insert(key,message);
}

void Logger::debug(std::string message){
	std::string key = "DEBUG :";
	if(_DEB_) Logger::insert(key,message);
}

void Logger::error(std::string  message){
	std::string key = "ERROR :";
	if(_ERROR_) Logger::insert(key,message);
}

void Logger::insert(const std::string& key, const std::string& value){
	if (ioFile.good()) {
		if(!_PROD_){
			if(_DATE_)
				std::cout << Logger::getTime();
			std::cout << key << " " << value << std::endl;
		}
		if(_DATE_)
			ioFile << Logger::getTime();
		ioFile << key << " " << value << std::endl;
		ioFile.flush();
	} else throw new FileNotOpenedException();
}

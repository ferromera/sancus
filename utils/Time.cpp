/*
 * Time.cpp
 *
 *  Created on: 17/10/2011
 *      Author: Romera 2
 */

#include "Time.h"
#include "StringUtils.h"

Time::Time() {
}

int Time::getCurrentDate()
{
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char output[8];
    strftime(output,128,"%Y%m%d",tlocal);
    std::string fecha = output;
    return StringUtils::strToint(fecha);
}
std::string Time::getTime(){
	time_t rawtime;
	time(&rawtime);
	struct tm* timestamp = localtime(&rawtime);

	std::stringstream out;
	if (timestamp->tm_mday < 10)
		out << "0" << timestamp->tm_mday;
	else out << timestamp->tm_mday;
	if (timestamp->tm_mon < 10)
		out << "0" << timestamp->tm_mon + 1;
	else out << timestamp->tm_mon + 1;
	out << timestamp->tm_year + 1900;
	if (timestamp->tm_hour < 10 )
		out << "0" << timestamp->tm_hour;
	else out << timestamp->tm_hour;
	if (timestamp->tm_min < 10 )
		out << "0" << timestamp->tm_min;
	else out << timestamp->tm_min;
	if (timestamp->tm_sec < 10 )
		out << "0" << timestamp->tm_sec ;
	else out << timestamp->tm_sec;
	return out.str();
}

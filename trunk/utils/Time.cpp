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

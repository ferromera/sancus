/**
 * Clase singleton que permite hacer logging de mensajes
 * sobre uno o varios archivos de log
 * autor nromani, ssoria.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <time.h>
#include "../estructuras/FileException.h"

/**
 * Definición
 */
class Logger {

/* Miembros privados de la clase Logger*/
private:
	/**
	 * Instancia singleton de la clase Logger.
	 */
	static Logger* logger;

	/**
	 * Almacena un stream de salida sobre el archivo de log.
	 */
	std::fstream ioFile;

	/**
	 * Constructor privado de la clase Logger.
	 */
	Logger();

	/*
	 * Arma el string con la fecha y hora actuales.
	 */
	std::string getTime();

/*Miembros publicos de la clase Logger*/
public:
	/**
	 * Devuelve la unica instancia del singleton.
	 */
	static Logger* getInstance();

	/**
	 * Escribe un mensaje de debug sobre el log.
	 */
	void debug(std::string message);

	/**
	 * Escribe un mensaje de informacion sobre el log.
	 */
	void info(std::string message);

	/**
	 * Escribe un mensaje de warning sobre el log.
	 */
	void warn(std::string message);

	/**
	 * Escribe un mensaje de error sobre el log.
	 */
	void error(std::string message);

	/**
	 * Inserta un mensaje en el archivo de log.
	 */
	void insert(const std::string& key,const std::string& value);

	/**
	 * Destructor virtual de la clase Logger.
	 */
	virtual ~Logger();
};

#endif /** LOGGER_H */

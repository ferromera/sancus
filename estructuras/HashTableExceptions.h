/*
 * HashTableException.h
 *
 *  Created on: Oct 1, 2011
 *      Author: alfredo
 */

#ifndef HASHTABLEEXCEPTION_H_
#define HASHTABLEEXCEPTION_H_

#include <exception>

class UniqueViolationException: public std::exception{};
class RehashCountException : public std::exception{};
class RecordNotFoundException : public std::exception{};

#endif /* HASHTABLEEXCEPTION_H_ */

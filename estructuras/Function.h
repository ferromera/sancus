/*
 * Function.h
 *
 *  Created on: Oct 13, 2011
 *      Author: alfredo
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

template<class T>
class Function {
private:
	unsigned int size;
public:
	Function(unsigned int size) {
		this->size = size;
	}

	unsigned int getSize(){
		return size;
	}

	virtual unsigned int hash(const typename T::Key & key, unsigned int jump) = 0;
};

template<class T>
class HashFunction: public Function<T> {
public:
	HashFunction(unsigned int size) : Function<T>(size) {
	}

	unsigned int hash(const typename T::Key & key, unsigned int jump) {
		return key.getKey() % this->getSize();
	}
};

template<class T>
class ReHashFunction: public Function<T> {
public:
	ReHashFunction(unsigned int size) :
			Function<T>(size) {
	}

	unsigned int hash(const typename T::Key & key, unsigned int jump) {
		return (jump + (key.getKey() % 5) + 1) % this->getSize();
	}
};

#endif /* FUNCTION_H_ */

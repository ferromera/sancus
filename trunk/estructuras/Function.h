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

	unsigned int convertToInt(const std::string & key) {
		return 0;
	}

	virtual unsigned int
			hashTemplateMethod(unsigned int key, unsigned int jump) = 0;
public:
	Function(unsigned int size) {
		this->size = size;
	}

	unsigned int getSize() {
		return size;
	}

	unsigned int hash(const typename T::Key & key, unsigned int jump) {
		if (T::Key::isString)
			return hashTemplateMethod(convertToInt(key.getString()), jump);
		else
			return hashTemplateMethod(key.getUint(), jump);
	}
};

template<class T>
class HashFunction: public Function<T> {
private:
	unsigned int hashTemplateMethod(unsigned int key, unsigned int jump) {
		return key % this->getSize();
	}
public:
	HashFunction(unsigned int size) :
		Function<T> (size) {
	}
};

template<class T>
class ReHashFunction: public Function<T> {
private:
	unsigned int hashTemplateMethod(unsigned int key, unsigned int jump) {
		return (jump + (key % 5) + 1) % this->getSize();
	}

public:
	ReHashFunction(unsigned int size) :
		Function<T> (size) {
	}
};

#endif /* FUNCTION_H_ */

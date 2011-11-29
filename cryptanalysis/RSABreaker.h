/*
 * RSADecryptor.h
 *
 *  Created on: 20/11/2011
 *      Author: fernando
 */

#ifndef RSADECRYPTOR_H_
#define RSADECRYPTOR_H_
#include <string>
#include <vector>

class RSABreaker {
public:
	RSABreaker();
	static const unsigned int N=88151509;//9787x9007
	static const unsigned int E=7;
	static void attack(std::string encryptedFilePath);
	static void attack2(std::string encryptedFilePath,unsigned int N,unsigned int E);
	static void addBNumber(const std::vector<unsigned int> & base, unsigned int b,
			std::vector<unsigned int> & bNumbers, std::vector<std::vector<unsigned int> >&e);
	static unsigned int calculateX(std::vector<unsigned int> & bNumbers,std::vector<unsigned int> & alpha,unsigned int b_c);
	static unsigned int calculateY(std::vector<unsigned int> & base,std::vector<unsigned int> & beta,unsigned int b_c);
	static std::vector<unsigned int> calculateBeta(std::vector<unsigned int> alfa, std::vector<std::vector<unsigned int> >e,std::vector<unsigned int> c);
	static std::vector<unsigned int> calculateAlpha(std::vector<std::vector<unsigned int> > & e,
			unsigned int &cIndex);
	virtual ~RSABreaker();
};

#endif /* RSADECRYPTOR_H_ */

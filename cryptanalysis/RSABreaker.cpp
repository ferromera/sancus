/*
 * RSADecryptor.cpp
 *
 *  Created on: 20/11/2011
 *      Author: fernando
 */

#include "RSABreaker.h"
#include <vector>
#include "../estructuras/MathUtils.h"
#include <math.h>
#include <iostream>

RSABreaker::RSABreaker() {

}
void RSABreaker::attack2(std::string encryptedFilePath,unsigned int N,unsigned int E) {
	unsigned int lastPrime=2;
	while(N%lastPrime!=0)
		lastPrime=MathUtils::nextPrime(lastPrime+1);
	std::cout<<lastPrime<<std::endl;
	unsigned int p= lastPrime;
	unsigned int q= N/lastPrime;
	unsigned int phi=(p-1)*(q-1);
	unsigned int d=  MathUtils::euclidesExtendido(E, phi).f;
	File *file=new File();
	file->open(encryptedFilePath,File::BIN);
	file->seek(0,File::END);
	unsigned int size= file->tell();
	file->seek(0,File::END);
	char *buffer= new char[size];
	file->read(buffer,size);
	decrypt(buffer,size,N,d);
	File *file=new File();
	file->open(encryptedFilePath.append("_breaked.txt"),File::BIN);
	file->write(buffer,size);

}
void RSABreaker::decrypt(void * c, size_t bytes,unsigned int N, unsigned int D) {
		int * f = (int *) c;

		for (unsigned int i = 0; i < bytes; i++) {
			f[i] = MathUtils::powMod(f[i], D, N);
		}
	}
/*
 * Utiliza el algoritmo de Dixon
 */
void RSABreaker::attack(std::string encryptedFilePath) {
	std::vector<unsigned int> base;
	base.push_back(2);
	base.push_back(3);
	base.push_back(5);
	base.push_back(7);
	/*base.push_back(11);
	base.push_back(13);
	base.push_back(17);
	base.push_back(19);*/
	//unsigned int lastPrime=19;
	unsigned int p = N, q;
	std::vector<unsigned int> bNumbers;
	std::vector<unsigned int> alpha;
	std::vector<unsigned int> c;
	std::vector<unsigned int> beta;
	std::vector<std::vector<unsigned int> > e;
	unsigned int b, cIndex = 0, b_c;
	unsigned int intentos = 1000;
	srand((unsigned)time(0));
	while (p == N || p==1) {
		if (intentos-- == 0)
			return;
			//throw CouldNotBreakRSAException();
		bNumbers.clear();
		alpha.clear();
		beta.clear();
		e.clear();
		c.clear();

		while (bNumbers.size() < base.size() + 1) {
			b = MathUtils::randomNumber((unsigned int) sqrt(N) + 1, N);
			addBNumber(base, b, bNumbers, e);
		}
		/*std::cout << "size de e: " << e.size() << " x " << e[0].size() << std::endl;
		for (unsigned int i = 0; i < bNumbers.size(); i++)
			std::cout << bNumbers[i] << std::endl;
		for (unsigned int i = 0; i < e.size(); i++) {
			for (unsigned int j = 0; j < e.size() - 1; j++)
				std::cout << e[i][j] << " ";
			std::cout << std::endl;
		}*/
		alpha = calculateAlpha(e, cIndex);
		/*std::cout << "alpha: " << std::endl;
		for (unsigned int i = 0; i < alpha.size(); i++)
			std::cout << alpha[i] << " ";
		std::cout  << std::endl;*/
		c = e[cIndex];
		/*std::cout << "c: " << std::endl;
		for (unsigned int i = 0; i < c.size(); i++)
			std::cout << c[i] << " ";
		std::cout  << std::endl;*/
		e.erase(e.begin() + cIndex);
		b_c = bNumbers[cIndex];
		bNumbers.erase(bNumbers.begin() + cIndex);
		beta = calculateBeta(alpha, e, c);
		unsigned int x = calculateX(bNumbers, alpha, b_c);
		unsigned int y = calculateY(base, beta, b_c);
		p = MathUtils::euclidesExtendido(x + y, N).d;
		q = N / p;
		//phi = (p - 1) * (q - 1);
		//d = MathUtils::euclidesExtendido(E, phi).f;
		//std::cout << p << " "<< q<<std::endl;
		std::cout <<intentos <<std::endl;
		/*std::cout << lastPrime<<std::endl;
		if(lastPrime<N)
			lastPrime=MathUtils::nextPrime(lastPrime+1);
		base.push_back(lastPrime);*/
		/*for (unsigned int i = 0; i < base.size(); i++)
					std::cout << base[i] << " ";
				std::cout  << std::endl;*/
	}
	std::cout << p << " "<< q<<std::endl;
}
void RSABreaker::addBNumber(const std::vector<unsigned int> & base, unsigned int b,
		std::vector<unsigned int> & bNumbers, std::vector<std::vector<unsigned int> >&e) {
	std::vector<unsigned int> e_i;
	e_i.resize(base.size(), 0);
	unsigned int remain = b;
	for (unsigned int i = 0; i < base.size(); i++) {
		while (remain % base[i] == 0) {
			e_i[i]++;
			remain /= base[i];
		}
	}
	if (remain != 1)
		return;
	e.push_back(e_i);
	bNumbers.push_back(b);
	return;
}

unsigned int RSABreaker::calculateX(std::vector<unsigned int> & bNumbers, std::vector<unsigned int> & alpha,
		unsigned int b_c) {
	unsigned int x = b_c;
	for (unsigned int i = 0; i < bNumbers.size(); i++)
		if (alpha[i] == 1)
			x *= bNumbers[i];
	return x;
}
unsigned int RSABreaker::calculateY(std::vector<unsigned int> & base, std::vector<unsigned int> & beta,
		unsigned int b_c) {
	unsigned int y = 1;
	for (unsigned int i = 0; i < base.size(); i++)
		y *= pow(base[i], beta[i]);
	return y;
}
std::vector<unsigned int> RSABreaker::calculateBeta(std::vector<unsigned int> alfa,
		std::vector<std::vector<unsigned int> > e, std::vector<unsigned int> c) {
	std::vector<unsigned int> beta;
	unsigned int beta_i = 0;
	for (unsigned int i = 0; i < alfa.size(); i++) {
		beta_i = 0;
		for (unsigned int j = 0; j < alfa.size(); j++)
			beta_i += alfa[j] * e[i][j];
		beta_i += c[i];
		beta.push_back(beta_i / 2);
	}
	return beta;
}
std::vector<unsigned int> RSABreaker::calculateAlpha(std::vector<std::vector<unsigned int> > & e, unsigned int &cIndex) {
	unsigned int n = e.size() - 1;
	std::vector<unsigned int> zero_n1;
	zero_n1.resize(n + 1, 0);
	unsigned int i, j, k, aux;
	std::vector<std::vector<unsigned int> > id;
	id.resize(n + 1, zero_n1);
	for (i = 0; i < n + 1; i++)
		id[i][i] = 1;
	std::vector<unsigned int> row;
	row.resize(n + 1, 0);
	for (i = 0; i < n + 1; i++)
		row[i] = i;
	std::vector<unsigned int> zero_n;
	zero_n.resize(n, 0);
	std::vector<std::vector<unsigned int> > e_b;
	e_b.resize(n + 1, zero_n);

	for (i = 0; i < n + 1; i++)
		for (j = 0; j < n; j++)
			e_b[i][j] = e[i][j] % 2;

	/*std::cout << "Identidad: " << std::endl;
	for (unsigned int i = 0; i < id.size(); i++) {
		for (unsigned int j = 0; j < id.size(); j++)
			std::cout << id[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << "e binaria: : " << std::endl;
	for (unsigned int i = 0; i < e_b.size(); i++) {
		for (unsigned int j = 0; j < e_b.size() - 1; j++)
			std::cout << e_b[i][j] << " ";
		std::cout << std::endl;
	}*/

	for (k = 0; k < n; k++) {
		/*std::cout << "row: " << std::endl;
		for (unsigned int ii = 0; ii < row.size(); ii++)
			std::cout << row[ii] << " ";
		std::cout << std::endl;

		std::cout << "Identidad: " << std::endl;
		for (unsigned int ii = 0; ii < id.size(); ii++) {
			for (unsigned int jj = 0; jj < id.size(); jj++)
				std::cout << id[ii][jj] << " ";
			std::cout << std::endl;
		}
		std::cout << "e binaria: : " << std::endl;
		for (unsigned int ii = 0; ii < e_b.size(); ii++) {
			for (unsigned int jj = 0; jj < e_b.size() - 1; jj++)
				std::cout << e_b[ii][jj] << " ";
			std::cout << std::endl;
		}*/

		if (e_b[k][k] == 0) {
			for (i = k + 1; i < n + 1; i++)
				if (e_b[i][k] == 1)
					break;
			if (i == n + 1)
				continue;
			aux = row[i];
			row[i] = row[k];
			row[k] = aux;
			for (j = k; j < n; j++) {
				aux = e_b[i][j];
				e_b[i][j] = e_b[k][j];
				e_b[k][j] = aux;
			}
			for (j = 0; j < n + 1; j++) {
				aux = id[i][j];
				id[i][j] = id[k][j];
				id[k][j] = aux;

			}
		}
		for (i = k + 1; i < n + 1; i++) {
			if (e_b[i][k] == 0)
				continue;

			for (j = k; j < n; j++)
				e_b[i][j] = (e_b[i][j] + e_b[k][j]) % 2;
			for (j = 0; j < n + 1; j++)
				id[i][j] = (id[i][j] + id[k][j]) % 2;
		}
	}
	//std::cout<< row[n]<<std::endl;
		cIndex = row[n];
	id[n].erase(id[n].begin()+cIndex);

	return id[n];//alpha

}
/*static std::vector<unsigned int> calculateAlpha(std::vector<std::vector<unsigned int> > & e,
 std::vector<unsigned int> c) {
 std::vector<unsigned int> zero;
 unsigned int n = e.size();
 zero.resize(n + 1, 0);
 std::vector<std::vector<unsigned int> > e_t;
 e_t.resize(n, zero);
 for (unsigned int i = 0; i < n; i++)
 for (unsigned int j = 0; j < n; j++)
 e_t[j][i] = e[i][j];
 for (unsigned int i = 0; i < n; i++)
 e_t[i][n] = c[i];

 //seguir aca
 for (i = 0; i < n; i++) {
 aux = e_t[i][dependentIndex];
 e_t[i][dependentIndex] = e_t[k][dependentIndex];
 e_t[k][dependentIndex] = aux;
 }
 if (e_t[n - 1][n - 1] % 2 == e_t[n - 1][n] % 2) {
 alpha[n - 1] = 1;
 alpha[n] = 1;
 } else {
 alpha[n - 1] = (e_t[n - 1][n - 1] + 1) % 2;
 alpha[n] = (e_t[n - 1][n] + 1) % 2;
 }
 for (i = n - 2; i >= 0; i--) {
 s = 0;
 j = i + 1;
 while (j < n) {
 s = s + e_t[i][j] * alpha[j];
 j++;
 }
 if (e_t[n - 1][n - 1] % 2 == e_t[n - 1][n] % 2) {
 alpha[n - 1] = 1;
 alpha[n] = 1;
 } else {
 alpha[n - 1] = (e_t[n - 1][n - 1] + 1) % 2;
 alpha[n] = (e_t[n - 1][n] + 1) % 2;
 }
 }

 alpha[i]=(alpha[i][n]-s)/

 }*/

RSABreaker::~RSABreaker() {
	// TODO Auto-generated destructor stub
}

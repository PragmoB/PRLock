#pragma once

#include "stdafx.h"
#include "BigInteger.h"

#include <tuple>

class RSA
{
public:
	RSA();

	BigInteger N;
	BigInteger e;
	BigInteger d;

	void SetKey(BigInteger prime1 = NULL, BigInteger prime2 = NULL);
	BigInteger Encrypt(int P);
	int Decrypt(BigInteger C);
	~RSA();

private:
	BigInteger GetPrime(BigInteger prime);
	std::tuple<BigInteger, BigInteger, BigInteger> extended_euclidean(BigInteger a, BigInteger b);
	BigInteger gcd(BigInteger a, BigInteger b);
	BOOL isPrime(BigInteger num);
	BigInteger BigRand(int mostDigit, int leastDigit);
	BigInteger fastmod(BigInteger num, BigInteger arg_power, BigInteger arg_operand);
};


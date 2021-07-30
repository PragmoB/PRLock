#pragma once

#include "stdafx.h"
#include "BigInteger/BigInteger.h"
#include <openssl/bn.h>

#include <tuple>

class RSA_Pragmo
{
public:
	RSA_Pragmo();

	BigInteger N;
	BigInteger e;
	BigInteger d;

	void SetKeyAuto(int bitlength = 60);
	void SetKey(BigInteger prime1 = NULL, BigInteger prime2 = NULL);
	BigInteger Encrypt(BigInteger P);
	BigInteger Decrypt(BigInteger C);
	~RSA_Pragmo();

private:
	std::tuple<BigInteger, BigInteger, BigInteger> extended_euclidean(BigInteger a, BigInteger b);
	BigInteger BigRand(BigInteger mod);
	//BigInteger GetPrime(BigInteger prime);
	//BOOL RSA_Pragmo::Miller_Rabin(BigInteger n);
	//BOOL isPrime(BigInteger num);
	//BigInteger fastmod(BigInteger num, BigInteger arg_power, BigInteger arg_operand);
};


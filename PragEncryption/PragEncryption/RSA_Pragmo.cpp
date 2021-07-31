#include "stdafx.h"
#include "RSA_Pragmo.h"
#include "BigInteger/BigInteger.h"
#include <openssl/bn.h>

#include <ctime>
#include <cstdlib>
#include <tuple>
#include <vector>

RSA_Pragmo::RSA_Pragmo()
{
}

RSA_Pragmo::~RSA_Pragmo()
{
}

BigInteger bnum2bint(BIGNUM* num)
{
	BigInteger result = 0;

	BOOL sign = FALSE;
	if (BN_is_negative(num))
	{
		sign = TRUE;
		BN_set_negative(num, FALSE);
	}

	BIGNUM* tmpbn = BN_new();
	for (BigInteger digit = 1; !BN_is_zero(num); digit *= 0x10000000)
	{
		BN_copy(tmpbn, num);
		BN_div_word(num, 0x10000000);
		BN_mul_word(num, 0x10000000);

		BN_sub(tmpbn, tmpbn, num);

		BigInteger tmpbi = (unsigned int)BN_get_word(tmpbn);
		result += digit * tmpbi;
		BN_div_word(num, 0x10000000);
	}

	if (sign)
	{
		result *= -1;
		BN_set_negative(num, TRUE);
	}

	BN_free(tmpbn);
	return result;
}
BIGNUM* bint2bnum(BigInteger num)
{
	BIGNUM* result = BN_new();
	BN_zero(result);

	BIGNUM* digit = BN_new();
	BIGNUM* tmpbn = BN_new();
	BN_CTX* ctx = BN_CTX_new();

	BOOL sign = FALSE;
	if (num < 0)
	{
		sign = TRUE;
		num *= -1;
	}
	for (BN_one(digit); num != 0; BN_mul_word(digit, 0x10000000))
	{
		BigInteger tmpbi = num;
		num /= 0x10000000;
		num *= 0x10000000;

		tmpbi -= num;

		BN_set_word(tmpbn, tmpbi.toUnsignedLong());

		BN_mul(tmpbn, tmpbn, digit, ctx);
		BN_add(result, result, tmpbn);
		num /= 0x10000000;
	}
	if (sign)
		BN_set_negative(result, TRUE);

	BN_free(digit);
	BN_free(tmpbn);
	BN_CTX_free(ctx);

	return result;
}

void RSA_Pragmo::SetKeyAuto(int bitlength)
{
	srand((UINT)time(NULL));
	BIGNUM *prime1 = BN_new(), *prime2 = BN_new();

	do
	{
		int bitrand;

		do
			bitrand = rand() % bitlength;
		while (bitrand < 12 || bitlength - bitrand < 12); // prime1또는 prime2의 길이가 12비트 이하면 돌림

		BN_generate_prime(prime1, bitrand, TRUE, NULL, NULL, NULL, NULL);
		BN_generate_prime(prime2, bitlength - bitrand, TRUE, NULL, NULL, NULL, NULL);

		// prime1과 prime2가 같으면 안되므로(TMI: 사실 이거 알아내느라 개고생함) 한 번 더 돌려준다.
	} while (BN_cmp(prime1, prime2) == 0);
	
	SetKey(bnum2bint(prime1), bnum2bint(prime2));

	BN_free(prime1);
	BN_free(prime2);
}

void RSA_Pragmo::SetKey(BigInteger prime1, BigInteger prime2)
{
	BigInteger EulerN, temp;

	e = NULL;
	d = NULL;
	N = prime1 * prime2;
	EulerN = (prime1 - 1) * (prime2 - 1);

	for (BigInteger i = 65557; i < EulerN; i++)
	{
		BigInteger g, y, x;

		std::tie(g, x, y) = extended_euclidean(i, EulerN);

		if (g == 1) // e와 오일러N값은 서로소여야함
			if (x > 1 && x < EulerN)
			{

				CString str;
				e = i;
				d = x;
				break;
			}
	}
}

/*
 * GCD(a, b)를 구하고 ax + by = GCD(a, b)를 만족하는 x와 y값을 구하는 과정
 * 
 */
std::tuple<BigInteger, BigInteger, BigInteger> RSA_Pragmo::extended_euclidean(BigInteger a, BigInteger b) {

	BIGNUM *tmp0 = BN_new(), *tmp1 = BN_new();
	BN_CTX* ctx = BN_CTX_new();
	BN_zero(tmp0); BN_one(tmp1);
	std::vector<BIGNUM*> s = { tmp1,tmp0 };

	tmp0 = BN_new(), tmp1 = BN_new();
	BN_zero(tmp0); BN_one(tmp1);
	std::vector<BIGNUM*> t = { tmp0,tmp1 };

	std::vector<BIGNUM*> r = { bint2bnum(a), bint2bnum(b) };
	std::vector<BIGNUM*> q;

	while (1)
	{
		tmp0 = BN_new(); tmp1 = BN_new();
		BIGNUM* r2 = r[r.size() - 2];
		BIGNUM* r1 = r[r.size() - 1];
		BN_div(tmp1, tmp0, r2, r1, ctx);
		q.push_back(tmp1);
		r.push_back(tmp0);

		if (BN_is_zero(r[r.size() - 1]))
			break;

		BIGNUM* s2 = s[s.size() - 2];
		BIGNUM* s1 = s[s.size() - 1];

		BIGNUM* t2 = t[t.size() - 2];
		BIGNUM* t1 = t[t.size() - 1];

		BIGNUM* q1 = q[q.size() - 1];
		
		tmp0 = BN_new(); tmp1 = BN_new();
		BN_mul(tmp0, s1, q1, ctx);
		BN_sub(tmp0, s2, tmp0);
		s.push_back(tmp0);
		BN_mul(tmp1, t1, q1, ctx);
		BN_sub(tmp1, t2, tmp1);
		t.push_back(tmp1);
	}
	
	// return gcd, s, t
	std::tuple<BigInteger, BigInteger, BigInteger> ret = 
	{ bnum2bint(r[r.size() - 2]), bnum2bint(s[s.size() - 1]), bnum2bint(t[t.size() - 1]) };

	for (auto tmp : s)
		BN_free(tmp);
	for (auto tmp : t)
		BN_free(tmp);
	for (auto tmp : r)
		BN_free(tmp);
	for (auto tmp : q)
		BN_free(tmp);
	BN_CTX_free(ctx);

	s.clear(), t.clear(), r.clear(), q.clear();

	return ret;
}

BigInteger RSA_Pragmo::Encrypt(BigInteger P)
{
	// TODO: 여기에 구현 코드 추가.

	BIGNUM* P_BN = bint2bnum(P);
	BIGNUM* N_BN = bint2bnum(N);
	BIGNUM* e_BN = bint2bnum(e);
	BIGNUM* C_BN = BN_new();
	BN_CTX* ctx = BN_CTX_new();

	/* C = P^e mod N */
	BN_mod_exp(C_BN, P_BN, e_BN, N_BN, ctx);
	BigInteger result = bnum2bint(C_BN);

	BN_free(P_BN);
	BN_free(N_BN);
	BN_free(e_BN);
	BN_free(C_BN);
	BN_CTX_free(ctx);

	return result;
}


BigInteger RSA_Pragmo::Decrypt(BigInteger C)
{
	// TODO: 여기에 구현 코드 추가.

	BIGNUM* C_BN = bint2bnum(C);
	BIGNUM* N_BN = bint2bnum(N);
	BIGNUM* d_BN = bint2bnum(d);
	BIGNUM* P_BN = BN_new();
	BN_CTX* ctx = BN_CTX_new();

	/* P = C^d mod N */;
	BN_mod_exp(P_BN, C_BN, d_BN, N_BN, ctx);
	BigInteger result = bnum2bint(P_BN);

	BN_free(C_BN);
	BN_free(P_BN);
	BN_free(N_BN);
	BN_free(d_BN);
	BN_CTX_free(ctx);

	return result;
}

BigInteger RSA_Pragmo::BigRand(BigInteger mod)
{
	// TODO: 여기에 구현 코드 추가.
	if (mod <= 0)
		return BigInteger(0);

	srand((UINT)time(NULL));

	BigInteger sum = 0, digit;
	BigInteger i32_1 = 0x80000000;
	i32_1 *= 2;
	for (digit = 1; sum < mod; digit *= i32_1)
		sum += digit * (UINT)rand();

	sum += digit * (UINT)rand();

	return sum % mod;
}

/*
BigInteger RSA_Pragmo::GetPrime(BigInteger prime)
{
	// TODO: 여기에 구현 코드 추가.
	if (prime % 2 == 0) // 짝수면
		prime += 1; // 홀수로 만듬

	for (; !isPrime(prime); prime += 2);
	return prime;
}

BOOL RSA_Pragmo::Miller_Rabin(BigInteger n)
{
	if (n % 2 == 0) // 짝수면 걸러야함
		return false;

	int a[] = { 2, 0 };
	BigInteger temp = n - 1, d = (n - 1) * 2;
	for (temp = n - 1; temp * 2 == d; temp /= 2) // d가 홀수가 되는 순간 조건식이 성립하지 않게됨
		d = temp;

	BOOL success = FALSE; // 아래의 조건을 한 번이라도 통과했는가?
	for (int i = 0; a[i] != 0; i++)
	{
		success = FALSE;

		if (fastmod(a[i], d, n) == 1) // a^d ≡ 1 (mod n)
			continue;

		for (BigInteger k = 1; d * k != n - 1; k *= 2)
		{
			if (fastmod(a[i], d * k, n) == n - 1) // a^(d * k) ≡ -1 (mod n)
			{
				success = TRUE;
				break;
			}
		}
		if (!success) // 위의 조건을 한번이라도 만족하지 못했다면
			return FALSE; // 합성수임
	}
	return TRUE; // 모든 a에 대해 다 통과했다면 밀러 라빈 통과
}

BOOL RSA_Pragmo::isPrime(BigInteger num)
{
	BOOL isNotPrime = TRUE;
	BigInteger root;

	// 플리히타의 소수 십자가 : 1차 거름
	for (int i = 1; i <= 24; i++)
	{
		if ((num - i) % 24 == 0) // num이 소수일 가능성이 있다면
		{
			isNotPrime = FALSE; // 일단은 소수인걸로 설정해둠
			break;
		}

		if ((i - 1) % 3 == 0) // 반복을 위한 추가 연산
			i += 3;
	}
	if (isNotPrime) // 1차 거름에서 걸러졌으므로 제외
		return FALSE;

	// 페르마의 소수 정리 : 2차 거름
	int a_arr[] = { 2, 3, 5 };
	for(int a : a_arr)
		if (fastmod(a, num - 1, num) != 1)
			return FALSE;


	// 무차별 대입 : 3차 거름

	for (root = 1; (num / root) > root; root++); // num의 제곱근을 구함.
	for (BigInteger i = 2; i <= root; i++)
	{
		if (num % i == 0) // 나누어 떨어지면 소수가 아님
			return FALSE;

	}
	return TRUE;
}

// 모듈러 곱셈법칙 이용) (A * B) % C = ((A % C) * (B % C)) % C
BigInteger RSA_Pragmo::fastmod(BigInteger num, BigInteger arg_power, BigInteger arg_operand)
{
	BigInteger result = num % arg_operand, power;
	for (power = 2; power <= arg_power; power *= 2)
		result = (result * result) % arg_operand;

	power /= 2;

	if (power == arg_power) // arg_power가 2의 거듭제곱으로만 이루어진 수 라면 ex) 1024, 256
		return result; // 결과를 바로 리턴.
	else // 아니면
		return (result * fastmod(num, arg_power - power, arg_operand)) % arg_operand;
}
*/
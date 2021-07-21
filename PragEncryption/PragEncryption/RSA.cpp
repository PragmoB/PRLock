#include "stdafx.h"
#include "RSA.h"
#include "BigInteger/BigInteger.h"

#include <ctime>
#include <cstdlib>
#include <tuple>


RSA::RSA()
{
}

RSA::~RSA()
{
}

void RSA::SetKeyAuto(int bitlength)
{
	srand((UINT)time(NULL));
	BigInteger prime1, prime2, rand, maxlength = 1;
	for (int i = 0; i < bitlength; i++)
		maxlength *= 2;
	
	BigInteger num = maxlength;
	int digit;
	for (digit = 0; num != 0; digit++) // num의 자릿수 구하기
		num /= 10;

	rand = BigRand(digit - 1, 2); // 먼저 2^bitlength / 10 의 자릿수 ~ 두 자릿수의 랜덤 수 생성
	prime1 = GetPrime(rand); // 그보다 큰 소수 생성
	num = maxlength / prime1; // N값은 2^bitlength보다 작고  2^(biglength - 1)과 같거나 크게 만들어야 하므로
	prime2 = GetPrime(num); // 대략적으로 나눈값으로 소수를 찾는다

	// prime1과 prime2가 같으면 안되므로(TMI: 사실 이거 알아내느라 개고생함) 한 번 더 돌려준다.
	if (prime1 == prime2)
		prime2 = GetPrime(prime2 + 1);

	SetKey(prime1, prime2);
}
void RSA::SetKey(BigInteger prime1, BigInteger prime2)
{
	BigInteger EulerN, temp;

	e = NULL;
	d = NULL;
	N = prime1 * prime2;
	EulerN = (prime1 - 1) * (prime2 - 1);

	for (BigInteger i = 3; i < EulerN; i++)
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

BigInteger RSA::GetPrime(BigInteger prime)
{
	// TODO: 여기에 구현 코드 추가.
	if (prime % 2 == 0) // 짝수면
		prime += 1; // 홀수로 만듬

	for (; !isPrime(prime); prime += 2);
	return prime;
}

BOOL RSA::Miller_Rabin(BigInteger n)
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

BOOL RSA::isPrime(BigInteger num)
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

/*
 * GCD(a, b) = g 라고 할 때 ax + by = g를 만족하는 x와 y값을 구하는 과정
 * 
 */
std::tuple<BigInteger, BigInteger, BigInteger> RSA::extended_euclidean(BigInteger a, BigInteger b) {
	if (b == 0)
		return std::make_tuple(a, 1, 0);
	BigInteger g, x, y;
	std::tie(g, x, y) = extended_euclidean(b, a%b);
	return std::make_tuple(g, y, x - (a / b)*y);
}

BigInteger RSA::BigRand(int mostDigit, int leastDigit)
{
	// TODO: 여기에 구현 코드 추가.

	if (leastDigit > mostDigit)
		return BigInteger(0);

	srand((UINT)time(NULL));

	int digit;
	BigInteger temp = 1;
	BigInteger result = 0;
	digit = rand() % (mostDigit - leastDigit + 1) + leastDigit;

	for (int i = 1; i < digit; i++) // digit = 자릿수
		temp *= 10;

	for (; temp >= 1; temp /= 10)
		result += temp * (rand() % 9 + 1);

	return result;
}


BigInteger RSA::Encrypt(BigInteger P)
{
	// TODO: 여기에 구현 코드 추가.

	/* C = P^e mod N */

	return fastmod(P, e, N);
}


BigInteger RSA::Decrypt(BigInteger C)
{
	// TODO: 여기에 구현 코드 추가.

	/* P = C^d mod N */
	return fastmod(C, d, N);
}


BigInteger RSA::gcd(BigInteger a, BigInteger b)
{
	// TODO: 여기에 구현 코드 추가.
	BigInteger tmp, n;

	if (a < b)
	{
		tmp = a;
		a = b;
		b = tmp;
	}

	while (b != 0)
	{
		n = a % b;
		a = b;
		b = n;
	}

	return a;
}

// 모듈러 곱셈법칙 이용) (A * B) % C = ((A % C) * (B % C)) % C
BigInteger RSA::fastmod(BigInteger num, BigInteger arg_power, BigInteger arg_operand)
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
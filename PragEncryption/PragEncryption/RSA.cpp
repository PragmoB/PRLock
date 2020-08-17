#include "stdafx.h"
#include "RSA.h"
#include "BigInteger.h"

#include <ctime>
#include <cstdlib>
#include <tuple>


RSA::RSA()
{
}

RSA::~RSA()
{
}

void RSA::SetKey(BigInteger prime1, BigInteger prime2)
{
	BigInteger EulerN, temp;
	if (prime1 == NULL || prime2 == NULL)
	{
		prime1 = 10;
		prime2 = 10;

		prime1 = GetPrime(BigRand(13, 11));
		prime2 = GetPrime(BigRand(13, 11));

		// prime1�� prime2�� ������ �ȵǹǷ�(TMI: ��� �̰� �˾Ƴ����� �������) ���� ���������� �����ش�.
		while (prime1 == prime2)
			prime2 = GetPrime(BigRand(13, 11));
	}
	srand((UINT)time(NULL));

	e = NULL;
	d = NULL;
	N = prime1 * prime2;
	EulerN = (prime1 - 1) * (prime2 - 1);

	for (BigInteger i = BigRand(10, 9); i < EulerN; i++)
	{
		BigInteger g, y, x;

		std::tie(g, x, y) = extended_euclidean(i, EulerN);
		if (g == 1)
			if (x > 1 && x < EulerN)
			{

				CString str;
				e = x;
				d = i;
				break;
			}

	}
}

BigInteger RSA::GetPrime(BigInteger prime)
{
	// TODO: ���⿡ ���� �ڵ� �߰�.

	for (; !isPrime(prime); prime++);
	return prime;
}

BOOL RSA::isPrime(BigInteger num)
{
	BOOL isNotPrime = TRUE;
	BigInteger root;

	// �ø���Ÿ�� �Ҽ� ���ڰ� : 1�� �Ÿ�
	for (int i = 1; i <= 24; i++)
	{
		if ((num - i) % 24 == 0) // num�� �Ҽ��� ���ɼ��� �ִٸ�
		{
			isNotPrime = FALSE; // �ϴ��� �Ҽ��ΰɷ� �����ص�
			break;
		}

		if ((i - 1) % 3 == 0) // �ݺ��� ���� �߰� ����
			i += 3;
	}
	if (isNotPrime) // 1�� �Ÿ����� �ɷ������Ƿ� ����
		return FALSE;

	// �丣���� �Ҽ� ���� : 2�� �Ÿ�
	if (fastmod(2, num - 1, num) != 1)
		return FALSE;


	// ������ ���� : 3�� �Ÿ�

	for (root = 1; (num / root) > root; root++); // num�� �������� ����.
	for (BigInteger i = 2; i < root; i++)
	{
		if (num % i == 0) // ������ �������� �Ҽ��� �ƴ�
			return FALSE;

	}
	return TRUE;
}


std::tuple<BigInteger, BigInteger, BigInteger> RSA::extended_euclidean(BigInteger a, BigInteger b) {
	if (b == 0)
		return std::make_tuple(a, 1, 0);
	BigInteger g, x, y;
	std::tie(g, x, y) = extended_euclidean(b, a%b);
	return std::make_tuple(g, y, x - (a / b)*y);
}

BigInteger RSA::BigRand(int mostDigit, int leastDigit)
{
	// TODO: ���⿡ ���� �ڵ� �߰�.

	if (leastDigit > mostDigit)
		return BigInteger(0);

	srand((UINT)time(NULL));

	int digit;
	BigInteger temp = 1;
	BigInteger result = 0;
	digit = rand() % (mostDigit - leastDigit + 1) + leastDigit;

	for (int i = 1; i < digit; i++) // digit = �ڸ���
		temp *= 10;

	for (; temp >= 1; temp /= 10)
		result += temp * (rand() % 9 + 1);

	return result;
}


BigInteger RSA::Encrypt(int P)
{
	// TODO: ���⿡ ���� �ڵ� �߰�.

	/* C = P^e mod N */

	return fastmod(P, e, N);
}


int RSA::Decrypt(BigInteger C)
{
	// TODO: ���⿡ ���� �ڵ� �߰�.

	/* P = C^d mod N */
	return fastmod(C, d, N).toInt();
}


BigInteger RSA::gcd(BigInteger a, BigInteger b)
{
	// TODO: ���⿡ ���� �ڵ� �߰�.
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

// ��ⷯ ������Ģ �̿�) (A * B) % C = ((A % C) * (B % C)) % C
BigInteger RSA::fastmod(BigInteger num, BigInteger arg_power, BigInteger arg_operand)
{
	BigInteger result = num % arg_operand, power;
	for (power = 2; power <= arg_power; power *= 2)
		result = (result * result) % arg_operand;

	power /= 2;

	if (power == arg_power) // arg_power�� 2�� �ŵ��������θ� �̷���� �� ��� ex) 1024, 256
		return result; // ����� �ٷ� ����.
	else // �ƴϸ�
		return (result * fastmod(num, arg_power - power, arg_operand)) % arg_operand;
}
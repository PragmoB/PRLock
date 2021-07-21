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
	for (digit = 0; num != 0; digit++) // num�� �ڸ��� ���ϱ�
		num /= 10;

	rand = BigRand(digit - 1, 2); // ���� 2^bitlength / 10 �� �ڸ��� ~ �� �ڸ����� ���� �� ����
	prime1 = GetPrime(rand); // �׺��� ū �Ҽ� ����
	num = maxlength / prime1; // N���� 2^bitlength���� �۰�  2^(biglength - 1)�� ���ų� ũ�� ������ �ϹǷ�
	prime2 = GetPrime(num); // �뷫������ ���������� �Ҽ��� ã�´�

	// prime1�� prime2�� ������ �ȵǹǷ�(TMI: ��� �̰� �˾Ƴ����� �������) �� �� �� �����ش�.
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
		if (g == 1) // e�� ���Ϸ�N���� ���μҿ�����
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
	// TODO: ���⿡ ���� �ڵ� �߰�.
	if (prime % 2 == 0) // ¦����
		prime += 1; // Ȧ���� ����

	for (; !isPrime(prime); prime += 2);
	return prime;
}

BOOL RSA::Miller_Rabin(BigInteger n)
{
	if (n % 2 == 0) // ¦���� �ɷ�����
		return false;

	int a[] = { 2, 0 };
	BigInteger temp = n - 1, d = (n - 1) * 2;
	for (temp = n - 1; temp * 2 == d; temp /= 2) // d�� Ȧ���� �Ǵ� ���� ���ǽ��� �������� �ʰԵ�
		d = temp;

	BOOL success = FALSE; // �Ʒ��� ������ �� ���̶� ����ߴ°�?
	for (int i = 0; a[i] != 0; i++)
	{
		success = FALSE;

		if (fastmod(a[i], d, n) == 1) // a^d �� 1 (mod n)
			continue;

		for (BigInteger k = 1; d * k != n - 1; k *= 2)
		{
			if (fastmod(a[i], d * k, n) == n - 1) // a^(d * k) �� -1 (mod n)
			{
				success = TRUE;
				break;
			}
		}
		if (!success) // ���� ������ �ѹ��̶� �������� ���ߴٸ�
			return FALSE; // �ռ�����
	}
	return TRUE; // ��� a�� ���� �� ����ߴٸ� �з� ��� ���
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
	int a_arr[] = { 2, 3, 5 };
	for(int a : a_arr)
		if (fastmod(a, num - 1, num) != 1)
			return FALSE;


	// ������ ���� : 3�� �Ÿ�

	for (root = 1; (num / root) > root; root++); // num�� �������� ����.
	for (BigInteger i = 2; i <= root; i++)
	{
		if (num % i == 0) // ������ �������� �Ҽ��� �ƴ�
			return FALSE;

	}
	return TRUE;
}

/*
 * GCD(a, b) = g ��� �� �� ax + by = g�� �����ϴ� x�� y���� ���ϴ� ����
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


BigInteger RSA::Encrypt(BigInteger P)
{
	// TODO: ���⿡ ���� �ڵ� �߰�.

	/* C = P^e mod N */

	return fastmod(P, e, N);
}


BigInteger RSA::Decrypt(BigInteger C)
{
	// TODO: ���⿡ ���� �ڵ� �߰�.

	/* P = C^d mod N */
	return fastmod(C, d, N);
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
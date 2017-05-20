#include <cassert>
#include <iostream>
#include <string>

#include "BigInteger.h"



// fib  :0 1 1 2 3 5 8 13
// index:0 1 2 3 4 5 6 7
BigInteger fib(const BigInteger &n)
{
	BigInteger f1 = 0, f2 = 1, f3;
	if (n == 0) return f1;

	for (BigInteger i = 2; i <= n; ++i) {
		f3 = f1 + f2;
		f1 = f2;
		f2 = f3;
	}
	return f2;
}

// fact :1 1 2 6 24 120 720
// index:0 1 2 3 4  5   6
BigInteger fact(const BigInteger &n)
{
	BigInteger result = 1;
	for (BigInteger i = 2; i <= n; ++i) {
		result *= i;
	}
	return result;
}


int main()
{
	
	{
		// Constructor
		assert(0 == BigInteger());
		assert(BigInteger() == 0);
		assert(BigInteger(12345678) == 12345678);
		assert(BigInteger("12345678") == "12345678");
		assert(BigInteger(std::string("12345678")) == std::string("12345678"));
		assert(BigInteger(INT_MAX) == INT_MAX);
		assert(BigInteger(INT_MIN) == INT_MIN);
		assert(BigInteger(LONG_MAX) == LONG_MAX);
		assert(BigInteger(LONG_MIN) == LONG_MIN);
		assert(BigInteger(LLONG_MAX) == LLONG_MAX);
		assert(BigInteger(LLONG_MIN) == LLONG_MIN);
		assert(BigInteger(UINT_MAX) == UINT_MAX);
		assert(BigInteger(ULONG_MAX) == ULONG_MAX);
		assert(BigInteger(ULLONG_MAX) == ULLONG_MAX);
		assert(BigInteger(0U) == 0U);
		assert(BigInteger(0UL) == 0UL);
		assert(BigInteger(0ULL) == 0ULL);
		assert(BigInteger("12345678") == BigInteger("12345678"));
		assert(BigInteger(1) == 1);
	}

	{
		// Relational operations
		assert(BigInteger("0000") == 0);				// leading zeros
		assert(BigInteger(-1234).absolute() == 1234);	// absolute
		
		assert(-1 < BigInteger(1));
		assert(BigInteger(-1) < 1);						// - < +
		assert(BigInteger(-1) < 0);						// - < 0
		assert(BigInteger(0) < 1);						// 0 < +						
		assert(BigInteger(1) < 2);						// + < +
		assert(BigInteger(-2) < -1);					// - < -
		assert(BigInteger(123456789) < 1234567890);		// + < + 
		
		assert(0 <= BigInteger(0));
		assert(BigInteger(0) <= 0);
		assert(BigInteger(-1) <= 1);
		assert(BigInteger(-1) <= 0);
		assert(BigInteger(0) <= 1);
		assert(BigInteger(1) <= 2);
		assert(BigInteger(-2) <= 1);
		assert(BigInteger(12345678) <= 1234567890);

		assert(1 > BigInteger(-1));
		assert(BigInteger(1) > -1);
		assert(BigInteger(0) > -1);
		assert(BigInteger(1) > 0);
		assert(BigInteger(2) > 1);
		assert(BigInteger(-1) > -2);
		assert(BigInteger(1234567890) > 123456789);

		assert(0 >= BigInteger(0));
		assert(BigInteger(0) >= 0);
		assert(BigInteger(1) >= -1);
		assert(BigInteger(0) >= -1);
		assert(BigInteger(1) >= 0);
		assert(BigInteger(2) >= 1);
		assert(BigInteger(-1) >= -2);
		assert(BigInteger(1234567890) >= 123456789);
	}

	{
		// to...
		assert(BigInteger(INT_MAX).toInt() == INT_MAX);
		assert(BigInteger(INT_MIN).toInt() == INT_MIN);
		assert(BigInteger(LONG_MAX).toLong() == LONG_MAX);
		assert(BigInteger(LONG_MIN).toLong() == LONG_MIN);
		assert(BigInteger(LLONG_MAX).toLongLong() == LLONG_MAX);
		assert(BigInteger(LLONG_MIN).toLongLong() == LLONG_MIN);
		assert(BigInteger(0U).toUnsignedInt() == 0U);
		assert(BigInteger(0UL).toUnsignedLong() == 0UL);
		assert(BigInteger(0ULL).toUnsignedLongLong() == 0ULL);
		assert(BigInteger(INT_MIN).toString() == "-2147483648");

#ifdef BIGINTEGER_EXCEPTION_ON
		try {
			const BigInteger overINT("2147483648");
			int i = overINT.toInt();
		}
		catch (const BigIntException &e) {
			std::cout << "catch:" << e.what() << std::endl;
			assert(e.what() == std::string("Out of int bounds"));
		}
#else
		BigInteger overINT("2147483648");
		int i = overINT.toInt();	// overflow

		BigInteger unsignedInt("2147483648");
		unsigned int ui = unsignedInt.toUnsignedInt(); // ok
		
		BigInteger overUINT("4294967296");
		unsigned int ui2 = overUINT.toUnsignedInt(); // overflow

		BigInteger overULONG("-1");
		unsigned long ul = overULONG.toUnsignedLong(); // overflow
#endif // BIGINTEGER_EXCEPTION_ON
	}

	{
		// Operations
		int i = 0;
		BigInteger bi1 = 0;
		while (i < 500) {
			assert(bi1 == i);
			++i;
			i++;
			++bi1;
			bi1++;
		}
		while (i > -500) {
			assert(bi1 == i);
			--i;
			i--;
			--bi1;
			bi1--;
		}
		
		bi1 = "123456789012345678901234567890";
		auto bi2 = bi1 + 1;
		assert(++bi1 == bi2);
		assert(bi1++ == bi2);
		assert(--bi1 == bi2);
		assert(bi1-- == bi2);
		assert(bi1 - bi1 == 0);

		bi2 = bi1 + 1;
		bi1 += 1;
		assert(bi2 == bi1);
		bi1 = bi2 - 1;
		bi2 -= 1;
		assert(bi2 == bi1);

		assert(1 + BigInteger(-1) == 0);
		assert(BigInteger(1) + BigInteger(-1) == 0);	// + -
		assert(BigInteger(1) + BigInteger(1) == 2);		// + +
		assert(BigInteger(-1) + BigInteger(-1) == -2);	// - -
		assert(BigInteger(-2) + 1 == -1);				// - +

		assert(1 - BigInteger(-1) == 2);
		assert(BigInteger(1) - BigInteger(-1) == 2);	// + -
		assert(BigInteger(1) - BigInteger(1) == 0);		// + +
		assert(BigInteger(-1) - BigInteger(-2) == 1);	// - -
		assert(BigInteger(0) - 1 == -1);				// - +

		assert(fib(7) == 13);
		//std::cout << fib(1000) << std::endl;

		assert(bi1 + bi1 == bi1 * 2);
		assert(0 * BigInteger(1) == 0);
		assert(BigInteger(0) * 1 == 0);		// 0 * x == 0
		assert(BigInteger(1) * 1 == 1);		// + * + == +
		assert(BigInteger(-1) * -1 == 1);	// - * - == +
		assert(BigInteger(-1) * 1 == -1);	// - * + == -
		assert(BigInteger(1) * -2 == -2);	// + * - == -

		bi2 = bi1 + bi1;
		bi1 *= 2;
		assert(bi1 == bi2);

		assert(fact(6) == 720);
		//std::cout << fact(100) << std::endl;

		assert(1260257 / BigInteger(37) == 34061);
		assert(1260257 % BigInteger(37) == 0);
		assert(BigInteger(1260257) / 37 == 34061);
		assert(BigInteger(1260257) % 37 == 0);
		
		assert(BigInteger(12345) / 123 == 12345 / 123);
		assert(BigInteger(-12345) / 123 == -12345 / 123);
		assert(BigInteger(12345) / -123 == 12345 / -123);
		assert(BigInteger(-12345) / -123 == -12345 / -123);		

		assert(BigInteger(12345) % 123 == 12345 % 123);
		assert(BigInteger(-12345) % 123 == -12345 % 123);
		assert(BigInteger(12345) % -123 == 12345 % -123);
		assert(BigInteger(-12345) % -123 == -12345 % -123);

		BigInteger m("12345678901234567890");
		BigInteger n("9876543210");
		assert((m / n) * n + m % n == m);

#ifdef BIGINTEGER_EXCEPTION_ON
		try {
			auto div0 = BigInteger(1) / 0;
		}
		catch (const BigIntException &e) {
			std::cout << "catch:" << e.what() << std::endl;
			assert(e.what() == std::string("Division by zeros"));
		}
#else
		auto div0 = BigInteger(1) / 0;
#endif // BIGINTEGER_EXCEPTION_ON

		BigInteger neg1(-1);
		auto squNeg1 = neg1 ^ 2;
		auto cubNeg1 = neg1 ^ 3;
		assert(squNeg1 == 1);
		assert(cubNeg1 == -1);

		BigInteger exp2_10(2);
		exp2_10 ^= 10;
		assert(exp2_10 == 1024);
		assert((exp2_10 ^ 0) == 1);
		assert(5 ^ BigInteger(2) == 25);

#ifdef BIGINTEGER_EXCEPTION_ON
		try {
			BigInteger exp2(2);
			exp2 ^= -1;
		}
		catch (const BigIntException &e) {
			std::cout << "catch:" << e.what() << std::endl;
			assert(e.what() == std::string("Not support negative exponent"));
		}
#else
		BigInteger exp2(2);
		exp2 ^= -1;
#endif
	}

	{
		// Other
		assert(BigInteger("1234567890").numberOfDigits() == 10);
		BigInteger bi("1234567890");
		assert(bi[0] == '1');
#ifdef BIGINTEGER_EXCEPTION_ON
		try {
			auto outRange = BigInteger("1234567890");
			char digit11 = outRange[10];
		}
		catch (const BigIntException &e) {
			std::cout << "catch:" << e.what() << std::endl;
			assert(e.what() == std::string("Invalid digit index"));
		}
#else
		auto outRange = BigInteger("1234567890");
		char digit11 = outRange[10];
#endif
		assert(gcd(1989, 1590) == 3);
		assert(lcm(3, 5) == 15);
		
		BigInteger m(1989);
		BigInteger n(1590);
		assert(gcd(m, n) == gcd(n, m));
		assert(lcm(m, n) == lcm(n, m));
		assert(gcd(gcd(m, n), 3) == gcd(m, gcd(n, 3)));
		assert(lcm(lcm(m, n), 3) == lcm(m, lcm(n, 3)));
		assert(gcd(m, lcm(m, n)) == m);
		assert(lcm(m, gcd(m, n)) == m);
	
		BigInteger oneHundred(100);
		assert(oneHundred.intSqrt() == 10);
		assert((oneHundred + 1).intSqrt() == 10);
		assert((oneHundred - 1).intSqrt() == 9);
#ifdef BIGINTEGER_EXCEPTION_ON
		try {
			BigInteger(-1).intSqrt();
		}
		catch (const BigIntException &e) {
			std::cout << "catch:" << e.what() << std::endl;
			assert(e.what() == std::string("Cannot compute negative square root"));
		}
#else
		BigInteger(-1).intSqrt();
#endif // BIGINTEGER_EXCEPTION_ON

	}


	system("pause");
	return 0;
}
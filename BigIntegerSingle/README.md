## BigInteger

BigInteger is a lightweight libary in C++11, it support basic common math operations. And the program is easy to understand with simple but enough effective algorithm.

You can see or run below example in main.cpp, also you can run a [VS2015 project](https://github.com/llgithubll/Cpp_toys/tree/master/BigInteger) with more test case to see usage.

Or just ignore below guidance.  `#inculde "BigInteger.h"`, then use it like insite type.

* You can constructe BigIngeter in very conveniences way
```
BigInteger bi0;
std::cin >> bi0;
std::cout << bi0 << std::endl;
std::cout << BigInteger() << std::endl; // 0
std::cout << BigInteger(123456789) << std::endl; // using inside type to constructe
std::cout << BigInteger("123456789") << std::endl; // using string to constructe
```
* You can do add(+), sub(-), mul(\*), div(/), mod(%), pow(^) in very easy way, just like the inside type. Also it's support +=, -=, \*=, /=, %=, ^=.
```
auto bi1 = BigInteger("987654321098765432109876543210");
BigInteger bi2 = 3;
std::cout << bi1 + bi2 << std::endl;
std::cout << bi1 - bi2 << std::endl;
std::cout << bi1 * bi2 << std::endl;
std::cout << bi1 / bi2 << std::endl;
std::cout << bi1 % bi2 << std::endl;
// Attention ^ have low priority, so it better with parameters
std::cout << (bi1 ^ bi2) << std::endl;
// It can automate convert the operands to BigInteger while operands have one BigInteger
std::cout << 3 + bi2 << std::endl;
```
* It's also support i++, ++i, i--, --i.
```
std::cout << bi2++ << std::endl;
std::cout << bi2-- << std::endl;
std::cout << ++bi2 << std::endl;
std::cout << --bi2 << std::endl;
```
* And, the gcd, lcm function you can straight use.
```
std::cout << gcd(2, "1234567890") << std::endl;
std::cout << lcm(2, "1234567890") << std::endl;
```
* Moreover, there are some addition function you can used at a BigInteger object. Like -, absolute(), intSqrt(), numnumberOfDigits(), and [] which get the nth digit in the BigInteger(n from 0).
```
std::cout << -BigInteger(1) << std::endl;
std::cout << BigInteger(-1).absolute() << std::endl;
std::cout << BigInteger(100).intSqrt() << std::endl;
std::cout << BigInteger(11111).numberOfDigits() << std::endl;
BigInteger bi3 = 123456789;
std::cout << bi3[0] << std::endl; // 1
```
* Also you can let the BigInteger by throw exception. Instead of printf wrong message to console. Just define BIGINTEGER_EXCEPTION_ON at the top of head files.
```
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
```
* You can get more detail in program^_^.

Reference：https://github.com/panks/BigInteger


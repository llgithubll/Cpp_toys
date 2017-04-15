## BigInteger
A simple BigInteger class.
It is easy to use and understand, using simple but enough effective algorithm implement operations.

### Interface
```
	// Constructor
	BigInteger() = default;
	BigInteger(const char *_c);
	BigInteger(const std::string& _s);
	BigInteger(const int _num);
	BigInteger(const long _num);
	BigInteger(const long long _num);
	BigInteger(const unsigned int _num);
	BigInteger(const unsigned long _num);
	BigInteger(const unsigned long long _num);
	BigInteger(const BigInteger& _num) = default;

	// Get, Set
	std::string getNum() const;
	void setNum(std::string &_num);
	bool getSign() const;
	void setSign(bool _sign);
	bool isNeg() const;

	// Primitive type
	int toInt() const; // throw
	long toLong() const; // throw
	long long toLongLong() const; // throw
	unsigned int toUnsignedInt() const; // throw
	unsigned long toUnsignedLong() const; // throw
	unsigned long long toUnsignedLongLong() const; // throw
	std::string toString() const;

	// Operations
	char operator [](size_t n) const; // throw(get nth digit, n begin from 0)
	BigInteger operator-() const;
	BigInteger operator+(const BigInteger &_rhs) const;
	BigInteger operator-(const BigInteger &_rhs) const;
	BigInteger operator*(const BigInteger &_rhs) const;
	BigInteger operator/(const BigInteger &_rhs) const; // throw
	BigInteger operator%(const BigInteger &_rhs) const; // throw
	BigInteger operator^(const BigInteger &_rhs) const; // throw pow(m, n) n >= 0

	// Operational assignments
	const BigInteger& operator+=(const BigInteger& _rhs);
	const BigInteger& operator-=(const BigInteger& _rhs);
	const BigInteger& operator*=(const BigInteger& _rhs);
	const BigInteger& operator/=(const BigInteger& _rhs); // throw
	const BigInteger& operator%=(const BigInteger& _rhs); // throw
	const BigInteger& operator^=(const BigInteger& _rhs); // throw pow(m, n) n >= 0

	// Increment/Decrement
	const BigInteger& operator++(); // prefix
	const BigInteger& operator--(); // prefix
	BigInteger operator++(int); // postfix
	BigInteger operator--(int); // postfix

	// Relational operations
	bool operator==(const BigInteger& _rhs) const;
	bool operator!=(const BigInteger& _rhs) const;
	bool operator< (const BigInteger& _rhs) const;
	bool operator<=(const BigInteger& _rhs) const;
	bool operator> (const BigInteger& _rhs) const;
	bool operator>=(const BigInteger& _rhs) const;

	// Other
	size_t numberOfDigits() const;
	BigInteger absolute() const;
	BigInteger intSqrt() const; // throw
	BigInteger gcd(const BigInteger &_lhs, const BigInteger &_rhs); // Other
	BigInteger lcm(const BigInteger &_lhs, const BigInteger &_rhs); // Other
	std::ostream& operator<<(std::ostream &_os, const BigInteger &_num);
	std::istream& operator >> (std::istream &_is, BigInteger &_num);

```

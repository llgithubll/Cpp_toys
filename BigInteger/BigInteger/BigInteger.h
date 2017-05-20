#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
#include <utility>
#include <cassert>


//#define BIGINTEGER_EXCEPTION_ON
#ifdef BIGINTEGER_EXCEPTION_ON
#include <exception>

class BigIntException : public std::exception {
public:
	BigIntException(const std::string& _txt) throw();
	~BigIntException() throw();
	const char* what() const throw();
private:
	std::string txt;
};

#endif

class BigInteger {
	friend std::ostream& operator<<(std::ostream &_os, const BigInteger &_num);
	friend std::istream& operator >> (std::istream &_is, BigInteger &_num);
	friend BigInteger gcd(const BigInteger &_lhs, const BigInteger &_rhs); // Other
	friend BigInteger lcm(const BigInteger &_lhs, const BigInteger &_rhs); // Other

	friend bool operator==(const BigInteger &_lhs, const BigInteger &_rhs);
	friend bool operator!=(const BigInteger &_lhs, const BigInteger &_rhs);
	friend bool operator< (const BigInteger &_lhs, const BigInteger &_rhs);
	friend bool operator<=(const BigInteger &_lhs, const BigInteger &_rhs);
	friend bool operator> (const BigInteger &_lhs, const BigInteger &_rhs);
	friend bool operator>=(const BigInteger &_lhs, const BigInteger &_rhs);

	friend BigInteger operator+(const BigInteger &_lhs, const BigInteger &_rhs);
	friend BigInteger operator-(const BigInteger &_lhs, const BigInteger &_rhs);
	friend BigInteger operator*(const BigInteger &_lhs, const BigInteger &_rhs);
	friend BigInteger operator/(const BigInteger &_lhs, const BigInteger &_rhs); // throw
	friend BigInteger operator%(const BigInteger &_lhs, const BigInteger &_rhs); // throw
	friend BigInteger operator^(const BigInteger &_lhs, const BigInteger &_rhs); // throw pow(m, n) n >= 0
private:
	std::string num = "0";
	bool sign = false; // true: negative, false:positive
public:
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
	std::string getNum() const { return num; }
	void setNum(std::string &_num) { num = _num; }
	bool getSign() const { return sign; }
	void setSign(bool _sign) { sign = _sign; }
	bool isNeg() const { return sign == true; }

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
	BigInteger add(const BigInteger &_rhs) const;
	BigInteger sub(const BigInteger &_rhs) const;
	BigInteger mul(const BigInteger &_rhs) const;
	BigInteger div(const BigInteger &_rhs) const; // throw
	BigInteger mod(const BigInteger &_rhs) const; // throw
	BigInteger pow(const BigInteger &_rhs) const; // throw pow(m, n) n >= 0

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

	// Other
	size_t numberOfDigits() const;
	BigInteger absolute() const;
	BigInteger intSqrt() const; // throw

private:
	bool equals(const BigInteger& _rhs) const;
	bool less(const BigInteger& _rhs) const;
	bool greater(const BigInteger& _rhs) const;

	std::string addNum(std::string num1, std::string num2) const;
	std::string subNum(std::string num1, std::string num2) const;
	std::string mulNum(std::string num1, std::string num2) const;
	std::pair<std::string, std::string>					   
		divNum(std::string num1, std::string num2) const;		   

	void removeLeadingZeros(std::string &num) const;
};

std::ostream& operator<<(std::ostream &_os, const BigInteger &_num);
std::istream& operator >> (std::istream &_is, BigInteger &_num);
BigInteger gcd(const BigInteger &_lhs, const BigInteger &_rhs); // Other
BigInteger lcm(const BigInteger &_lhs, const BigInteger &_rhs); // Other

BigInteger operator+(const BigInteger &_lhs, const BigInteger &_rhs);
BigInteger operator-(const BigInteger &_lhs, const BigInteger &_rhs);
BigInteger operator*(const BigInteger &_lhs, const BigInteger &_rhs);
BigInteger operator/(const BigInteger &_lhs, const BigInteger &_rhs); // throw
BigInteger operator%(const BigInteger &_lhs, const BigInteger &_rhs); // throw
BigInteger operator^(const BigInteger &_lhs, const BigInteger &_rhs); // throw pow(m, n) n >= 0

bool operator==(const BigInteger &_lhs, const BigInteger &_rhs);
bool operator!=(const BigInteger &_lhs, const BigInteger &_rhs);
bool operator< (const BigInteger &_lhs, const BigInteger &_rhs);
bool operator<=(const BigInteger &_lhs, const BigInteger &_rhs);
bool operator> (const BigInteger &_lhs, const BigInteger &_rhs);
bool operator>=(const BigInteger &_lhs, const BigInteger &_rhs);

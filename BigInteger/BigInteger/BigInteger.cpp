#include "BigInteger.h"

#ifdef BIGINTEGER_EXCEPTION_ON
BigIntException::BigIntException(const std::string & _txt) throw()
	:std::exception(), txt(_txt)
{
}

BigIntException::~BigIntException() throw()
{
}

const char * BigIntException::what() const throw()
{
	return txt.c_str();
}
#endif

BigInteger::BigInteger(const char * _c)
	:BigInteger(std::string(_c))
{
}

BigInteger::BigInteger(const std::string & _s)
{
	if (_s[0] == '-') {
		sign = true;
		num = _s.substr(1);
	}
	else {
		num = _s;
	}
	for (int i = 0; i < num.size(); i++) {
		if (!isdigit(num[i])) {
#ifdef BIGINTEGER_EXCEPTION_ON
			throw BigIntException("Invalid integer");
#else
			std::cerr << "Invalid integer" << std::endl;
#endif
			break;
		}
	}
	removeLeadingZeros(num);
}

BigInteger::BigInteger(const int _num)
	:BigInteger(std::to_string(_num))
{
}

BigInteger::BigInteger(const long _num)
	:BigInteger(std::to_string(_num))
{
}

BigInteger::BigInteger(const long long _num)
	:BigInteger(std::to_string(_num))
{
}

BigInteger::BigInteger(const unsigned int _num)
	:BigInteger(std::to_string(_num))
{
}

BigInteger::BigInteger(const unsigned long _num)
	:BigInteger(std::to_string(_num))
{
}

BigInteger::BigInteger(const unsigned long long _num)
	:BigInteger(std::to_string(_num))
{
}

int BigInteger::toInt() const
{
	if (*this < INT_MIN || *this > INT_MAX) {
#ifdef BIGINTEGER_EXCEPTION_ON
		throw BigIntException("Out of int bounds");
#else
		std::cerr << "Out of int bounds:" << *this << std::endl;
#endif
	}
	else {
		return std::stoi((sign ? "-":"") + num);
	}
	return 0;
}

long BigInteger::toLong() const
{
	if (*this < LONG_MIN || *this > LONG_MAX) {
#ifdef BIGINTEGER_EXCEPTION_ON
		throw BigIntException("Out of long bounds");
#else
		std::cerr << "Out of long bounds:" << *this << std::endl;
#endif
	}
	else {
		return std::stol((sign ? "-":"") + num);
	}
	return 0;
}

long long BigInteger::toLongLong() const
{
	if (*this < LLONG_MIN || *this > LLONG_MAX) {
#ifdef BIGINTEGER_EXCEPTION_ON
		throw BigIntException("Out of long long bounds");
#else
		std::cerr << "Out of long long bounds:" << *this << std::endl;
#endif
	}
	else {
		return std::stoll((sign ? "-":"") + num);
	}
	return 0;
}

unsigned int BigInteger::toUnsignedInt() const
{
	if (sign || *this > UINT_MAX) {
#ifdef BIGINTEGER_EXCEPTION_ON
		throw BigIntException("Out of unsigned int bounds");
#else
		std::cerr << "Out of unsigned int bounds:" << *this << std::endl;
#endif
	}
	else {
		return stoul(num); // int auto cast to unsigned int ?
	}
	return 0;
}

unsigned long BigInteger::toUnsignedLong() const
{
	if (sign || *this > ULONG_MAX) {
#ifdef BIGINTEGER_EXCEPTION_ON
		throw BigIntException("Out of unsigned long bounds");
#else
		std::cerr << "Out of unsigned long bounds:" << *this << std::endl;
#endif
	}
	else {
		return std::stoul(num);
	}
	return 0;
}

unsigned long long BigInteger::toUnsignedLongLong() const
{
	if (sign || *this > ULLONG_MAX) {
#ifdef BIGINTEGER_EXCEPTION_ON
		throw BigIntException("Out of unsigned long long bounds");
#else
		std::cerr << "Out of unsigned long long bounds:" << *this << std::endl;
#endif
	}
	else {
		return std::stoull(num);
	}
	return 0;
}

std::string BigInteger::toString() const
{
	return (sign ? "-" : "") + num;
}



bool BigInteger::equals(const BigInteger & _rhs) const
{
	return (sign == _rhs.sign) 
		&& (num.size() == _rhs.num.size()) 
		&& num == _rhs.num;
}

bool BigInteger::less(const BigInteger & _rhs) const
{
	if (sign && !_rhs.sign) return true; // - is less than +
	else if (!sign && _rhs.sign) return false; // + is not less than -
	else if (sign && _rhs.sign) { // -, -
		if (num.size() > _rhs.num.size()) return true;
		else if (num.size() < _rhs.num.size()) return false;
		else return num > _rhs.num;
	}
	else { // +, +
		if (num.size() < _rhs.num.size()) return true;
		else if (num.size() > _rhs.num.size()) return false;
		else return num < _rhs.num;
	}
}

bool BigInteger::greater(const BigInteger & _rhs) const
{
	return !equals(_rhs) && !less(_rhs);
}

std::string BigInteger::addNum(std::string num1, std::string num2) const
{
	std::string sum = (num1.size() > num2.size()) ? num1 : num2;
	char carry = '0';
	int diffSize = (num1.size() > num2.size()) ?
		(num1.size() - num2.size()) : (num2.size() - num1.size());

	if (num1.size() > num2.size()) num2.insert(0, diffSize, '0');
	else						   num1.insert(0, diffSize, '0');

	for (int i = num1.size() - 1; i >= 0; i--) {
		sum[i] = ((carry - '0') + (num1[i] - '0') + (num2[i] - '0')) + '0';
		if (i != 0) {
			if (sum[i] > '9') {
				sum[i] -= 10;
				carry = '1';
			}
			else {
				carry = '0';
			}
		}
	}
	if (sum[0] > '9') {
		sum[0] -= 10;
		sum.insert(0, 1, '1');
	}
	return sum;
}

std::string BigInteger::subNum(std::string num1, std::string num2) const
{
	std::string diff = (num1.size() > num2.size()) ? num1 : num2;
	int diffSize = (num1.size() > num2.size()) ?
		num1.size() - num2.size() : num2.size() - num1.size();

	if (num1.size() > num2.size()) num2.insert(0, diffSize, '0');
	else						   num1.insert(0, diffSize, '0');

	for (int i = num1.size() - 1; i >= 0; i--) {
		if (num1[i] < num2[i]) {
			num1[i] += 10;
			num1[i - 1]--; // because assume num1 >= num2, we will not access num1[0 - 1]
		}
		diff[i] = ((num1[i] - '0') - (num2[i] - '0')) + '0';
	}
	removeLeadingZeros(diff);
	return diff;
}

std::string BigInteger::mulNum(std::string num1, std::string num2) const
{
	if (num1.size() > num2.size()) num1.swap(num2);

	std::string res = "0";
	for (int i = num1.size() - 1; i >= 0; i--) {
		std::string temp = num2;
		int curDigit = num1[i] - '0';
		int carry = 0;

		for (int j = temp.size() - 1; j >= 0; j--) {
			int proDigit = ((temp[j] - '0') * curDigit) + carry;

			if (proDigit > 9) {
				carry = proDigit / 10;
				proDigit = proDigit % 10;
			}
			else {
				carry = 0;
			}

			temp[j] = proDigit + '0';
		}

		if (carry > 0) {
			temp.insert(0, 1, carry + '0');
		}
		temp.append(num1.size() - i - 1, '0');
		res = addNum(res, temp);
	}

	removeLeadingZeros(res);
	return res;
}

std::pair<std::string, std::string> BigInteger::divNum(std::string num1, std::string num2) const
{ 
	// algorithm: simulate traditional long division(https://en.wikipedia.org/wiki/Long_division)
	std::string quo, rem;

	std::string tmpNumer;
	for (int i = 0; i < num1.size(); i++) {
		tmpNumer.push_back(num1[i]);
		int digitQuo = 0;
		while (tmpNumer.size() > num2.size() ||
			(tmpNumer.size() == num2.size() && tmpNumer >= num2)) {
			
			tmpNumer = subNum(tmpNumer, num2);
			if (tmpNumer == "0") tmpNumer = "";
			
			digitQuo++;
		}
		quo.push_back(digitQuo + '0');
	}

	rem = (tmpNumer == "") ? "0": tmpNumer;
	removeLeadingZeros(quo);
	return std::pair<std::string, std::string>(quo, rem);
}


size_t BigInteger::numberOfDigits() const
{
	return num.size();
}

BigInteger BigInteger::absolute() const
{
	return BigInteger(num);
}

BigInteger BigInteger::intSqrt() const
{
	if (*this < 0) {
#ifdef BIGINTEGER_EXCEPTION_ON
		throw BigIntException("Cannot compute negative square root");
#else
		std::cerr << "Cannot compute negative square root:" << *this << std::endl;
#endif // BIGINTEGER_EXCEPTION_ON
	}

	if (*this == 0 || *this == 1) return *this;

	BigInteger start = 1, end = *this, mid, ans;
	while (start <= end) {
		mid = (start + end) / 2;

		if (mid * mid == *this) {
			return mid;
		}
		else if (mid * mid < *this) {
			start = mid + 1;
			ans = mid;
		}
		else {
			end = mid - 1;
		}
	}
	return ans;
}

void BigInteger::removeLeadingZeros(std::string &num) const
{
	int i = 0;
	while (i < num.size()) {
		if (num[i] == '0') i++;
		else break;
	}
	if (i == num.size()) num = "0";
	else num = num.substr(i);
}

std::ostream & operator<<(std::ostream & _os, const BigInteger & _num)
{
	_os << _num.toString();
	return _os;
}

std::istream & operator >> (std::istream & _is, BigInteger & _num)
{
	std::string s;
	_is >> s;
	if (_is) {
		_num = BigInteger(s);
	}
	else {
		_num = BigInteger();
	}
	return _is;
}

BigInteger gcd(const BigInteger & _lhs, const BigInteger & _rhs)
{
	if (_lhs < 0 || _rhs < 0) {
#ifdef BIGINTEGER_EXCEPTION_ON
		throw BigIntException("parameters of gcd should not negative");
#else
		std::cerr << "parameters of gcd should not negative:("
			<< _lhs << "," << _rhs << ")" << std::endl;
#endif // BIGINTEGER_EXCEPTION_ON
	}

	BigInteger r, m = _lhs, n = _rhs;
	while (n > 0) {
		r = m % n;
		m = n;
		n = r;
	}
	return m;
}

BigInteger lcm(const BigInteger & _lhs, const BigInteger & _rhs)
{
	return _lhs * _rhs / gcd(_lhs, _rhs);
}

BigInteger operator+(const BigInteger & _lhs, const BigInteger & _rhs)
{
	return _lhs.add(_rhs);
}

BigInteger operator-(const BigInteger & _lhs, const BigInteger & _rhs)
{
	return _lhs.sub(_rhs);
}

BigInteger operator*(const BigInteger & _lhs, const BigInteger & _rhs)
{
	return _lhs.mul(_rhs);
}

BigInteger operator/(const BigInteger & _lhs, const BigInteger & _rhs)
{
	return _lhs.div(_rhs);
}

BigInteger operator%(const BigInteger & _lhs, const BigInteger & _rhs)
{
	return _lhs.mod(_rhs);
}

BigInteger operator^(const BigInteger & _lhs, const BigInteger & _rhs)
{
	return _lhs.pow(_rhs);
}

bool operator==(const BigInteger & _lhs, const BigInteger & _rhs)
{
	return _lhs.equals(_rhs);
}

bool operator!=(const BigInteger & _lhs, const BigInteger & _rhs)
{
	return !_lhs.equals(_rhs);
}

bool operator<(const BigInteger & _lhs, const BigInteger & _rhs)
{
	return _lhs.less(_rhs);
}

bool operator<=(const BigInteger & _lhs, const BigInteger & _rhs)
{
	return _lhs.less(_rhs) || _lhs.equals(_rhs);
}

bool operator>(const BigInteger & _lhs, const BigInteger & _rhs)
{
	return _lhs.greater(_rhs);
}

bool operator>=(const BigInteger & _lhs, const BigInteger & _rhs)
{
	return !_lhs.less(_rhs);
}


char BigInteger::operator[](size_t n) const
{
	if (n >= numberOfDigits()) {
#ifdef BIGINTEGER_EXCEPTION_ON
		throw BigIntException("Invalid digit index");
#else
		std::cerr << "Invalid digit index:" << n << std::endl;
		return -1;
#endif
	}
	return num[n];
}

BigInteger BigInteger::operator-() const
{
	BigInteger opposite = *this;
	opposite.setSign(!sign);
	return opposite;
}

BigInteger BigInteger::add(const BigInteger & _rhs) const
{
	BigInteger addition;
	if (getSign() == _rhs.getSign()) {
		addition.setNum(addNum(getNum(), _rhs.getNum()));
		addition.setSign(getSign());
	}
	else {
		if (absolute() > _rhs.absolute()) {
			addition.setNum(subNum(getNum(), _rhs.getNum()));
			addition.setSign(getSign());
		}
		else {
			addition.setNum(subNum(_rhs.getNum(), getNum()));
			addition.setSign(_rhs.getSign());
		}
	}
	if (addition.getNum() == "0") addition.setSign(false); // avoid -0
	return addition;
}

BigInteger BigInteger::sub(const BigInteger & _rhs) const
{
	BigInteger opposite = _rhs;
	opposite.setSign(!_rhs.sign);
	return (*this) + opposite;
}

BigInteger BigInteger::mul(const BigInteger & _rhs) const
{
	BigInteger res;
	res.setNum(mulNum(num, _rhs.getNum()));
	
	if (sign ^ _rhs.getSign()) { // +,- or -,+
		res.setSign(true);		
	}
	else { // +,+ or -,-
		res.setSign(false);
	}
	return res;
}

BigInteger BigInteger::div(const BigInteger & _rhs) const
{
	if (_rhs == 0) {
#ifdef BIGINTEGER_EXCEPTION_ON
		throw BigIntException("Division by zeros");
#else
		std::cerr << "Division by zeros" << std::endl;
		return 0;
#endif
	}

	// C++11 standard:
	// (-m) / n == -(m / n)
	// m / (-n) == -(m / n)
	auto ans = divNum(num, _rhs.getNum());
	auto quo = BigInteger(ans.first);

	if (sign == _rhs.getSign()) { // + / + or - / -
		return quo;
	}
	else { // - / + or + / -
		quo.setSign(true);
		return quo;
	}
}

BigInteger BigInteger::mod(const BigInteger & _rhs) const
{
	if (_rhs == 0) {
#ifdef BIGINTEGER_EXCEPTION_ON
		throw BigIntException("Division by zeros");
#else
		std::cerr << "Division by zeros" << std::endl;
		return 0;
#endif
	}
	
	// C++11 standard:
	// (-m) % n == -(m % n)
	// m % (-n) == m % n
	auto ans = divNum(num, _rhs.getNum());
	auto rem = BigInteger(ans.second);
	
	if (sign) return -rem;
	else return rem;
}

BigInteger BigInteger::pow(const BigInteger & _rhs) const
{
	// m ^ n is pow(m, n)
	if (_rhs < 0) {
#ifdef BIGINTEGER_EXCEPTION_ON
		throw BigIntException("Not support negative exponent");
#else
		std::cerr << "Not support negative exponent:" << _rhs << std::endl;
#endif
	}
	
	if (_rhs == 0) return BigInteger(1);
	
	BigInteger x = *this;
	BigInteger n = _rhs;
	BigInteger y(1);
	while (n > 1) {
		if ((n.getNum()[n.numberOfDigits() - 1] - '0') % 2 == 0) { // if n is even
			x = x * x;
			n = n / 2;
		}
		else {
			y = x * y;
			x = x * x;
			n = (n - 1) / 2;
		}
	}

	return x * y;
}

const BigInteger & BigInteger::operator+=(const BigInteger & _rhs)
{
	*this = *this + _rhs;
	return *this;
}

const BigInteger & BigInteger::operator-=(const BigInteger & _rhs)
{
	*this = *this - _rhs;
	return *this;
}

const BigInteger & BigInteger::operator*=(const BigInteger & _rhs)
{
	*this = *this * _rhs;
	return *this;
}

const BigInteger & BigInteger::operator/=(const BigInteger & _rhs)
{
	*this = *this / _rhs;
	return *this;
}

const BigInteger & BigInteger::operator%=(const BigInteger & _rhs)
{
	*this = *this % _rhs;
	return *this;
}

const BigInteger & BigInteger::operator^=(const BigInteger & _rhs)
{
	*this = *this ^ _rhs;
	return *this;
}

const BigInteger & BigInteger::operator++()
{
	*this = *this + 1;
	return *this;
}

const BigInteger & BigInteger::operator--()
{
	*this = *this - 1;
	return *this;
}

BigInteger BigInteger::operator++(int)
{
	BigInteger before = *this;
	*this = *this + 1;
	return before;
}

BigInteger BigInteger::operator--(int)
{
	BigInteger before = *this;
	*this = *this - 1;
	return before;
}

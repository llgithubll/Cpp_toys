/* @vzch
Term = <number> | '(' Exp ')'
Factor = Term (('*' | '/') Term)*
Exp = Factor (('+' | '-') Factor)*
*/

#pragma once
#include <sstream>
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>

const std::string WrongOperator = "Wrong operator";
const std::string NeedsExpression = "This needs expression";
const std::string NeedsRightParenthesis = "This needs a ')'";

class Exception {
public:
	std::string context;
	std::string error;
	Exception(const std::string& _context, const std::string& _error)
		:context(_context), error(_error) {}
};

enum BinaryOperator
{
	Plus = '+',
	Minus = '-',
	Multiply = '*',
	Divide = '/',
	NonOperator = '~'
};

class Expression {
private:
	bool isLeft;
public:
	Expression() {
		isLeft = false;
	}
	virtual ~Expression() = default;
	virtual double eval() = 0;
};

class NumberExpression: public Expression {
private:
	double value;
public:
	explicit NumberExpression(double _value)
		:value(_value) {}
	double eval() override {
		return value;
	}
};

class BinaryExpression : public Expression {
private:
	BinaryOperator op;
	typedef std::unique_ptr<Expression> Uptr;
	Uptr first;
	Uptr second;
public:
	BinaryExpression(BinaryOperator _op, Uptr _first, Uptr _second)
		:op(_op), first(std::move(_first)), second(std::move(_second)) {} // std::move is necessary
	double eval() override {
		auto lhs = first->eval();
		auto rhs = second->eval();
		switch (op) {
		case Plus:
			return lhs + rhs;
		case Minus:
			return lhs - rhs;
		case Multiply:
			return lhs * rhs;
		case Divide:
			return lhs / rhs;
		default:
			throw Exception("", WrongOperator);
		}
	}
};

// function's declarations
std::unique_ptr<NumberExpression> getNumber(std::stringstream& stream);
std::unique_ptr<Expression> getTerm(std::stringstream& stream);
std::unique_ptr<Expression> getFactor(std::stringstream& stream);
std::unique_ptr<Expression> getExp(std::stringstream& stream);
std::unique_ptr<Expression> getExp(std::string exp);

// skip space and read next character
bool nextIs(std::stringstream& stream, const char ch) {
	auto oldPos = stream.tellg();
	while (stream.peek() == ' ') {
		stream.get();
	}
	if (!stream.eof() && stream.peek() == ch) {
		stream.get();
		return true;
	}
	else {
		stream.seekg(oldPos);
		return false;
	}
}

std::unique_ptr<NumberExpression> getNumber(std::stringstream& stream) {
	bool gotInteger = false;
	bool gotFraction = false;
	auto integer = 0;
	auto fraction = 0.0;
	while (stream.peek() == ' ') {
		stream.get();
	}
	int sign = 1;
	if (stream.peek() == '-' || stream.peek() == '+') {
		sign = stream.peek() == '-' ? -1 : 1;
		stream.get();
	}
	for (auto ch = stream.peek(); '0' <= ch && ch <= '9'; ch = stream.peek()) {
		integer = integer * 10 + (ch - '0');
		gotInteger = true;
		stream.get();
	}
	if (stream.peek() == '.') {
		stream.get();
	}
	double pow = 0.1;
	for (auto ch = stream.peek(); '0' <= ch && ch <= '9'; ch = stream.peek()) {
		fraction = fraction + (ch - '0') * pow;
		pow *= 0.1;
		gotFraction = true;
		stream.get();
	}
	if (gotInteger || gotFraction) {
		return std::make_unique<NumberExpression>(sign * (integer + fraction));
	}
	else {
		throw Exception(stream.str(), NeedsExpression);
	}
}

std::unique_ptr<Expression> getTerm(std::stringstream& stream) {
	try {
		return getNumber(stream);
	}
	catch (Exception e) {
		if (nextIs(stream, '(')) {
			auto result = getExp(stream);
			if (!nextIs(stream, ')')) {
				throw Exception(stream.str(), NeedsRightParenthesis);
			}
			return result;
		}
		throw;
	}
}

std::unique_ptr<Expression> getFactor(std::stringstream& stream) {
	auto result = getTerm(stream);
	while (true) {
		BinaryOperator op = NonOperator;
		if (nextIs(stream, '*')) {
			op = Multiply;
		}
		else if (nextIs(stream, '/')) {
			op = Divide;
		}
		else {
			break;
		}
		result = std::make_unique<BinaryExpression>(op, std::move(result), getTerm(stream));
	}
	return result;
}

std::unique_ptr<Expression> getExp(std::stringstream& stream) {
	auto result = getFactor(stream);
	while (true) {
		BinaryOperator op = NonOperator;
		if (nextIs(stream, '+')) {
			op = Plus;
		}
		else if (nextIs(stream, '-')) {
			op = Minus;
		}
		else {
			break;
		}
		result = std::make_unique<BinaryExpression>(op, std::move(result), getFactor(stream));
	}
	return result;
}

std::unique_ptr<Expression> getExp(std::string exp) {
	std::stringstream ss(exp);
	return getExp(ss);
}
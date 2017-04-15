#include <cassert>
#include <cmath>
#include "ExpressionAST.h"

bool equal(double lhs, double rhs) {
	return fabs(lhs - rhs) < 0.001;
}

int main() {
	{ // 0
		auto str = "1*2    /(3- 5)";
		auto exp = getExp(str);
		assert (equal(exp->eval(), -1));
	}
	{ // 1
		auto str = "1/0";
		auto exp = getExp(str);
		std::cout << str << "= " << exp->eval() << std::endl;
		assert(exp->eval() == INFINITY);
	}
	{ // 2
		try {
			auto str = "2  + )3";
			getExp(str);
		}
		catch (Exception e) {
			std::cout << e.error << std::endl;
			assert(e.error == NeedsExpression);
		}
	}
	{ // 3
		try {
			auto str = "2 * (3 + 4";
			auto exp = getExp(str);
		}
		catch (Exception e) {
			std::cout << e.error << std::endl;
			assert(e.error == NeedsRightParenthesis);
		}
	}
	{ // 4
		try {
			auto brokenExp = BinaryExpression(static_cast<BinaryOperator>(1), std::make_unique<NumberExpression>(1), std::make_unique<NumberExpression>(1));
			brokenExp.eval();
		}
		catch (Exception e) {
			std::cout << e.error << std::endl;
			assert(std::string(e.error) == WrongOperator);
		}
	}
	{ // 5
		try {
			auto str = ")3 + 1";
			auto exp = getExp(str);
		}
		catch (Exception e) {
			std::cout << e.error << std::endl;
			assert(e.error == NeedsExpression);
		}
	}
	{ // 6
		auto str = "-1.23456 * (+3 - (-4))";
		auto exp = getExp(str);
		std::cout << exp->eval() << std::endl;
		assert(equal(exp->eval(), -1.23456*(+3 - (-4))));
	}
	{ // 7
	  // so this program would calculate the legal expression at the beginning
		// and ignore the illegal expression behind
		auto str = "2 * 2 & 2 * 2"; 
		auto exp = getExp(str);
		assert(equal(exp->eval(), 4));
	}
	std::cout << "*****success*****" << std::endl;
	system("pause");
	return 0;
}
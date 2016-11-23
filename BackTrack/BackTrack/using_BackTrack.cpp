#include <vector>
#include <cassert>
#include <functional>
#include <iostream>
#include "BackTrack.h"

// This file using n-queens problem to show the usage of BackTrack
const int NQueens = 8;

// 1. Define the types
// I is the iterator type for the container.
// T is the type of data being stored in the container.
// V is a user-defined boolean function that will return true if current decision tree is valid.
typedef std::vector<int> Queen;
typedef Queen::iterator QueenIter;

// 2. Define the operators
// BackTrack uses the following operators on T: &, ==, !=, =, ++(prefix).
// The compiler predefines these operators for int, so we let T is int, these work is done.


// 3. Define a Validator function
// Derive the validator function from std::binary_function
// The validator function takes iterators pointing to the beginning and 
// the end of container holding the tree. This function should return true
// if the condition is valid.
// Remember that at the time of the call every element except the last has already been
// checked for validity, so you only need to check that the back element,*(end-1),is valid.
struct QueenIsValid : public std::binary_function<QueenIter, QueenIter, bool> {
	bool operator()(const QueenIter& begin, const QueenIter& end) const {
		auto thisRow = end - begin - 1;
		auto thisColumn = *(end - 1);
		for (int i = 0; i < thisRow; i++) {
			int columnGap = *(begin + i) - thisColumn;
			columnGap = columnGap < 0 ? -columnGap : columnGap;
			if (columnGap == 0 || columnGap == thisRow - i) {
				return false;
			}
		}
		return true;
	}
};

// ulility
void printQueen(std::vector<int> &result, int resultCount) {
	std::cout << "Solution " << resultCount << ":" << std::endl;
	for (int i = 0; i < result.size(); i++) {
		for (int j = 0; j < result.size(); j++) {
			std::cout << ((j == result[i]) ? "Q " : ". ");
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main() {
	Queen queensResult(NQueens);
	// 4. Construct a BackTrack object
	// To generate the decision tree the algorithm needs to know the valid range for T
	// The BackTrack constructor accepts these values as parameters:
	// Provide T, I, V for construct template
	// and first, last for the begin and end state.
	// BackTrack(const T& first, const T& last) 
	BackTrack<int, QueenIter, QueenIsValid> eightQueen(0, NQueens - 1);

	// 5. Call it
	// BackTrack's operator() takes the begin and end iterators to your container,
	// along with a bool parameter that specifies whether this is the first call.
	// the operator() return true, if a valid solution was found,
	// and the queensResult now contains the coloring for each state.
	// Once you have found the first solution you can call operator() again to get
	// next solution. Because the queensResult store the decision tree
	// it can backtrack from current solution, and find next valid solution.
	// and the firstTime is set to false when operator() is called,
	// guaranteeing that subsequent calls find the next solution
	bool firstTime = true;
	int resultCount = 0;
	bool valid = eightQueen(queensResult.begin(), queensResult.end(), firstTime);
	while (valid) {
		printQueen(queensResult, ++resultCount);
		valid = eightQueen(queensResult.begin(), queensResult.end(), firstTime);
	}
	system("pause");
	return 0;
}
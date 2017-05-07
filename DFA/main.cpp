#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "DFA.h"

using namespace std;

int main()
{
	// a(b|c)*
	DFA<char> dfa(0, false);
	dfa.addState(1, true);
	dfa.addState(2, true);
	dfa.addState(3, true);
	dfa.addTransition(0, 'a', 1);
	dfa.addTransition(1, 'b', 2);
	dfa.addTransition(1, 'c', 3);
	dfa.addTransition(2, 'b', 2);
	dfa.addTransition(2, 'c', 3);
	dfa.addTransition(3, 'b', 2);
	dfa.addTransition(3, 'c', 3);

	vector<char> canAccepted = { 'a', 'b', 'c', 'b', 'c' };
	for (int i = 0; i < canAccepted.size(); i++) {
		dfa.input(canAccepted[i]);
	}
	assert(dfa.isAccepting());

	dfa.reset();
	vector<char> cannotAccepted = { 'a', 'a' };
	for (int i = 0; i < cannotAccepted.size(); i++) {
		dfa.input(cannotAccepted[i]);
	}
	assert(!dfa.isAccepting());

	return 0;
}

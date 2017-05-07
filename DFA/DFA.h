#include <set>
#include <map>
#include <utility>

using std::map;
using std::set;
using std::pair;
using std::make_pair;

template <typename T>
class DFA {
public:
	explicit DFA(int _initState, bool _isFinal);
	~DFA();
	void addState(int _state, bool _isFinal);
	void addTransition(int _beginState, T _input, int _endState);
	bool isAccepting();
	void reset();
	int input(T _input);
	int state();
private:
	int initState;
	int curState;
	set<int> states;
	set<int> finalStates;
	map<pair<int, T>, int> transitions;
};

template<typename T>
inline DFA<T>::DFA(int _initState, bool _isFinal)
{
	initState = curState = _initState;
	addState(initState, _isFinal);
	addState(-1, false); // -1 is invalid state
}

template<typename T>
inline DFA<T>::~DFA()
{
}

template<typename T>
inline void DFA<T>::addState(int _state, bool _isFinal)
{
	states.insert(_state);
	if (_isFinal) finalStates.insert(_state);
}

template<typename T>
inline void DFA<T>::addTransition(int _beginState, T _input, int _endState)
{
	transitions.insert(make_pair(make_pair(_beginState, _input), _endState));
}

template<typename T>
inline bool DFA<T>::isAccepting()
{
	return finalStates.count(curState) > 0;
}

template<typename T>
inline void DFA<T>::reset()
{
	curState = initState;
}

template<typename T>
inline int DFA<T>::input(T _input)
{
	auto tr = make_pair(curState, _input);
	auto p = transitions.find(tr);
	if (p != transitions.end()) {
		return curState = p->second;
	}
	else {
		return curState = -1;
	}
}

template<typename T>
inline int DFA<T>::state()
{
	return curState;
}





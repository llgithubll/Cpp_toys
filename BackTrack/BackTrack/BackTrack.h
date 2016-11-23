#pragma once

template <typename T, typename I, typename V>
class BackTrack {
public:
	// Precondition:first <= last
	BackTrack(const T& first, const T& last);	

	// Finds the next solution to the problem
	// Repeated calls can find all solutions to the problem
	// Return true if a solution was found
	// Set first to true to get the first solution
	bool operator()(const I& begin, I end, bool& first);

private:
	// Finds the next valid sibling of the leaf(end-1) (candidates in the same level)
	// Return true is if a valid sibling was found.
	bool FindValidSibling(const I& begin, const I& end);

	// Return true if an unvisited node was found
	bool VisitNewNode(const I& begin, I& end);

	void CreateLeftLeaf(I& end);

	T rightChild;
	T leftChild;
	V isValid;
};

template <typename T, typename I, typename V>
BackTrack<T, I, V>::BackTrack(const T& first, const T& last)
	:leftChild(first), rightChild(last)
{
}

template <typename T, typename I, typename V>
bool BackTrack<T, I, V>::VisitNewNode(const I& begin, I& end) {
	// ALGORITHM:
	// If the current node is the rightmost child we must backtrack one level
	// because there are no more children at this level.
	// So we back up until we find a non-rightmost child,
	// then generate the child to the right.
	// If we back up to the top without finding an unvisted child,
	// then all nodes have been generated.

	// Back up as long as the node is the rightmost child of its parent
	while (end - begin > 0 && *(end - 1) == rightChild) {
		--end;	// Go to previous level
	}

	I back = end - 1;
	if (end - begin > 0 && *back != rightChild) {
		++(*back);	// Go to next sibling
		return true;
	}
	else {
		return false;
	}
}

template <typename T, typename I, typename V>
bool BackTrack<T, I, V>::FindValidSibling(const I& begin, const I& end) {
	// Note:
	// The leaf entry to this routine has not yet been used or tested for validity,
	// so the leaf is only to "find" sibling and considered a "sibling" of itself.

	I back = end - 1;
	while (true) {
		if (isValid(begin, end)) {	// From root to this leaf, the condition is valid
			return true;
		}
		else if (*back != rightChild) { // ++(*back) == ++(*(end-1))
			++(*back);					// ++ means to get another state in level 'back' of the tree
		}
		else {
			return false;
		}
	}
}

template <typename T, typename I, typename V>
inline void BackTrack<T, I, V>::CreateLeftLeaf(I& end) {
	*(end++) = leftChild;
}

template <typename T, typename I, typename V>
bool BackTrack<T, I, V>::operator()(const I& begin, I end, bool& first) {
	const int size = end - begin;

	// If first time, need to create a root.
	// Otherwise need to visit new node since vector contains the last solution.
	// So, from 'begin' to 'end' is a solution.
	if (first) {
		first = false;
		end = begin;
		CreateLeftLeaf(end);
	}
	else if (!VisitNewNode(begin, end)) {
		return false;
	}

	while (true) {
		if (FindValidSibling(begin, end)) {
			if (end - begin < size) {
				CreateLeftLeaf(end);
			}
			else {
				return true;
			}
		}
		else if (!VisitNewNode(begin, end)) {
			return false;	// The tree has been exhausted, so no solution exists.
		}
	}
}
#include <iostream>
#include <string>
#include <fstream>
#include "BipartiteGraph.h"

using namespace std;

int main() {
	{
		string fileName;
		cin >> fileName;
		ifstream input(fileName + ".txt");
		if (input) {
			BipartiteGraph BG(input);
			cout << BG.toString() << endl;
			cout << (BG.isBipartiteGraph() ? "Is " : "Is not ") << "bipartite graph" << endl;
			if (BG.isBipartiteGraph()) {
				cout << "Max matching: " << BG.maxMatchingCount() << endl;
				cout << BG.maxMatchingEdge() << endl;
			}
		}
		else {
			cerr << "No such file" << endl;
		}
	}
	system("pause");
	return 0;
}
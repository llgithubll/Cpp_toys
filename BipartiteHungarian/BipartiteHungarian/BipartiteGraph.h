#pragma once
#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include <cstring>

using std::vector;
using std::string;
using std::istream;
using std::to_string;

class BipartiteGraph {
private:
	int V = 0;
	int E = 0;
	bool isBipartite = true;
	int maxMatching = 0;
	
	vector<int> *adj = nullptr;
	int *link = nullptr;
	bool *marked = nullptr;
	bool *color = nullptr;
	
	// identify the bipartite
	void dfs(int v) {
		marked[v] = true;
		for (int w : adj[v]) {
			if (!marked[w]) {
				color[w] = !color[v];
				dfs(w);
			}
			else if (color[w] == color[v]) {
				isBipartite = false;
			}
		}
	}

	// find augmenting
	bool find(int v) {
		for (int w : adj[v]) {
			if (!marked[w]) {
				int to = link[w];
				link[w] = v;
				marked[w] = true;
				if (to == -1 || find(to)) {
					return true;
				}
				link[w] = to;
			}
		}
		return false;
	}
	// calculate the max matching
	int hungarian() {
		int total = 0;
		for (int i = 0; i < V; i++) {
			link[i] = -1;
		}
		for (int v = 0; v < V; v++) {
			if (color[v]) { // one side
				for (int i = 0; i < V; i++) {
					marked[i] = false;
				}
				if (find(v)) {
					total++;
				}
			}
		}
		return total;
	}
public:
	////////////////////////// constructor //////////////////////
	BipartiteGraph(int _V)
		:V(_V) {
		E = 0;
		adj = new vector<int>[V];
		link = new int[V];
		marked = new bool[V];
		color = new bool[V];
		for (int i = 0; i < V; i++) {
			link[i] = -1;
			marked[i] = false;
			color[i] = false;
		}
	}
	BipartiteGraph(istream &is) {
		int v, e;
		is >> v >> e;
		assert(v >= 0 && e >= 0);
		V = v;
		E = 0;
		adj = new vector<int>[V];
		link = new int[V];
		marked = new bool[V];
		color = new bool[V];
		for (int i = 0; i < V; i++) {
			link[i] = -1;
			marked[i] = false;
			color[i] = false;
		}
		
		for (int i = 0; i < e; i++) {
			int v1, v2;
			is >> v1 >> v2;
			addEdge(v1, v2);
		}
		assert(e == E);

		for (int v = 0; v < V; v++) {
			if (!marked[v]) {
				dfs(v);
			}
		}
		if (isBipartite) {
			maxMatching = hungarian();
		}
	}

	~BipartiteGraph() {
		delete[] adj;
		delete[] color;
		delete[] link;
		delete[] marked;
	}
	/////////////////////// facility /////////////////////////////////////
	void addEdge(int v, int w) {
		assert(v >= 0 && v < V && w >= 0 && w < V);
		E++;
		adj[v].push_back(w);
		adj[w].push_back(v);
	}

	/////////////////////// get ///////////////////////////////////
	int verticesCount() {
		return V;
	}
	int edgesCount() {
		return E;
	}
	vector<int>& adjacent(int v) {
		assert(0 <= v && v <= V);
		return adj[v];
	}
	bool isBipartiteGraph() {
		return isBipartite;
	}
	int maxMatchingCount() {
		return maxMatching;
	}
	string maxMatchingEdge() {
		string s = "";
		for (int v = 0; v < V; v++) {
			if (link[v] != -1) {
				s += to_string(v);
				s += "-";
				s += to_string(link[v]);
				s += "\n";
			}
		}
		return s;
	}
	string toString() { // return type string& is not allowed, because s is local variable
		string s = "";
		s += to_string(V);
		s += " vertices, ";
		s += to_string(E);
		s += " edges\n";
		for (int v = 0; v < V; v++) {
			s += to_string(v);
			s += ": ";
			for (int w : adj[v]) {
				s += to_string(w);
				s += " ";
			}
			s += "\n";
		}
		return s;
	}

};
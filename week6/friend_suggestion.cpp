#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <limits>
#include <utility>

using namespace std;
using Length = long long;
const Length INF = numeric_limits<Length>::max() / 4;
class BiDijkstra;

class Graph {
public:
	Graph(int vertex_count) {
		adj.assign(vertex_count, {});
		cost.assign(vertex_count, {});
	}
	vector<vector<int>> adj;
	vector<vector<int>> cost;
	friend class BiDijkstra;
private:
	vector<Length> distances;
	priority_queue<pair<Length, int>,
								 vector<pair<Length, int>>,
								 greater<pair<Length, int>>> priq;
};


class BiDijkstra {
public:
	BiDijkstra(int n, Graph norm, Graph reverse)
		: V(n), normal_(move(norm)), reversed_(move(reverse)) {
		visited_.assign(n, false);
		workset_.reserve(n);
	}

	Length Distance(int s, int t) {
		clear();

		return -1;
	}

	void clear() {
		for (int v : workset_) {
			normal_.distances[v] = reversed_.distances[v] = INF;
			visited_[v] = false;
		}
		workset_.clear();
	}

private:
	const int V;
	Graph normal_;
	Graph reversed_;
	vector<int> workset_;
	vector<bool> visited_;
};

int main() {
	int n, m;
	cin >> n >> m;
	Graph normal(n);
	Graph reversed(n);

	for (int i = 0; i < m; ++i) {
		int u, v, c;
		cin >> u >> v >> c;
		normal.adj[u - 1].push_back(v - 1);
		normal.cost[u - 1].push_back(c);
		reversed.adj[v - 1].push_back(u - 1);
		reversed.cost[v - 1].push_back(c);
	}

	BiDijkstra bidij(n, move(normal), move(reversed));

	int t;
	cin >> t;
	for (int i = 0; i < t; ++i) {
		int u, v;
		cin >> u >> v;
		cout << bidij.Distance(u - 1, v - 1) << endl;
	}
}

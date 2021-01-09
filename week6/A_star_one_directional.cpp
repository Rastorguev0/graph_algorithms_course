#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <unordered_set>
#include <sstream>
#include <limits>
#include <cmath>

using namespace std;
using Length = long long;
using Queue = priority_queue<pair<Length, int>,
														 vector<pair<Length, int>>,
														 greater<pair<Length, int>>>;
const Length INF = numeric_limits<Length>::max() / 4;

struct Point {
	int x;
	int y;
};

class AStar {
public:
	AStar(size_t N, vector<vector<int>> adj_list, vector<vector<int>> cost_, vector<Point> coords_)
		: V(N), adj(move(adj_list)), cost(move(cost_)), coords(move(coords_)),
		used(vector<bool>(N, false)), dist(vector<Length>(N, INF)) {}

	Length Distance(int s, int t) {
		Clear();
		SRC = s; TRG = t;
		Queue priq;
		dist[s] = 0;
		worked.insert(s);
		priq.emplace(Priority(s), s);

		while (!priq.empty()) {
			int vertex = ExtractMin(priq);
			if (vertex == t) return dist[vertex];
			if (!used[vertex]) {
				for (size_t i = 0; i < adj[vertex].size(); ++i) {
					Relax(priq, vertex, adj[vertex][i], cost[vertex][i]);
				}
				used[vertex] = true;
			}
		}

		return -1;
	}

private:
	Length Euristic(int v) const {
		return pow(coords[v].x - coords[TRG].x, 2) + pow(coords[v].y - coords[TRG].y, 2);
	}

	Length Priority(int vertex) const {
		return dist[vertex] + Euristic(vertex);
	}

	void Relax(Queue& q, int src, int trg, int cost) {
		if (dist[trg] > dist[src] + cost) {
			worked.insert(trg);
			dist[trg] = dist[src] + cost;
			q.emplace(Priority(trg), trg);
		}
	}

	int ExtractMin(Queue& q) {
		int v = q.top().second;
		q.pop();
		return v;
	}

	void Clear() {
		for (int v : worked) {
			used[v] = false;
			dist[v] = INF;
		}
		worked.clear();
	}

private:
	int SRC, TRG;
	const size_t V;
	const vector<vector<int>> adj;
	const vector<vector<int>> cost;
	const vector<Point> coords;
  unordered_set<int> worked;
  vector<bool> used;
  vector<Length> dist;
};

void Test1() {
	stringstream in("2 1\n0 0\n0 1\n1 2 1\n4\n1 1\n2 2\n1 2\n2 1\n");
	int n, m;
	in >> n >> m;
	vector<Point> vertexes(n);
	for (int i = 0; i < n; ++i) {
		in >> vertexes[i].x >> vertexes[i].y;
	}
	vector<vector<int>> adj(n), cost(n);
	for (int i = 0; i < m; ++i) {
		int u, v, c;
		in >> u >> v >> c;
		adj[u - 1].push_back(v - 1);
		cost[u - 1].push_back(c);
	}

	AStar astar(n, move(adj), move(cost), move(vertexes));
	int t;
	in >> t;
	for (int i = 0; i < t; ++i) {
		int u, v;
		in >> u >> v;
		cout << astar.Distance(u - 1, v - 1) << endl;
	}
}

void Test2() {
	stringstream in("4 4\n0 0\n0 1\n2 1\n2 0\n1 2 1\n4 1 2\n2 3 2\n1 3 6\n1\n1 3\n");
	int n, m;
	in >> n >> m;
	vector<Point> vertexes(n);
	for (int i = 0; i < n; ++i) {
		in >> vertexes[i].x >> vertexes[i].y;
	}
	vector<vector<int>> adj(n), cost(n);
	for (int i = 0; i < m; ++i) {
		int u, v, c;
		in >> u >> v >> c;
		adj[u - 1].push_back(v - 1);
		cost[u - 1].push_back(c);
	}

	AStar astar(n, move(adj), move(cost), move(vertexes));
	int t;
	in >> t;
	for (int i = 0; i < t; ++i) {
		int u, v;
		in >> u >> v;
		cout << astar.Distance(u - 1, v - 1) << endl;
	}
}

void Test() {
	stringstream in("5 10\n0 0\n10 10\n20 10\n20 5\n10 5\n1 2 0\n2 1 0\n2 3 80\n 3 2 80\n 3 4 30\n 4 3 30\n4 5 1\n5 4 1\n1 5 80\n5 1 80");
	int n, m;
	in >> n >> m;
	vector<Point> vertexes(n);
	for (int i = 0; i < n; ++i) {
		in >> vertexes[i].x >> vertexes[i].y;
	}
	vector<vector<int>> adj(n), cost(n);
	for (int i = 0; i < m; ++i) {
		int u, v, c;
		in >> u >> v >> c;
		adj[u - 1].push_back(v - 1);
		cost[u - 1].push_back(c);
	}

	AStar astar(n, move(adj), move(cost), move(vertexes));
	int t;
	cin >> t;
	for (int i = 0; i < t; ++i) {
		int u, v;
		cin >> u >> v;
		cout << astar.Distance(u - 1, v - 1) << endl;
	}
}

int main() {
	Test1();
	Test2();
	Test(); return 0;
	int n, m;
	cin >> n >> m;
	vector<Point> vertexes(n);
	for (int i = 0; i < n; ++i) {
		cin >> vertexes[i].x >> vertexes[i].y;
	}
	vector<vector<int>> adj(n), cost(n);
	for (int i = 0; i < m; ++i) {
		int u, v, c;
		cin >> u >> v >> c;
		adj[u - 1].push_back(v - 1);
		cost[u - 1].push_back(c);
	}

	AStar astar(n, move(adj), move(cost), move(vertexes));
	int t;
	cin >> t;
	for (int i = 0; i < t; ++i) {
		int u, v;
		cin >> u >> v;
		cout << astar.Distance(u - 1, v - 1) << endl;
	}
}

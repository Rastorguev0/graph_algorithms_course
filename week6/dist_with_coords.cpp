#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <unordered_set>
#include <limits>
#include <cmath>
#include <sstream>

using namespace std;
using Length = long long;
using Queue = priority_queue<pair<double, int>,
														 vector<pair<double, int>>,
														 greater<pair<double, int>>>;
const Length INF = numeric_limits<Length>::max() / 4;
class AStar;
enum class GStatus { NORMAL, REVERSED };

struct Point {
	int x;
	int y;
};

class Graph {
public:
	Graph(int vertex_count) :
		adj(vector<vector<int>>(vertex_count)),
		cost(vector<vector<int>>(vertex_count)),
		distances(vector<Length>(vertex_count, INF)),
		visited(vector<bool>(vertex_count, false))
	{}
	vector<vector<int>> adj;
	vector<vector<int>> cost;
	friend class AStar;
private:
	mutable vector<bool> visited;
	mutable vector<Length> distances;
	mutable Queue priq;
	mutable unordered_set<int> workset;
};


class AStar {
public:
	AStar(int n, Graph norm, Graph reverse, vector<Point> coords)
		: V(n), normal_(move(norm)), reversed_(move(reverse)), coords_(move(coords)) {}

	Length Distance(int s, int t) const {
		Prepare(s, t);
		while (!normal_.priq.empty() && !reversed_.priq.empty()) {
			const int vn = ExtractMin(GStatus::NORMAL);
			if (!normal_.visited[vn]) {
				Process(GStatus::NORMAL, vn);
				normal_.visited[vn] = true;
				if (reversed_.visited[vn]) return MakeDistance(vn);
			}

			const int vr = ExtractMin(GStatus::REVERSED);
			if (!reversed_.visited[vr]) {
				Process(GStatus::REVERSED, vr);
				reversed_.visited[vr] = true;
				if (normal_.visited[vr]) return MakeDistance(vr);
			}
		}
		Clear();
		return -1;
	}

private:
	double Heuristic(GStatus status, int v) const {
		const int t = status == GStatus::NORMAL ? TRG : SRC;
		return sqrt(pow(coords_[v].x - coords_[t].x, 2) + pow(coords_[v].y - coords_[t].y, 2));
	}

	double Priority(GStatus status, int v) const {
		const int minus = status == GStatus::NORMAL ? 1 : -1;
		const auto& dist = status == GStatus::NORMAL ? normal_.distances : reversed_.distances;
		return dist[v] + minus / 2 * (Heuristic(GStatus::NORMAL, v) - Heuristic(GStatus::REVERSED, v));
	}

	Length MakeDistance(int vertex) const {
		Length best_length = normal_.distances[vertex] + reversed_.distances[vertex];
		for (int u : normal_.workset) {
			if (normal_.distances[u] + reversed_.distances[u] < best_length)
				best_length = normal_.distances[u] + reversed_.distances[u];
			normal_.distances[u] = INF;
			normal_.visited[u] = false;
		}
		for (int u : reversed_.workset) {
			if (normal_.distances[u] + reversed_.distances[u] < best_length)
				best_length = normal_.distances[u] + reversed_.distances[u];
			reversed_.distances[u] = INF;
			reversed_.visited[u] = false;
		}
		normal_.workset.clear();
		reversed_.workset.clear();
		normal_.priq = {};
		reversed_.priq = {};
		return best_length;
	}

	void Process(GStatus status, int v) const {
		auto& graph = status == GStatus::NORMAL ? normal_ : reversed_;
		for (size_t i = 0; i < graph.adj[v].size(); ++i) {
			if (!graph.visited[graph.adj[v][i]]) {
				Relax(status, v, graph.adj[v][i], graph.cost[v][i]);
			}
		}
	}

	void Relax(GStatus status, int src, int trg, Length len) const {
		auto& graph = status == GStatus::NORMAL ? normal_ : reversed_;
		if (graph.distances[trg] > graph.distances[src] + len) {
			graph.workset.insert(trg);
			graph.distances[trg] = graph.distances[src] + len;
			graph.priq.emplace(Priority(status, trg), trg);
		}
	}
	int ExtractMin(GStatus status) const {
		Queue& q = status == GStatus::NORMAL ? normal_.priq : reversed_.priq;
		int vertex = q.top().second;
		q.pop();
		return vertex;
	}

	void Prepare(int s, int t) const {
		normal_.distances[s] = 0;
		normal_.priq.emplace(Priority(GStatus::NORMAL, s), s);
		reversed_.distances[t] = 0;
		reversed_.priq.emplace(Priority(GStatus::REVERSED, t), t);
		normal_.workset.insert(s);
		reversed_.workset.insert(t);
		SRC = s;
		TRG = t;
	}

	void Clear() const {
		for (int v : normal_.workset) {
			normal_.distances[v] = reversed_.distances[v] = INF;
			normal_.visited[v] = false;
		}
		for (int v : reversed_.workset) {
			normal_.distances[v] = reversed_.distances[v] = INF;
			reversed_.visited[v] = false;
		}
		normal_.workset.clear();
		reversed_.workset.clear();
		normal_.priq = {};
		reversed_.priq = {};
	}

private:
	const int V;
	const Graph normal_;
	const Graph reversed_;
	const vector<Point> coords_;
	mutable int SRC = 0;
	mutable int TRG = 0;
};

void Test1() {
	stringstream in("2 1\n0 0\n0 1\n1 2 1\n4\n1 1\n2 2\n1 2\n2 1\n");
	int n, m;
	in >> n >> m;
	vector<Point> vertexes(n);
	for (int i = 0; i < n; ++i) {
		in >> vertexes[i].x >> vertexes[i].y;
	}
	Graph normal(n), reversed(n);
	for (int i = 0; i < m; ++i) {
		int u, v, c;
		in >> u >> v >> c;
		normal.adj[u - 1].push_back(v - 1);
		normal.cost[u - 1].push_back(c);
		reversed.adj[v - 1].push_back(u - 1);
		reversed.cost[v - 1].push_back(c);
	}

	AStar astar(n, move(normal), move(reversed), move(vertexes));
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
	Graph normal(n), reversed(n);
	for (int i = 0; i < m; ++i) {
		int u, v, c;
		in >> u >> v >> c;
		normal.adj[u - 1].push_back(v - 1);
		normal.cost[u - 1].push_back(c);
		reversed.adj[v - 1].push_back(u - 1);
		reversed.cost[v - 1].push_back(c);
	}

	AStar astar(n, move(normal), move(reversed), move(vertexes));
	int t;
	in >> t;
	for (int i = 0; i < t; ++i) {
		int u, v;
		in >> u >> v;
		cout << astar.Distance(u - 1, v - 1) << endl;
	}
}

void Test3() {
	stringstream in("5 10\n0 0\n10 10\n20 10\n20 5\n10 5\n1 2 0\n2 1 0\n2 3 80\n 3 2 80\n 3 4 30\n 4 3 30\n4 5 1\n5 4 1\n1 5 80\n5 1 80\n1\n1 4");
	int n, m;
	in >> n >> m;
	vector<Point> vertexes(n);
	for (int i = 0; i < n; ++i) {
		in >> vertexes[i].x >> vertexes[i].y;
	}
	Graph normal(n), reversed(n);
	for (int i = 0; i < m; ++i) {
		int u, v, c;
		in >> u >> v >> c;
		normal.adj[u - 1].push_back(v - 1);
		normal.cost[u - 1].push_back(c);
		reversed.adj[v - 1].push_back(u - 1);
		reversed.cost[v - 1].push_back(c);
	}

	AStar astar(n, move(normal), move(reversed), move(vertexes));	int t;
	in >> t;
	for (int i = 0; i < t; ++i) {
		int u, v;
		in >> u >> v;
		cout << astar.Distance(u - 1, v - 1) << endl;
	}
}


int main() {
	/*
	Test1();
	Test2();
	Test3();
	return 0;
	*/
	int n, m;
	cin >> n >> m;
	vector<Point> vertexes(n);
	for (int i = 0; i < n; ++i) {
		cin >> vertexes[i].x >> vertexes[i].y;
	}
	Graph normal(n), reversed(n);
	for (int i = 0; i < m; ++i) {
		int u, v, c;
		cin >> u >> v >> c;
		normal.adj[u - 1].push_back(v - 1);
		normal.cost[u - 1].push_back(c);
		reversed.adj[v - 1].push_back(u - 1);
		reversed.cost[v - 1].push_back(c);
	}

	AStar astar(n, move(normal), move(reversed), move(vertexes));
	int t;
	cin >> t;
	for (int i = 0; i < t; ++i) {
		int u, v;
		cin >> u >> v;
		cout << astar.Distance(u - 1, v - 1) << endl;
	}
}

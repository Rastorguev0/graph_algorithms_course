#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <unordered_set>
#include <limits>
#include <cmath>
#include <random>
#include "profile.h"

using namespace std;
using Length = long long;
using Queue = priority_queue<pair<Length, int>,
														 vector<pair<Length, int>>,
														 greater<pair<Length, int>>>;
const Length INF = numeric_limits<Length>::max() / 4;
class AStar;
enum class GStatus { NORMAL, REVERSED };

struct Point {
	int x;
	int y;
};

Length EuristicDist(Point from, Point to) {
	return pow((to.x - from.x), 2) + pow((to.y - from.y), 2);
}

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
		Clear();
		Prepare(s, t);
		while (!normal_.priq.empty() && !reversed_.priq.empty()) {
			int vn = ExtractMin(GStatus::NORMAL);
			if (!normal_.visited[vn]) {
				Process(GStatus::NORMAL, vn);
				normal_.visited[vn] = true;
				if (reversed_.visited[vn]) return ShortestDistance(vn);
			}

			int vr = ExtractMin(GStatus::REVERSED);
			if (!reversed_.visited[vr]) {
				Process(GStatus::REVERSED, vr);
				reversed_.visited[vr] = true;
				if (normal_.visited[vr]) return ShortestDistance(vr);
			}
		}
		return -1;
	}

private:
	Length ShortestDistance(int vertex) const {
		Length best_length = normal_.distances[vertex] + reversed_.distances[vertex];
		for (int u : normal_.workset) {
			if (normal_.distances[u] + reversed_.distances[u] < best_length)
				best_length = normal_.distances[u] + reversed_.distances[u];
		}
		for (int u : reversed_.workset) {
			if (normal_.distances[u] + reversed_.distances[u] < best_length)
				best_length = normal_.distances[u] + reversed_.distances[u];
		}
		return best_length;
	}

	void Process(GStatus status, int v) const {
		auto& graph = status == GStatus::NORMAL ? normal_ : reversed_;
		int goal = status == GStatus::NORMAL ? TRG : SRC;
		for (size_t i = 0; i < graph.adj[v].size(); ++i) {
			if (!graph.visited[graph.adj[v][i]]) {
				Relax(status, v, graph.adj[v][i], graph.cost[v][i], goal);
			}
		}
	}

	void Relax(GStatus status, int src, int trg, Length len, int goal) const {
		auto& graph = status == GStatus::NORMAL ? normal_ : reversed_;
		if (graph.distances[trg] > graph.distances[src] + len) {
			graph.workset.insert(trg);
			graph.distances[trg] = graph.distances[src] + len;
			graph.priq.emplace(graph.distances[trg], trg);
		}
	}

	int ExtractMin(GStatus status) const {
		auto& q = status == GStatus::NORMAL ? normal_.priq: reversed_.priq;
		int vertex = q.top().second;
		q.pop();
		return vertex;
	}

	void Prepare(int s, int t) const {
		SRC = s; TRG = t;
		Length dist = EuristicDist(coords_[s], coords_[t]);
		normal_.distances[s] = dist;
		normal_.priq.emplace(dist, s);
		reversed_.distances[t] = dist;
		reversed_.priq.emplace(dist, t);
		normal_.workset.insert(s);
		reversed_.workset.insert(t);
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
		SRC = TRG = 0;
	}

private:
	const int V;
	const Graph normal_;
	const Graph reversed_;
	const vector<Point> coords_;
	mutable int SRC = 0;
	mutable int TRG = 0;
};

int main() {
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

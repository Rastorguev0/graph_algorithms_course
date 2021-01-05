#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <utility>
#include <unordered_set>
#include <limits>
#include <random>
#include "profile.h"

using namespace std;
using Length = long long;
using Queue = priority_queue<pair<Length, int>,
														 vector<pair<Length, int>>,
														 greater<pair<Length, int>>>;
const Length INF = numeric_limits<Length>::max() / 4;
class BiDijkstra;
enum class GStatus { NORMAL, REVERSED };

class Graph {
public:
	Graph(int vertex_count) :
		adj(vector<vector<int>>(vertex_count)),
		cost(vector<vector<int>>(vertex_count)),
		distances(vector<Length>(vertex_count, INF))
	{}
	vector<vector<int>> adj;
	vector<vector<int>> cost;
	friend class BiDijkstra;
private:
	mutable vector<Length> distances;
	mutable Queue priq;
	mutable unordered_set<int> workset;
};


class BiDijkstra {
public:
	BiDijkstra(int n, Graph norm, Graph reverse)
		: V(n), normal_(move(norm)), reversed_(move(reverse)) {
		visited_.assign(n, false);
	}

	Length Distance(int s, int t) const {
		Clear();
		Prepare(s, t);
		while (!normal_.priq.empty() && !reversed_.priq.empty()) {
			int vn = ExtractMin(GStatus::NORMAL);
			if (!visited_[vn]) {
				Process(GStatus::NORMAL, vn);
				visited_[vn] = true;
				if (reversed_.workset.count(vn)) return ShortestDistance(vn);
			}

			int vr = ExtractMin(GStatus::REVERSED);
			if (!visited_[vr]) {
				Process(GStatus::REVERSED, vr);
				visited_[vr] = true;
				if (normal_.workset.count(vr)) return ShortestDistance(vr);
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
		for (size_t i = 0; i < graph.adj[v].size(); ++i) {
			if (!visited_[graph.adj[v][i]]) {
				graph.workset.insert(graph.adj[v][i]);
				Relax(status, v, graph.adj[v][i], graph.cost[v][i]);
			}
		}
	}

	void Relax(GStatus status, int src, int trg, Length len) const {
		auto& graph = status == GStatus::NORMAL ? normal_ : reversed_;
		if (graph.distances[trg] > graph.distances[src] + len) {
			graph.distances[trg] = graph.distances[src] + len;
			graph.priq.emplace(graph.distances[trg], trg);
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
		normal_.priq.emplace(0, s);
		reversed_.distances[t] = 0;
		reversed_.priq.emplace(0, t);
		normal_.workset.insert(s);
		reversed_.workset.insert(t);
	}

	void Clear() const {
		for (int v : normal_.workset) {
			normal_.distances[v] = reversed_.distances[v] = INF;
			visited_[v] = false;
		}
		normal_.workset.clear();
		for (int v : reversed_.workset) {
			normal_.distances[v] = reversed_.distances[v] = INF;
			visited_[v] = false;
		}
		reversed_.workset.clear();
		clear(normal_.priq);
		clear(reversed_.priq);
	}

	void clear(Queue& q) const {
		while (!q.empty()) q.pop();
	}

private:
	const int V;
	const Graph normal_;
	const Graph reversed_;
	mutable vector<bool> visited_;
};

void SpeedTest() {
	LOG_DURATION("TOTAL");
	auto rnd = default_random_engine();
	const int V = 1'000'000;
	const int W = 1'000;
	const int Q = 1'000;
	const int min_friends = 6;
	const int max_friends = 7;
	Graph normal(V);
	Graph reversed(V);
	{
		LOG_DURATION("FILLING");
		for (unsigned i = 0; i < V; ++i) {
			for (unsigned j = 0; j < (rnd() % (max_friends - min_friends) + min_friends); ++j) {
				int to = rnd() % V;
				int w = rnd() % W;
				if (i == to) continue;
				normal.adj[i].push_back(to);
				normal.cost[i].push_back(w);
				reversed.adj[to].push_back(i);
				reversed.cost[to].push_back(w);
			}
		}
	}
	{
		LOG_DURATION("BI SEARCHING");
		BiDijkstra bidij(V, move(normal), move(reversed));
		Length total = 0;
		for (int i = 0; i < Q; ++i) {
			int from = rnd() % V;
			int to = rnd() % V;
			total += bidij.Distance(from, to);
		}
		cout << total << endl;
	}
}

int main() {
	SpeedTest(); return 0;
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
	return 0;
}

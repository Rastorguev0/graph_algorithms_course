#include <iostream>
#include <limits>
#include <vector>
#include <queue>

using namespace std;

template<class T>
class length {
public:
	length() : inf(numeric_limits<T>::max()) {
		value_ = inf;
	}
	length(const T& val) : length() {
		value_ = val;
	}
	bool finite() const {
		return value_ != inf;
	}
	bool infinite() const {
		return value_ == inf;
	}
	length& operator = (const length& other) {
		value_ = other.value_;
		return *this;
	}
	length& operator = (const T& val) {
		value_ = val;
		return *this;
	}
	bool operator > (const T& val) const {
		return infinite() || value_ > val;
	}
	bool operator > (const length& other) const {
		return other.finite() && value_ > other.value_;
	}
	length operator + (const T& val) {
		length result;
		if (finite()) {
			result.value_ = value_ + val;
		}
		return result;
	}
	length operator + (const length& other) {
		length result;
		if (finite() && other.finite()) result.value_ = value_ + other.value_;
		return result;
	}

	T value() const {
		return value_;
	}

private:
	const T inf;
	T value_;
};

struct edge {
	int from;
	int to;
	int cost;
};

struct Distances {
	Distances(int vertex_count) {
		dist.assign(vertex_count, {});
		indicators.assign(vertex_count, '+');
	}
	vector<length<long long>> dist;
	//'+' - have path, '*' - no way, '-' - negative cycle
	vector<char> indicators;
};

class Graph {
public:
	using list = vector<vector<int>>;
	Graph(list adj_list, vector<edge> e)
		: adj(move(adj_list)), edges(move(e)), V(adj.size()) {}

	int BellmanFord(int from, vector<length<long long>>& dist, vector<int>& prev) const {
		dist[from] = 0;
		bool changed = true;
		int last_changed = -1;
		for (int i = 0; i < V && changed; ++i) {
			changed = false;
			for (const auto& edge : edges) {
				if (dist[edge.to] > dist[edge.from] + edge.cost) {
					dist[edge.to] = dist[edge.from] + edge.cost;
					prev[edge.to] = edge.from;
					changed = true;
					last_changed = edge.to;
				}
			}
		}
		return changed ? last_changed : -1;
	}

	Distances ShortestPaths(int from) const {
		Distances result(V);
		vector<int> prev(V, -1);
		int last = BellmanFord(from, result.dist, prev);
		if (last >= 0) {
			for (int i = 0; i < V; ++i) {
				last = prev[last];
			}
			// last теперь - какая-то из вершин графа!
			// запущу DFS чтобы узнать все достигаемые вершины из этого цикла
			DFS(last, result.indicators);
		}
		for (int v = 0; v < V; ++v) {
			if (result.dist[v].infinite()) result.indicators[v] = '*';
		}
		return move(result);
	}

	void DFS(int v, vector<char>& ind) const {
		ind[v] = '-';
		for (int neighbour : adj[v]) {
			if (ind[neighbour] != '-') DFS(neighbour, ind);
		}
	}

private:
	const list adj;
	const vector<edge> edges;
	const int V;
};

int main() {
	int n, m, s;
	cin >> n >> m;
	vector<vector<int> > adj(n);
	vector<edge> edges(m);
	for (int i = 0; i < m; i++) {
		int x, y, w;
		cin >> x >> y >> w;
		adj[x - 1].push_back(y - 1);
		edges[i] = { x - 1, y - 1, w };
	}

	Graph G(move(adj), move(edges));
	cin >> s;
	s--;

	auto dists = G.ShortestPaths(s);
	for (int v = 0; v < dists.dist.size(); ++v) {
		if (dists.indicators[v] == '+') {
			cout << dists.dist[v].value() << endl;
		}
		else if (dists.indicators[v] == '-') {
			cout << '-' << endl;
		}
		else if (dists.indicators[v] == '*') {
			cout << '*' << endl;
		}
	}
	return 0;
}

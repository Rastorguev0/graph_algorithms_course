#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
using namespace std;

struct nullopt_t {
	explicit nullopt_t() {}
};
const nullopt_t nullopt{};

template<class T>
class optional {
public:
	optional() {}
	optional(nullopt_t null) {}
	optional(const optional& other) {
		*this = other;
	}

	optional(optional&& other) {
		*this = std::move(other);
	}

	optional& operator = (const optional& other) {
		if (initialized = other.initialized) {
			value_ = other.value_;
		}
		return *this;
	}

	optional& operator = (optional&& other) {
		if (initialized = other.initialized) {
			value_ = std::move(other.value_);
			other.initialized = false;
		}
		return *this;
	}

	T& operator = (const T& val) {
		initialized = true;
		return value_ = val;
	}
	T& operator = (T&& val) {
		initialized = true;
		return value_ = std::move(val);
	}

	operator bool() const {
		return initialized;
	}

	bool has_value() const {
		return initialized;
	}

	const T& value() const {
		return value_;
	}

	T& value() {
		return value_;
	}

private:
	T value_;
	bool initialized = false;
};

class Graph {
private:
	template<typename Comp>
	using p_queue = priority_queue<int, vector<int>, Comp>;
public:
	Graph(vector<vector<int>> adj_list, vector<vector<int>> w)
		: adj(move(adj_list)), weights(move(w)) {
		colors.assign(adj.size(), Color::WHITE);
	}

	int Distance(int from, int to) const {
		vector<optional<int>> dist(adj.size());
		auto cmp = [&dist](int lhs, int rhs) {
			return dist[lhs].value() > dist[rhs].value();
		};
		p_queue<decltype(cmp)> q(cmp);
		dist[from] = 0;
		q.push(from);

		RunDijkstra(q, dist);
		if (dist[to]) return dist[to].value();
		return -1;
	}

private:
	template<typename Comp>
	int ExtractMin(p_queue<Comp>& q) const {
		int v = q.top();
		q.pop();
		colors[v] = Color::BLACK;
		return v;
	}

	template<typename Comp>
	void RunDijkstra(p_queue<Comp>& q, vector<optional<int>>& dist) const {
		while (!q.empty()) {
			int u = ExtractMin(q);
			for (int i = 0; i < adj[u].size(); i++) {
				int v = adj[u][i];
				if (colors[v] != Color::BLACK) {
					if (!dist[v] || dist[v].value() > dist[u].value() + weights[u][i]) {
						dist[v] = dist[u].value() + weights[u][i];
						q.push(v);
					}
				}
			}
		}
	}

  enum class Color { WHITE, BLACK };
private:
	const vector<vector<int>> adj;
	const vector<vector<int>> weights;
	mutable vector<Color> colors;
};


int main() {
  int n, m;
  std::cin >> n >> m;
  vector<vector<int>> adj(n);
	vector<vector<int>> weights(n);
  for (int i = 0; i < m; i++) {
    int x, y, w;
    cin >> x >> y >> w;
		assert(w >= 0);
    adj[x - 1].push_back(y - 1);
		weights[x - 1].push_back(w);
  }

  Graph G(move(adj), move(weights));
  int s, t;
  cin >> s >> t;
  s--, t--;
  cout << G.Distance(s, t);
}
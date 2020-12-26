#include <iostream>
#include <vector>
#include <queue>
#include <utility>

using std::vector;
using std::queue;

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
public:
	Graph(vector<vector<int>> adj_list) {
		adj = move(adj_list);
		distances.assign(adj.size(), optional<int>());
		state.assign(adj.size(), Color::WHITE);
	}

	void Push(int vertex) {
		q.push(vertex);
		state[vertex] = Color::GRAY;
	}

	int Pop() {
		int vertex = q.front();
		q.pop();
		state[vertex] = Color::BLACK;
		return vertex;
	}

	int Dist(int from, int to) {
		distances[from] = 0;
		Push(from);
		while (!q.empty()) {
			int cur = Pop();
			for (int neighbour : adj[cur]) {
				if (state[neighbour] == Color::WHITE) {
					Push(neighbour);
					distances[neighbour] = distances[cur].value() + 1;
				}
			}
		}
		return distances[to] ? distances[to].value() : -1;
	}

private:
	enum class Color { WHITE, GRAY, BLACK };
private:
	vector<vector<int>> adj;
	vector<optional<int>> distances;
	vector<Color> state;
	queue<int> q;
};

int main() {
  int n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
    adj[y - 1].push_back(x - 1);
  }
	Graph G(move(adj));
  int s, t;
  std::cin >> s >> t;
  s--, t--;
  std::cout << G.Dist(s, t);
}

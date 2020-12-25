#include <iostream>
#include <vector>

using std::vector;
using std::pair;

class graph {
public:
	graph(vector<vector<int>> adj) : adj_(std::move(adj)) {
		visited.assign(adj_.size(), false);
	}

	int reach(int from, int to) const {
		visited[from] = true;
		for (int neighbour : adj_[from]) {
			if (neighbour == to) return 1;
			if (!visited[neighbour])
				if (reach(neighbour, to) == 1) return 1;
		}
		return 0;
	}

private:
	vector<vector<int>> adj_;
	mutable vector<bool> visited;
};

int main() {
	size_t n, m;
	std::cin >> n >> m;
	vector<vector<int>> adj(n, vector<int>());
	for (size_t i = 0; i < m; i++) {
		int x, y;
		std::cin >> x >> y;
		adj[x - 1].push_back(y - 1);
		adj[y - 1].push_back(x - 1);
	}

	int x, y;
	std::cin >> x >> y;
	graph g(move(adj));
	std::cout << g.reach(x - 1, y - 1);
}

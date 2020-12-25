#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::pair;

class graph {
public:
	graph(vector<vector<int>> adj) : adj_(std::move(adj)) {
		visited.assign(adj_.size(), false);
	}

	int number_of_components() {
		int res = 0;
		for (auto it = std::find(visited.begin(), visited.end(), false);
			it != visited.end();
			it = std::find(visited.begin(), visited.end(), false)) {
			DFS(std::distance(visited.begin(), it));
			res++;
		}
		return res;
	}

	void DFS(int vertex_id) const {
		visited[vertex_id] = true;
		for (int neighbour : adj_[vertex_id]) {
			if (!visited[neighbour]) DFS(neighbour);
		}
	}

private:
	vector<vector<int>> adj_;
	mutable vector<bool> visited;
};

int main() {
	size_t n, m;
	std::cin >> n >> m;
	vector<vector<int> > adj(n, vector<int>());
	for (size_t i = 0; i < m; i++) {
		int x, y;
		std::cin >> x >> y;
		adj[x - 1].push_back(y - 1);
		adj[y - 1].push_back(x - 1);
	}
	graph g(move(adj));
	std::cout << g.number_of_components();
}

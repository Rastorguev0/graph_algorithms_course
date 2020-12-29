#include <iostream>
#include <vector>
#include <limits>
using std::vector;

const int INF = std::numeric_limits<int>::max() / 2;

class Graph {
public:
	Graph(vector<vector<int>> adj_list, vector<vector<int>> c)
		: adj(move(adj_list)), costs(move(c)), V(adj.size()) {}

	bool NegCycle() const {
		vector<int> dist(V, INF);
		dist[0] = 0;
		bool changed = true;
		for (int i = 0; i < V && changed; ++i) {
			changed = false;
			//now exploring all edges; EDGE = source -> dest
			for (int source = 0; source < V; ++source) {
				for (int d = 0; d < adj[source].size(); ++d) {
					int dest = adj[source][d];
					if (dist[dest] > dist[source] + costs[source][d]) {
						dist[dest] = dist[source] + costs[source][d];
						changed = true;
					}
				}
			}
		}
		return changed;
	}

private:
	const vector<vector<int>> adj;
	const vector<vector<int>> costs;
	const int V;
};

bool negative_cycle(vector<vector<int>>& adj, vector<vector<int>>& cost) {
	Graph G(move(adj), move(cost));
	return G.NegCycle();
}

int main() {
	int n, m;
	std::cin >> n >> m;
	vector<vector<int>> adj(n, vector<int>());
	vector<vector<int>> cost(n, vector<int>());
	for (int i = 0; i < m; i++) {
		int x, y, w;
		std::cin >> x >> y >> w;
		adj[x - 1].push_back(y - 1);
		cost[x - 1].push_back(w);
	}
	std::cout << negative_cycle(adj, cost);
}

#include <iostream>
#include <vector>
#include <queue>

using std::vector;
using std::queue;

class Graph {
private:
	enum class Color { WHITE, GRAY, BLACK };
public:
	Graph(vector<vector<int>> adj_list) {
		adj = move(adj_list);
		state.assign(adj.size(), Color::WHITE);
	}

	Color AltColor(int vertex) {
		if (state[vertex] == Color::GRAY) return Color::BLACK;
		else if (state[vertex] == Color::BLACK) return Color::GRAY;
	}

	void Push(queue<int>& q, int vertex, Color c) {
		q.push(vertex);
		state[vertex] = c;
	}

	int Pop(queue<int>& q) {
		int vertex = q.front();
		q.pop();
		return vertex;
	}

	bool BFS(queue<int>& q) {
		while (!q.empty()) {
			int v = Pop(q);
			for (int neighbour : adj[v]) {
				if (state[neighbour] == Color::WHITE) Push(q, neighbour, AltColor(v));
				else if (state[neighbour] != AltColor(v)) return false;
			}
		}
		return true;
	}

	bool Bipartite() { //starts with 1st vertex in the graph
		queue<int> q;
		for (int v = 0; v < adj.size(); v++) {
			if (state[v] == Color::WHITE) {
				Push(q, v, Color::GRAY);
				if (!BFS(q)) return false;
			}
		}
		return true;
	}

private:
	vector<vector<int>> adj;
	vector<Color> state;
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
	std::cout << G.Bipartite();
}

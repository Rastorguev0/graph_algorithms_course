#include <algorithm>
#include <iostream>
#include <vector>
#include <map>

using std::vector;
using std::map;
using std::pair;

class DiGraph {
public:
	DiGraph(vector<vector<int>> adj_list) {
		adj = move(adj_list);
		stats.assign(adj.size(), Stat());
		time = -1;
	}

	DiGraph Reverse() {
		vector<vector<int>> reverse_adj(adj.size());
		for (int from = 0; from < adj.size(); from++) {
			for (int to : adj[from]) {
				reverse_adj[to].push_back(from);
			}
		}
		return DiGraph(move(reverse_adj));
	}

	void DFS() {
		for (size_t i = 0; i < adj.size(); i++) {
			if (stats[i].color == Color::WHITE) ClockingExplore(i);
		}
	}

	void Explore(int v) {
		stats[v].color = Color::BLACK;
		for (int neighbour : adj[v]) {
			if (stats[neighbour].color == Color::WHITE) Explore(neighbour);
		}
	}

	int SCC_Count() {
		DiGraph GR = Reverse();
		GR.DFS();
		int result = 0;
		for (auto clock = GR.post_clock.rbegin(); clock != GR.post_clock.rend(); clock++) {
			int v = clock->second;
			if (stats[v].color == Color::WHITE) {
				Explore(v);
				result++;
			}
		}
		return result;
	}

private:
	enum class Color { WHITE, BLACK };
	struct Stat {
		Color color = Color::WHITE;
	};

private:
	void ClockPre(int vertex) {
		pre_clock[++time] = vertex;
	}

	void ClockPost(int vertex) {
		post_clock[++time] = vertex;
	}

	void ClockingExplore(int v) {
		stats[v].color = Color::BLACK;
		ClockPre(v);
		for (int neighbour : adj[v]) {
			if (stats[neighbour].color == Color::WHITE) ClockingExplore(neighbour);
		}
		ClockPost(v);
	}

private:
	vector<vector<int>> adj;
	vector<Stat> stats;
	int time;
	map<int, int> pre_clock; //pre_clock[clock] = vertex;
	map<int, int> post_clock; //post_clock[clock] = vertex;
};


int main() {
	size_t n, m;
	std::cin >> n >> m;
	vector<vector<int> > adj(n, vector<int>());
	for (size_t i = 0; i < m; i++) {
		int x, y;
		std::cin >> x >> y;
		adj[x - 1].push_back(y - 1);
	}
	DiGraph G(move(adj));
	std::cout << G.SCC_Count();
}

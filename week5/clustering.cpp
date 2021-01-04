#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <set>
#include <unordered_set>
#include <cmath>

using namespace std;

struct edge {
	int u;
	int v;
	double w;

	bool operator < (const edge& other) const {
		return w < other.w;
	}
};


void Merge(vector<int>& v_s, vector<unordered_set<int>>& s_v, int v1, int v2) {
	int set1 = v_s[v1];
	int set2 = v_s[v2];
	for (int vertex : s_v[set2]) {
		s_v[set1].insert(vertex);
		v_s[vertex] = set1;
	}
}

double LongestBridge(const multiset<edge>& edges, size_t v_count, int claster_count) {
	vector<int> vert_to_set(v_count);
	vector<unordered_set<int>> set_to_vert(v_count);
	for (int v = 0; v < v_count; ++v) {
		vert_to_set[v] = v;
		set_to_vert[v].insert(v);
	}
	int clasters_count_now = v_count;

	for (const auto& edge : edges) {
		if (vert_to_set[edge.u] != vert_to_set[edge.v]) {
			if (clasters_count_now == claster_count) return edge.w;
			clasters_count_now--;
			Merge(vert_to_set, set_to_vert, edge.u, edge.v);
		}
	}
	return 0.;
}

double clustering(const vector<int>& x, const vector<int>& y, int k) {
	multiset<edge> edges;
	for (int i = 0; i < x.size(); ++i) {
		for (int j = i - 1; j >= 0; --j) {
			edge e = { i, j, sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2)) };
			edges.insert(move(e));
		}
	}

	return LongestBridge(edges, x.size(), k);
}

int main() {
	size_t n;
	int k;
	std::cin >> n;
	vector<int> x(n), y(n);
	for (size_t i = 0; i < n; i++) {
		std::cin >> x[i] >> y[i];
	}
	std::cin >> k;
	std::cout << std::setprecision(10) << clustering(x, y, k) << std::endl;
}

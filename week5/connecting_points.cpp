#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <numeric>
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

double minimum_distance_by_prim(const vector<int>& x, const vector<int>& y) {
  double result = 0.;

  return result;
}

void Merge(vector<int>& v_s, vector<unordered_set<int>>& s_v, int v1, int v2) {
  int set1 = v_s[v1];
  int set2 = v_s[v2];
  for (int vertex : s_v[set2]) {
    s_v[set1].insert(vertex);
    v_s[vertex] = set1;
  }
}

void RunKrusk(const multiset<edge>& edges, size_t v_count, double& total_w) {
  vector<int> vert_to_set(v_count);
  vector<unordered_set<int>> set_to_vert(v_count);
  for (int v = 0; v < v_count; ++v) {
    vert_to_set[v] = v;
    set_to_vert[v].insert(v);
  }

  for (const auto& edge : edges) {
    if (vert_to_set[edge.u] != vert_to_set[edge.v]) {
      total_w += edge.w;
      Merge(vert_to_set, set_to_vert, edge.u, edge.v);
    }
  }
}

double minimum_distance_by_krusk(const vector<int>& x, const vector<int>& y) {
  double result = 0.;
  multiset<edge> edges;
  for (int i = 0; i < x.size(); ++i) {
    for (int j = i - 1; j >= 0; --j) {
      edge e = { i, j, sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2)) };
      edges.insert(move(e));
    }
  }

  RunKrusk(edges, x.size(), result);
  return result;
}

int main() {
  size_t n;
  cin >> n;
  vector<int> x(n), y(n);
  for (int i = 0; i < n; i++) {
    cin >> x[i] >> y[i];
  }
  cout << setprecision(10) << minimum_distance_by_krusk(x, y) << endl;
}

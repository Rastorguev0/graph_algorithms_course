#include <algorithm>
#include <iostream>
#include <limits>
#include <iomanip>
#include <vector>
#include <set>
#include <numeric>
#include <unordered_set>
#include <cmath>
#include <random>
#include "profile.h"

using namespace std;

struct edge {
  int u;
  int v;
  double w;

  bool operator < (const edge& other) const {
    return w < other.w;
  }
};

const int INF = numeric_limits<double>::max() / 2;

int ExtractMin(const vector<double>& costs, vector<bool>& used) {
  auto it = min_element(costs.begin(), costs.end());
  int vertex = distance(costs.begin(), it);
  used[vertex] = true;
  return vertex;
}

double w(int x1, int y1, int x2, int y2) {
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double minimum_distance_by_prim(const vector<int>& x, const vector<int>& y) {
  double result = 0.;
  size_t V = x.size();
  vector<double> costs(V);
  vector<bool> used(V, false);
  size_t used_counter = 0;
  costs[0] = 0;
  while (used_counter != V) {
    int v = ExtractMin(costs, used);
    used_counter++;
    for (int n = 0; n < V; n++) {
      double cost = w(x[v], y[v], x[n], y[n]);
      if (!used[n] && costs[n] > cost) {
        costs[n] = cost;
        result += cost;
      }
    }
  }

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

void SpeedTest(int vertex_count) {
  auto rand = default_random_engine();

  vector<int> x(vertex_count), y(vertex_count);
  for (int i = 0; i < vertex_count; i++) {
    x[i] = rand() % 10'000;
    y[i] = rand() % 10'000;
  }
  {
    LOG_DURATION("KRUSKAL ALGORITHM");
    double result = minimum_distance_by_krusk(x, y);
  }
  {
    LOG_DURATION("PRIM ALGORITHM");
    double result = minimum_distance_by_prim(x, y);
  }
}

int main() {
  SpeedTest(1000);
  /*
  size_t n;
  cin >> n;
  vector<int> x(n), y(n);
  for (int i = 0; i < n; i++) {
    cin >> x[i] >> y[i];
  }
  cout << setprecision(10) << minimum_distance_by_krusk(x, y) << endl;
  */
}

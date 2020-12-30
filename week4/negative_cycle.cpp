#include <iostream>
#include <limits>
#include <vector>
#include <queue>

using namespace std;

template<class T>
class length {
public:
  length() : inf(numeric_limits<T>::max()) {
    value_ = inf;
  }
  length(const T& val) : length() {
    value_ = val;
  }
  bool finite() const {
    return value_ != inf;
  }
  bool infinite() const {
    return value_ == inf;
  }
  length& operator = (const length& other) {
    value_ = other.value_;
    return *this;
  }
  length& operator = (const T& val) {
    value_ = val;
    return *this;
  }
  bool operator > (const T& val) const {
    return infinite() || value_ > val;
  }
  bool operator > (const length& other) const {
    return other.finite() && value_ > other.value_;
  }
  length operator + (const T& val) {
    length result;
    if (finite()) {
      result.value_ = value_ + val;
    }
    return result;
  }
  length operator + (const length& other) {
    length result;
    if (finite() && other.finite()) result.value_ = value_ + other.value_;
    return result;
  }

  T value() const {
    return value_;
  }

private:
  const T inf;
  T value_;
};

struct edge {
  int from;
  int to;
  int cost;
};

class Graph {
public:
  using list = vector<vector<int>>;
  Graph(list adj_list, vector<edge> e)
    : adj(move(adj_list)), edges(move(e)), V(adj.size()) {}

  bool BellmanFord(int from, vector<length<long long>>& dist) const {
    dist[from] = 0;
    bool changed = true;
    for (int i = 0; i < V && changed; ++i) {
      changed = false;
      for (const auto& edge : edges) {
        if (dist[edge.to] > dist[edge.from] + edge.cost) {
          dist[edge.to] = dist[edge.from] + edge.cost;
          changed = true;
        }
      }
    }
    return changed;
  }

  bool NegCycle() const {
    //инициализируем расстояния 0, тк
    // 1)нам не важны сами расстояния
    // 2)даже если начальная вершина изолированна от
    //цикла отрицательного веса, расстояние до вершин в цикле
    //будет меняться
    vector<length<long long>> dist(V, 0);
    return BellmanFord(0, dist);
  }

private:
  const list adj;
  const vector<edge> edges;
  const int V;
};

void shortest_paths(vector<vector<int>>& adj, vector<vector<int>>& cost, int s, vector<long long>& distance, vector<int>& reachable, vector<int>& shortest) {
  //write your code here
}

int main() {
  int n, m, s;
  cin >> n >> m;
  vector<vector<int> > adj(n);
  vector<edge> edges(m);
  for (int i = 0; i < m; i++) {
    int x, y, w;
    cin >> x >> y >> w;
    adj[x - 1].push_back(y - 1);
    edges[i] = { x - 1, y - 1, w };
  }
  Graph G(move(adj), move(edges));
  cout << G.NegCycle();
}

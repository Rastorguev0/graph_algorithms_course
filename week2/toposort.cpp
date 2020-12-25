#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;
using std::pair;  

class DirectedGraph {
public:
  DirectedGraph(vector<vector<int>> adj_list) {
    adj = move(adj_list);
    used.assign(adj.size(), false); //V
  }

  void DFS(int v, vector<int>& order) {
    used[v] = true;
    for (int neighbour : adj[v]) {
      if (!used[neighbour]) DFS(neighbour, order);
    }
    order.push_back(v);
  }

  vector<int> Toposort() {
    vector<int> order;
    order.reserve(adj.size()); //V
    for (int v = 0; v < adj.size(); v++) {
      if (!used[v]) DFS(v, order);
    } //2V + E
    return order;
  }
private:
  int FindNotUsed() {
    auto it = std::find(used.begin(), used.end(), false);
    return it == used.end() ? -1 : std::distance(used.begin(), it);
  }
private:
  vector<vector<int>> adj;
  vector<bool> used;
};

int main() {
  size_t n, m;
  std::cin >> n >> m;
  vector<vector<int>> adj(n, vector<int>());
  for (size_t i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
  } //V + E

  DirectedGraph G(move(adj)); //V
  vector<int> order = G.Toposort(); //4V + E
  for (int i = order.size() - 1; i >= 0; --i) { //V
    std::cout << order[i] + 1 << " ";
  }
}

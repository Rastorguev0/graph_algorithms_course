#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::pair;

class DirectedGraph {
private:
  enum class Color {
    WHITE, GRAY, BLACK,
  };

  int FindWhite() const {
    auto it = std::find(v_colors.begin(), v_colors.end(), Color::WHITE);
    return it == v_colors.end() ? -1 : std::distance(v_colors.begin(), it);
  }
public:
  DirectedGraph(vector<vector<int>> adj_list) {
    adj = move(adj_list);
    v_colors.assign(adj.size(), Color::WHITE);
  }

  bool DFS(int v) const {
    v_colors[v] = Color::GRAY;
    for (int neighbour : adj[v]) {
      if (v_colors[neighbour] == Color::GRAY) return true;
      else if (v_colors[neighbour] == Color::WHITE) {
        if (DFS(neighbour)) return true;
      }
    }
    v_colors[v] = Color::BLACK;
    return false;
  }

  bool Acyclic() const {
    for (int v = FindWhite(); v >= 0; v = FindWhite()) {
      if (DFS(v)) return true;
    }
    return false;
  }
private:
  vector<vector<int>> adj;
  mutable vector<Color> v_colors;
};


int main() {
  size_t n, m;
  std::cin >> n >> m;
  vector<vector<int>> adj(n, vector<int>());
  for (size_t i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
  }

  DirectedGraph G(move(adj));
  std::cout << G.Acyclic();
}

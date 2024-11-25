#include <cmath>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

/*  Algorithm:
  1) Create an adjacency list, parents vector (stores 2^i parent),
    and depths vector
  2) Call Bfs function to fulfill parents and depths vectors
  3) Call GetDistance function to compute the final answer:
    - Call FindLCA to find the Least Common Ancestor
    - Do calculation with depths of u, v, and lca vertices
*/

// Preprocess for Binary Lifting
void PreprocessDataStructures(
    const std::vector<std::vector<int>>& adj_list,
    std::vector<std::vector<int>>& parent,
    std::vector<int>& depth
) {
  std::queue<int> que;
  // Initialize root
  depth[0] = 0;
  que.push(0);

  // Calculate depths and parents using bfs
  while (!que.empty()) {
    int cur_node = que.front();
    que.pop();

    for (int neighbor : adj_list[cur_node]) {
      if (depth[neighbor] == -1) {  // Not visited neighbor
        depth[neighbor] = depth[cur_node] + 1;
        parent[neighbor][0] = cur_node;  // Immediate parent to neighbor
        que.push(neighbor);
      }
    }
  }

  // Calculate 2^i-th parents for nodes
  uint log_number = parent[0].size() - 1;
  for (uint i = 0; i < adj_list.size(); ++i) {
    for (uint j = 1; j <= log_number; ++j) {
      if (parent[i][j - 1] != -1) {
        parent[i][j] = parent[parent[i][j - 1]][j - 1];
      }
    }
  }
}

int FindLCA(
    int vert_u,
    int vert_v,
    const std::vector<std::vector<int>>& parent,
    const std::vector<int>& depth
) {
  // Make 'u' vertex store the "deepest" node
  if (depth[vert_u] < depth[vert_v]) {
    std::swap(vert_u, vert_v);
  }

  // Lift vert_u up to the vert_v
  int log_number = static_cast<int>(parent[0].size() - 1);
  int depth_delta = depth[vert_u] - depth[vert_v];
  for (int i = 0; i <= log_number; ++i) {
    if (depth_delta & (1 << i)) {
      vert_u = parent[vert_u][i];
    }
  }

  // If vert_u and vert_v on the same node
  if (vert_u == vert_v) {
    return vert_u;
  }

  // Otherwise, they are on the same depth but not the same node
  // then find their lowest ancestor
  for (int i = log_number; i >= 0; --i) {
    if (parent[vert_u][i] != parent[vert_v][i]) {
      vert_u = parent[vert_u][i];
      vert_v = parent[vert_v][i];
    }
  }
  return parent[vert_u][0];
}

int GetDistance(
    int vert_u,
    int vert_v,
    const std::vector<std::vector<int>>& parent,
    const std::vector<int>& depth
) {
  // Find Lowest Common Ancestor
  int lca = FindLCA(vert_u, vert_v, parent, depth);
  // Return distance between 'u' and 'v' vertices
  return depth[vert_u] + depth[vert_v] - 2 * depth[lca];
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  // Read from console to fulfill the adjacency list
  int vertices_number = 0;
  std::cin >> vertices_number;

  std::vector<std::vector<int>> adj_list(vertices_number);
  for (int i = 0; i < vertices_number - 1; ++i) {
    int src = -1;
    int dst = -1;
    std::cin >> src >> dst;
    adj_list[src].push_back(dst);
    adj_list[dst].push_back(src);
  }

  // Calculate the logarithm
  int log_number = std::ceil(std::log2(vertices_number));
  // Initialize parents vector
  std::vector<std::vector<int>> parent(vertices_number, std::vector<int>(log_number + 1, -1));
  // Initialize depths vector
  std::vector<int> depth(vertices_number, -1);

  PreprocessDataStructures(adj_list, parent, depth);

  int queries_number = 0;
  std::cin >> queries_number;

  for (int i = 0; i < queries_number; ++i) {
    int vert_u = 0;
    int vert_v = 0;
    int value = 0;
    std::cin >> vert_u >> vert_v >> value;

    int distance = GetDistance(vert_u, vert_v, parent, depth);
    if (distance <= value) {
      std::cout << "Yes\n";
    } else {
      std::cout << "No\n";
    }
  }

  return 0;
}

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

constexpr int MaxBits = 31;  // Maximum bits for XOR operation (depends on max size of weight)

struct TrieNode {
  std::array<TrieNode*, 2> child{nullptr, nullptr};
};

class Trie {
public:
  Trie() : root_(new TrieNode()) {
  }

  void Insert(int xor_value) {
    TrieNode* node = root_;               // take root
    for (int i = MaxBits; i >= 0; --i) {  // move down across the trie and insert new nodes
      int bit = (xor_value >> i) & 1;     //  according to the xor value
      if (!node->child[bit]) {
        node->child[bit] = new TrieNode();
      }
      node = node->child[bit];
    }
  }

  int Query(int xor_value) {
    TrieNode* node = root_;  // start from root
    int max_xor_value = 0;
    for (int i = MaxBits; i >= 0; --i) {  // move down across the trie and find max xor value
      int bit = (xor_value >> i) & 1;
      int inv = 1 - bit;
      if (node->child[inv]) {       //  take the opposite because:
        max_xor_value |= (1 << i);  //  bit ^ ~bit = 1 when bit ^ bit = 0
        node = node->child[inv];
      } else {
        node = node->child[bit];
      }
    }
    return max_xor_value;
  }

  ~Trie() {
    CleanNode(root_);
    delete root_;
  }

private:
  void CleanNode(TrieNode* node) {
    TrieNode* left_child = node->child[0];
    TrieNode* right_child = node->child[1];

    if (left_child != nullptr) {
      CleanNode(left_child);
      delete left_child;
    }
    if (right_child != nullptr) {
      CleanNode(right_child);
      delete right_child;
    }
  }

  TrieNode* root_;
};

// Do dfs to get XOR values from f (root) to other nodes
void Dfs(
    int node,
    int parent,
    std::vector<std::vector<std::pair<int, int>>>& graph,
    std::vector<int>& xor_from_f
) {
  for (auto& edge : graph[node]) {
    int neighbor = edge.first;
    int weight = edge.second;
    if (neighbor != parent) {
      xor_from_f[neighbor] = xor_from_f[node] ^ weight;
      Dfs(neighbor, node, graph, xor_from_f);
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int nodes_number = 0;
  int flowers_node = 0;
  std::cin >> nodes_number >> flowers_node;
  --flowers_node;  // Convert to 0-based indexing

  constexpr int EstimatedAdjNodes = 4;
  std::vector<std::vector<std::pair<int, int>>> graph(nodes_number);
  for (int i = 0; i < nodes_number; ++i) {
    graph[i].reserve(EstimatedAdjNodes);
  }

  for (int i = 0; i < nodes_number - 1; ++i) {
    int src = 0;
    int dest = 0;
    int weight = 0;
    std::cin >> src >> dest >> weight;

    // Convert to 0-based indexing
    --src;
    --dest;

    graph[src].emplace_back(dest, weight);
    graph[dest].emplace_back(src, weight);
  }

  // Compute XOR values from node f to all other nodes
  std::vector<int> xor_from_f(nodes_number, 0);
  Dfs(flowers_node, -1, graph, xor_from_f);

  // Insert XOR values into the trie data structure, excluding f node
  Trie trie;
  for (int i = 0; i < nodes_number; ++i) {
    if (i != flowers_node) {
      trie.Insert(xor_from_f[i]);
    }
  }

  // Find maximum XOR between any two valid nodes
  int max_discount = 0;
  for (int i = 0; i < nodes_number; ++i) {
    if (i != flowers_node) {
      max_discount = std::max(max_discount, trie.Query(xor_from_f[i]));
    }
  }

  std::cout << max_discount << std::endl;
  return 0;
}
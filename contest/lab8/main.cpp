#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// Function to find the densest subgraph
pair<vector<int>, double> densestSubgraph(const vector<vector<int>>& graph) {
  int n = graph.size();
  if (n == 0) {
    return make_pair(vector<int>(), 0.0);  // Handle empty graph case
  }

  double maxDensity = 0.0;
  vector<int> maxSubgraph;

  for (int i = 0; i < (1 << n); ++i) {
    vector<int> subgraph;
    for (int j = 0; j < n; ++j) {
      if ((i >> j) & 1) {
        subgraph.push_back(j);
      }
    }

    if (subgraph.empty())
      continue;  // Skip empty subgraphs

    int numVertices = subgraph.size();
    int numEdges = 0;
    for (int u = 0; u < numVertices; ++u) {
      for (int v = u + 1; v < numVertices; ++v) {
        if (graph[subgraph[u]][subgraph[v]]) {
          numEdges++;
        }
      }
    }

    double density = (double)numEdges / (numVertices * (numVertices - 1) / 2.0);

    if (density > maxDensity) {
      maxDensity = density;
      maxSubgraph = subgraph;
    }
  }

  return make_pair(maxSubgraph, maxDensity);
}

int main() {
  int n, m;
  cin >> n >> m;

  vector<vector<int>> graph(n);  // Adjacency list

  vector<pair<int, int>> edges;
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    --u;
    --v;
    graph[u].push_back(v);
    graph[v].push_back(u);
  }

  pair<vector<int>, double> result = densestSubgraph(graph);

  cout << "Densest Subgraph Nodes: ";
  for (int node : result.first) {
    cout << node << " ";
  }
  cout << endl;
  cout << "Density: " << result.second << endl;

  return 0;
}

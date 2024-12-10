#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

using namespace std;

void processComponent(
    const vector<vector<int>>& adj,
    vector<bool>& visited,
    int start,
    double& best_density,
    set<int>& best_team
) {
  vector<int> degree(adj.size(), 0);
  set<int> current_nodes;

  queue<int> q;
  q.push(start);
  visited[start] = true;

  // Collect component nodes
  while (!q.empty()) {
    int node = q.front();
    q.pop();
    current_nodes.insert(node);
    for (int neighbor : adj[node]) {
      if (!visited[neighbor]) {
        visited[neighbor] = true;
        q.push(neighbor);
      }
      degree[node]++;
    }
  }

  int remaining_edges = 0;
  for (int node : current_nodes) {
    remaining_edges += degree[node];
  }
  remaining_edges /= 2;  // Each edge counted twice

  // Priority queue for degree updates
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
  for (int node : current_nodes) {
    pq.emplace(degree[node], node);
  }

  while (!pq.empty()) {
    // int d = pq.top().first;
    int node = pq.top().second;
    pq.pop();

    if (!current_nodes.count(node))
      continue;  // Skip stale entries

    double density = static_cast<double>(remaining_edges) / current_nodes.size();
    if (density > best_density) {
      best_density = density;
      best_team = current_nodes;
    }

    // Remove node and update neighbors
    current_nodes.erase(node);
    for (int neighbor : adj[node]) {
      if (current_nodes.count(neighbor)) {
        degree[neighbor]--;
        remaining_edges--;
        pq.emplace(degree[neighbor], neighbor);
      }
    }
  }
}

int main() {
  int n, m;
  cin >> n >> m;

  vector<vector<int>> adj(n + 1);
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  double best_density = 0.0;
  set<int> best_team;
  vector<bool> visited(n + 1, false);

  // Process each connected component
  for (int i = 1; i <= n; i++) {
    if (!visited[i]) {
      processComponent(adj, visited, i, best_density, best_team);
    }
  }

  // Output results
  cout << best_team.size() << endl;
  for (int node : best_team) {
    cout << node << endl;
  }

  return 0;
}

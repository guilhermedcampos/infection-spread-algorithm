#include <algorithm>
#include <cstdio>
#include <iostream>
#include <stack>
#include <vector>
#include <map>

using namespace std;

// Tarjan's algorithm for finding strongly connected components (Iterative version)
void tarjanSCC(int start, map<int, vector<int>>& adj, vector<int>& disc, vector<int>& low, stack<int>& st,
               vector<bool>& in_stack, vector<int>& scc, int& timer, int& scc_count) {
    stack<pair<int, int>> s;
    s.push({start, 0});

    while (!s.empty()) {
        int u = s.top().first;
        int state = s.top().second;
        s.pop();

        if (state == 0) {
            disc[u] = low[u] = timer++;
            st.push(u);
            in_stack[u] = true;

            s.push({u, 1});
            for (int v : adj[u]) {
                if (disc[v] == -1) {
                    s.push({v, 0});  // Push unvisited neighbors onto the stack
                } else if (in_stack[v]) {
                    low[u] = min(low[u], low[v]);
                }
            }
        } else {
            for (int v : adj[u]) {
                if (in_stack[v]) {
                    low[u] = min(low[u], low[v]);
                }
            }
            if (low[u] == disc[u]) {
                while (!st.empty() && st.top() != u) {
                    int v = st.top();
                    st.pop();
                    in_stack[v] = false;
                    scc[v] = scc_count;
                }
                st.pop();
                in_stack[u] = false;
                scc[u] = scc_count++;  // Increment SCC count when an SCC is found
            }
        }
    }
}

// Iterative DFS function to find the maximum path in the SCC graph
int dfs(int start, map<int, vector<int>>& adj_scc, vector<int>& dp) {
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int u = s.top();
        if (dp[u] != -1) {
            s.pop();
            continue;
        }

        bool finished = true;
        for (int v : adj_scc[u]) {
            if (dp[v] == -1) {
                s.push(v);
                finished = false;
            }
        }

        if (finished) {
            dp[u] = 0;
            for (int v : adj_scc[u]) {
                dp[u] = max(dp[u], 1 + dp[v]);
            }
            s.pop();
        }
    }

    return dp[start];
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    map<int, vector<int>> adj;
    vector<int> disc(n + 1, -1);
    vector<int> low(n + 1, -1);
    vector<int> scc(n + 1);
    vector<bool> in_stack(n + 1, false);
    stack<int> st;
    int timer = 0;
    int scc_count = 0;

    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        adj[u].push_back(v);
    }

    // Run Tarjan's algorithm to find SCCs
    for (int i = 1; i <= n; i++) {
        if (disc[i] == -1) {
            tarjanSCC(i, adj, disc, low, st, in_stack, scc, timer, scc_count);
        }
    }

    // Create SCC graph and calculate in-degrees
    map<int, vector<int>> adj_scc;
    vector<int> in_degree(scc_count, 0);

    for (auto& kv : adj) {
        int u = kv.first;
        for (int v : kv.second) {
            if (scc[u] != scc[v]) {
                adj_scc[scc[u]].push_back(scc[v]);
                in_degree[scc[v]]++;
            }
        }
    }

    // Optimization: Only call DFS for nodes that have no incoming edges
    vector<int> dp(scc_count, -1);
    int max_connections = 0;
    for (int i = 0; i < scc_count; i++) {
        if (in_degree[i] == 0) {
            max_connections = max(max_connections, dfs(i, adj_scc, dp));
        }
    }

    printf("%d\n", max_connections);

    return 0;
}

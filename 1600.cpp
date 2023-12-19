#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

/**
 * @brief Tarjan's algorithm for finding SCCs.
 *
 * @param start Starting node.
 * @param adj Adjacency list of the graph.
 * @param disc Discovery times of nodes.
 * @param low Low-link values of nodes.
 * @param scc SCC identifiers of nodes.
 * @param in_stack Boolean array to check if a node is in the stack.
 * @param st Stack of nodes.
 * @param timer Timer for discovery times.
 * @param scc_count Count of SCCs.
 */
void tarjanSCC(int start, vector<vector<int>> &adj, vector<int> &disc,
               vector<int> &low, vector<int> &scc, vector<bool> &in_stack,
               vector<int> &st, int &timer, int &scc_count) {
    vector<int> s;
    s.reserve(adj.size());
    s.push_back(start);

    vector<bool> color(adj.size(), false);

    while (!s.empty()) {
        int u = s.back();

        if (!color[u]) {
            disc[u] = low[u] = timer++;
            st.push_back(u);
            in_stack[u] = true;
            color[u] = true;

            for (int v : adj[u]) {
                if (disc[v] == -1) {
                    s.push_back(v);
                    disc[v] = timer;
                    low[v] = timer;
                    timer++;
                } else if (in_stack[v]) {
                    low[u] = min(low[u], low[v]);
                }
            }
        } else {
            s.pop_back();
            for (int v : adj[u]) {
                if (in_stack[v]) {
                    low[u] = min(low[u], low[v]);
                }
            }
            if (low[u] == disc[u]) {
                while (!st.empty() && st.back() != u) {
                    int v = st.back();
                    st.pop_back();
                    in_stack[v] = false;
                    scc[v] = scc_count;
                }
                st.pop_back();
                in_stack[u] = false;
                scc[u] = scc_count++;
            }
        }
    }
}

/**
 * @brief Depth-first search for finding the maximum number of connections.
 *
 * @param start Starting node.
 * @param dp Dynamic programming array for storing the maximum number of
 * connections from each node.
 * @param adj_scc Adjacency list of the SCC graph.
 * @return int Maximum number of connections from the starting node.
 */
int dfs(int start, vector<int> &dp, vector<vector<int>> &adj_scc) {
    vector<int> s;
    s.reserve(adj_scc.size()); // reserve memory
    s.push_back(start);

    while (!s.empty()) {
        int u = s.back();
        if (dp[u] != -1) {
            s.pop_back();
            continue;
        }

        bool finished = true;
        for (int v : adj_scc[u]) {
            if (dp[v] == -1) {
                s.push_back(v);
                finished = false;
            }
        }

        if (finished) {
            dp[u] = 0;
            for (int v : adj_scc[u]) {
                dp[u] = max(dp[u], 1 + dp[v]);
            }
            s.pop_back();
        }
    }

    return dp[start];
}

/**
 * @brief Main function.
 *
 * @return int Exit status.
 */
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    scanf("%d %d", &n, &m);

    vector<vector<int>> adj(n + 1),
        adj_scc; // adjacency list of the graph and the SCC graph
    vector<int> disc(n + 1, -1), low(n + 1, -1), scc(n + 1), in_degree,
        st; // discovery times, low-link values, SCC identifiers, and in-degrees
            // of nodes
    vector<bool> in_stack(
        n + 1, false); // boolean array to check if a node is in the stack
    int timer = 0, scc_count = 0; // timer for discovery times, count of SCCs

    adj.resize(n + 1);
    disc.resize(n + 1, -1);
    low.resize(n + 1, -1);
    scc.resize(n + 1);
    in_stack.resize(n + 1, false);

    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        adj[u].push_back(v);
    }

    for (int i = 1; i <= n; i++) {
        if (disc[i] == -1) {
            tarjanSCC(i, adj, disc, low, scc, in_stack, st, timer, scc_count);
        }
    }

    adj_scc.resize(scc_count);
    in_degree.resize(scc_count, 0);

    for (int u = 1; u <= n; u++) {
        for (int v : adj[u]) {
            if (scc[u] != scc[v]) {
                adj_scc[scc[u]].push_back(scc[v]);
                in_degree[scc[v]]++;
            }
        }
    }

    vector<int> dp(scc_count, -1);
    int max_connections = 0;
    for (int i = 0; i < scc_count; i++) {
        if (in_degree[i] == 0) {
            max_connections = max(max_connections, dfs(i, dp, adj_scc));
        }
    }

    printf("%d\n", max_connections);

    return 0;
}
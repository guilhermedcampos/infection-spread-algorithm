#include <algorithm>
#include <cstdio>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

vector<vector<int>> adj, adj_scc;
vector<int> disc, low, scc, in_degree;
vector<bool> in_stack;
stack<int> st;
int timer = 0, scc_count = 0;

void tarjanSCC(int u) {
    disc[u] = low[u] = timer++;
    st.push(u);
    in_stack[u] = true;

    for (int v : adj[u]) {
        int disc_v = disc[v];
        if (disc_v == -1) {
            tarjanSCC(v);
            low[u] = min(low[u], low[v]);
        } else if (in_stack[v]) {
            low[u] = min(low[u], disc_v);
        }
    }

    if (low[u] == disc[u]) {
        while (true) {
            int v = st.top();
            st.pop();
            in_stack[v] = false;
            scc[v] = scc_count;
            if (u == v)
                break;
        }
        scc_count++;
    }
}

int dfs(int u, vector<int> &dp) {
    if (dp[u] != -1)
        return dp[u];
    dp[u] = 0;
    for (int v : adj_scc[u]) {
        dp[u] = max(dp[u], 1 + dfs(v, dp));
    }
    return dp[u];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    scanf("%d %d", &n, &m);

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
            tarjanSCC(i);
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
        max_connections = max(max_connections, dfs(i, dp));
    }

    printf("%d\n", max_connections);

    return 0;
}
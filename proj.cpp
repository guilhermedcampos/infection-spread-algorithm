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

void tarjanSCC(int start) {
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
                    s.push({v, 0});
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
                scc[u] = scc_count++;
            }
        }
    }
}

int dfs(int start, vector<int> &dp) {
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
        if (in_degree[i] == 0) {
            max_connections = max(max_connections, dfs(i, dp));
        }
    }

    printf("%d\n", max_connections);

    return 0;
}
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

vector<vector<int>> *adj, *adj_scc;
vector<int> *disc, *low, *scc, *in_degree;
vector<bool> *in_stack;
stack<int> st;  // Change to std::stack
int *timer, *scc_count;

// Tarjan's algorithm for finding strongly connected components (Iterative version)
void tarjanSCC(int start) {
    stack<pair<int, int>> s;
    s.push({start, 0});

    while (!s.empty()) {
        int u = s.top().first;
        int state = s.top().second;
        s.pop();

        if (state == 0) {
            (*disc)[u] = (*low)[u] = (*timer)++;
            st.push(u);
            (*in_stack)[u] = true;

            s.push({u, 1});
            for (int v : (*adj)[u]) {
                if ((*disc)[v] == -1) {
                    s.push({v, 0});  // Push unvisited neighbors onto the stack
                } else if ((*in_stack)[v]) {
                    (*low)[u] = min((*low)[u], (*low)[v]);
                }
            }
        } else {
            for (int v : (*adj)[u]) {
                if ((*in_stack)[v]) {
                    (*low)[u] = min((*low)[u], (*low)[v]);
                }
            }
            if ((*low)[u] == (*disc)[u]) {
                while (!st.empty() && st.top() != u) {
                    int v = st.top();
                    st.pop();
                    (*in_stack)[v] = false;
                    (*scc)[v] = (*scc_count);
                }
                st.pop();
                (*in_stack)[u] = false;
                (*scc)[u] = (*scc_count)++;  // Increment SCC count when an SCC is found
            }
        }
    }
}

// Iterative DFS function to find the maximum path in the SCC graph
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
        for (int v : (*adj_scc)[u]) {
            if (dp[v] == -1) {
                s.push(v);
                finished = false;
            }
        }

        if (finished) {
            dp[u] = 0;
            for (int v : (*adj_scc)[u]) {
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

    adj = new vector<vector<int>>(n + 1);
    disc = new vector<int>(n + 1, -1);
    low = new vector<int>(n + 1, -1);
    scc = new vector<int>(n + 1);
    in_stack = new vector<bool>(n + 1, false);
    timer = new int(0);
    scc_count = new int(0);

    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        (*adj)[u].push_back(v);
    }

    // Run Tarjan's algorithm to find SCCs
    for (int i = 1; i <= n; i++) {
        if ((*disc)[i] == -1) {
            tarjanSCC(i);
        }
    }

    // Create SCC graph and calculate in-degrees
    adj_scc = new vector<vector<int>>(*scc_count);
    in_degree = new vector<int>(*scc_count, 0);

    for (int u = 1; u <= n; u++) {
        for (int v : (*adj)[u]) {
            if ((*scc)[u] != (*scc)[v]) {
                (*adj_scc)[(*scc)[u]].push_back((*scc)[v]);
                (*in_degree)[(*scc)[v]]++;
            }
        }
    }

    // Optimization: Only call DFS for nodes that have no incoming edges
    vector<int> dp(*scc_count, -1);
    int max_connections = 0;
    for (int i = 0; i < *scc_count; i++) {
        if ((*in_degree)[i] == 0) {
            max_connections = max(max_connections, dfs(i, dp));
        }
    }

    printf("%d\n", max_connections);

    // Clean up allocated memory
    delete adj;
    delete disc;
    delete low;
    delete scc;
    delete in_stack;
    delete timer;
    delete scc_count;
    delete adj_scc;
    delete in_degree;

    return 0;
}
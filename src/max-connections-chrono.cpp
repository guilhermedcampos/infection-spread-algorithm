#include <algorithm>
#include <cstdio>
#include <iostream>
#include <stack>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono; 

// Graph adjacency list and strongly connected components adjacency vector
vector<vector<int>> graph, sccGraph;
// Discovery times, low link values and strongly connected components
vector<int> disc, low, scc;
// Stack status of nodes
vector<bool> inStack;
// Stack for DFS
stack<int> dfsS;
// Timer for discovery times and counter for SCCs
int timer = 0, scc_count = 0;

// Tarjan's algorithm for finding SCCs
void tarjanSCC(int start) {
    vector<int> visited(graph.size(), false);
    stack<pair<int, size_t>> stack;
    stack.push({start, 0});

    while (!stack.empty()) {
        int node = stack.top().first;
        size_t index = stack.top().second;
        stack.pop();

        if (!visited[node]) {
            visited[node] = true;
            disc[node] = low[node] = timer++;
            inStack[node] = true;
            dfsS.push(node);
        }

        bool Return = false;
        for (size_t i = index; i < graph[node].size(); ++i) {
            int neighbor = graph[node][i];
            if (!visited[neighbor]) {
                stack.push({node, i + 1});
                stack.push({neighbor, 0});
                Return = true;
                break;
            } else if (inStack[neighbor]) {
                low[node] = min(low[node], low[neighbor]);
            }
        }

        if (Return)
            continue;

        if (disc[node] == low[node]) {
            while (!dfsS.empty()) {
                int stackNode = dfsS.top();
                dfsS.pop();
                inStack[stackNode] = false;
                scc[stackNode] = scc_count;
                if (stackNode == node)
                    break;
            }
            scc_count++;
        } else if (!stack.empty()) {
            int parentNode = stack.top().first;
            low[parentNode] = min(low[parentNode], low[node]);
        }
    }
}

// DFS for finding the maximum number of connections
int dfs(int node, vector<int> &dp) {
    if (dp[node] != -1)
        return dp[node];
    dp[node] = 0;
    for (int neighbor : sccGraph[node]) {
        dp[node] = max(dp[node], 1 + dfs(neighbor, dp));
    }
    return dp[node];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int numNodes, numEdges;
    scanf("%d %d", &numNodes, &numEdges);
    auto start = high_resolution_clock::now();  

    graph.resize(numNodes + 1);
    disc.resize(numNodes + 1, -1);
    low.resize(numNodes + 1, -1);
    scc.resize(numNodes + 1);
    inStack.resize(numNodes + 1, false);

    for (int i = 0; i < numEdges; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        graph[u].push_back(v);
    }

    for (int i = 1; i <= numNodes; i++) {
        if (disc[i] == -1) {
            tarjanSCC(i);
        }
    }

    sccGraph.resize(scc_count);

    for (int u = 1; u <= numNodes; u++) {
        for (int v : graph[u]) {
            if (scc[u] != scc[v]) {
                sccGraph[scc[u]].push_back(scc[v]);
            }
        }
    }

    vector<int> dp(scc_count, -1);
    int max_connections = 0;
    for (int i = 0; i < scc_count; i++) {
        max_connections = max(max_connections, dfs(i, dp));
    }

    printf("%d\n", max_connections);
    auto stop = high_resolution_clock::now();  
    auto duration = duration_cast<milliseconds>(stop - start);  // Calculate the duration
    cout << "Execution Time: " << duration.count() << " milliseconds" << endl;

    return 0;
}
#include <cstdio>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>

class Graph {
private:
    int V;
    std::vector<std::vector<int>> adj, condensedAdj;
    std::vector<int> low, ids, sccs;
    std::stack<int> dfsStack;
    int id = 0, sccCount = 0;
    std::vector<bool> onStack;
    
    void iterativeDFS(int start) {
        std::stack<std::pair<int, size_t>> stack;
        std::vector<int> visited(V, false);
        stack.push({start, 0});

        while (!stack.empty()) {
            int at = stack.top().first;
            size_t index = stack.top().second;
            stack.pop();

            if (!visited[at]) {
                visited[at] = true;
                ids[at] = low[at] = id++;
                onStack[at] = true;
                dfsStack.push(at);
            }

            bool shouldReturn = false;
            for (size_t i = index; i < adj[at].size(); ++i) {
                int to = adj[at][i];
                if (!visited[to]) {
                    stack.push({at, i + 1});
                    stack.push({to, 0});
                    shouldReturn = true;
                    break;
                } else if (onStack[to]) {
                    low[at] = std::min(low[at], low[to]);
                }
            }

            if (shouldReturn) continue;

            if (ids[at] == low[at]) {
                while (!dfsStack.empty()) {
                    int node = dfsStack.top();
                    dfsStack.pop();
                    onStack[node] = false;
                    sccs[node] = sccCount;
                    if (node == at) break;
                }
                sccCount++;
            } else if (!stack.empty()) {
                int parent = stack.top().first;
                low[parent] = std::min(low[parent], low[at]);
            }
        }
    }


    void findSCCs() {
        ids.assign(V, -1);
        low.resize(V);
        onStack.assign(V, false);
        sccs.resize(V);
        for (int i = 0; i < V; ++i) {
            if (ids[i] == -1) iterativeDFS(i);
        }
    }

    void condenseGraph() {
        condensedAdj.resize(sccCount);
        for (int u = 0; u < V; ++u) {
            for (int v : adj[u]) {
                if (sccs[u] != sccs[v]) {
                    condensedAdj[sccs[u]].push_back(sccs[v]);
                }
            }
        }
    }

public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v) {
        if (u < 0 || u >= V || v < 0 || v >= V) return; // Bounds checking
        adj[u].push_back(v);
    } 


    int findLongestPath() {
        findSCCs();
        condenseGraph();
    
        std::vector<int> topoSort;
        std::queue<int> q;
        std::vector<int> condensedInDegree(sccCount, 0);
        for (const auto& edges : condensedAdj) {
            for (int v : edges) {
                condensedInDegree[v]++;
            }
        }

        for (int i = 0; i < sccCount; ++i) {
            if (condensedInDegree[i] == 0) {
                q.push(i);
            }
        }

        while (!q.empty()) {
            int u = q.front();  
            q.pop();  
            topoSort.push_back(u);   
            for (int v : condensedAdj[u]) {
                if (--condensedInDegree[v] == 0) {
                    q.push(v);
                }
            }
        }

        std::vector<int> dist(sccCount, 0);
        for (int u : topoSort) {
            for (int v : condensedAdj[u]) {
                if (dist[v] < dist[u] + 1) {
                    dist[v] = dist[u] + 1;
                }
            }
        }

        return *std::max_element(dist.begin(), dist.end());
    }
};

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) {
        return 1;
    }

    Graph g(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            return 1;
        }
        g.addEdge(u - 1, v - 1); 
    }

    printf("%d\n", g.findLongestPath());

    return 0;
}
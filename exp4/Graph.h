#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <climits>
#include <algorithm>
#include <functional>
using namespace std;

class Graph {
private:
    int vertexNum;
    vector<vector<int> > adjMatrix;
    bool isDirected;

    void dfsHelper(int curr, vector<bool>& visited) const;
    void tarjanHelper(int u, int parent, vector<int>& disc, vector<int>& low, 
                     vector<bool>& visited, vector<bool>& ap, int& time) const;

public:
    Graph(int n, bool directed = false) : vertexNum(n), isDirected(directed) {
        adjMatrix.resize(vertexNum);
        for (int i = 0; i < vertexNum; ++i) {
            adjMatrix[i].resize(vertexNum, 0);
        }
    }

    void addEdge(int v1, int v2, int weight = 1) {
        if (v1 >= 0 && v1 < vertexNum && v2 >= 0 && v2 < vertexNum) {
            adjMatrix[v1][v2] = weight;
            if (!isDirected) {
                adjMatrix[v2][v1] = weight;
            }
        } else {
            cerr << "顶点索引超出范围！" << endl;
        }
    }

    void printAdjMatrix() const {
        cout << "图的邻接矩阵：" << endl;
        cout << "  ";
        for (int i = 0; i < vertexNum; ++i) {
            cout << i << "\t";
        }
        cout << endl;
        
        for (int i = 0; i < vertexNum; ++i) {
            cout << i << " ";
            for (int j = 0; j < vertexNum; ++j) {
                cout << adjMatrix[i][j] << "\t";
            }
            cout << endl;
        }
    }

    void BFS(int start) const {
        if (start < 0 || start >= vertexNum) {
            cerr << "起点索引无效！" << endl;
            return;
        }
        
        vector<bool> visited(vertexNum, false);
        queue<int> q;
        vector<int> result;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            result.push_back(curr);

            for (int i = 0; i < vertexNum; ++i) {
                if (adjMatrix[curr][i] != 0 && !visited[i]) {
                    visited[i] = true;
                    q.push(i);
                }
            }
        }
        
        cout << "BFS遍历顺序（起点" << start << "）：";
        for (size_t i = 0; i < result.size(); ++i) {
            cout << char('A' + result[i]);
            if (i != result.size() - 1) cout << "→";
        }
        cout << endl;
    }

    void DFS_Recursive(int start) const {
        if (start < 0 || start >= vertexNum) {
            cerr << "起点索引无效！" << endl;
            return;
        }
        
        vector<bool> visited(vertexNum, false);
        vector<int> result;
        
        // 使用lambda函数进行递归DFS
        function<void(int)> dfs = [&](int curr) {
            visited[curr] = true;
            result.push_back(curr);
            
            for (int i = 0; i < vertexNum; ++i) {
                if (adjMatrix[curr][i] != 0 && !visited[i]) {
                    dfs(i);
                }
            }
        };
        
        dfs(start);
        
        cout << "DFS递归遍历顺序（起点" << start << "）：";
        for (size_t i = 0; i < result.size(); ++i) {
            cout << char('A' + result[i]);
            if (i != result.size() - 1) cout << "→";
        }
        cout << endl;
    }

    void DFS_Iterative(int start) const {
        if (start < 0 || start >= vertexNum) {
            cerr << "起点索引无效！" << endl;
            return;
        }
        
        vector<bool> visited(vertexNum, false);
        stack<int> st;
        vector<int> result;

        st.push(start);
        visited[start] = true;

        while (!st.empty()) {
            int curr = st.top();
            st.pop();
            result.push_back(curr);

            // 逆序入栈保证遍历顺序与递归一致
            for (int i = vertexNum - 1; i >= 0; --i) {
                if (adjMatrix[curr][i] != 0 && !visited[i]) {
                    visited[i] = true;
                    st.push(i);
                }
            }
        }
        
        cout << "DFS非递归遍历顺序（起点" << start << "）：";
        for (size_t i = 0; i < result.size(); ++i) {
            cout << char('A' + result[i]);
            if (i != result.size() - 1) cout << "→";
        }
        cout << endl;
    }

    void Dijkstra(int start) const {
        if (start < 0 || start >= vertexNum) {
            cerr << "起点索引无效！" << endl;
            return;
        }
        
        vector<int> dist(vertexNum, INT_MAX);
        vector<bool> visited(vertexNum, false);
        vector<int> parent(vertexNum, -1);

        dist[start] = 0;

        for (int count = 0; count < vertexNum - 1; ++count) {
            int minDist = INT_MAX, minIndex = -1;
            
            for (int v = 0; v < vertexNum; ++v) {
                if (!visited[v] && dist[v] < minDist) {
                    minDist = dist[v];
                    minIndex = v;
                }
            }

            if (minIndex == -1) break;
            visited[minIndex] = true;

            for (int v = 0; v < vertexNum; ++v) {
                if (!visited[v] && adjMatrix[minIndex][v] != 0 && 
                    dist[minIndex] != INT_MAX && 
                    dist[minIndex] + adjMatrix[minIndex][v] < dist[v]) {
                    dist[v] = dist[minIndex] + adjMatrix[minIndex][v];
                    parent[v] = minIndex;
                }
            }
        }

        cout << "Dijkstra算法（起点" << char('A' + start) << "）：" << endl;
        cout << "顶点\t距离\t路径" << endl;
        for (int i = 0; i < vertexNum; ++i) {
            cout << char('A' + i) << "\t";
            if (dist[i] == INT_MAX) {
                cout << "不可达\t-";
            } else {
                cout << dist[i] << "\t";
                // 输出路径
                if (i == start) {
                    cout << char('A' + start);
                } else {
                    stack<int> path;
                    int p = i;
                    while (p != -1) {
                        path.push(p);
                        p = parent[p];
                    }
                    while (!path.empty()) {
                        cout << char('A' + path.top());
                        path.pop();
                        if (!path.empty()) cout << "→";
                    }
                }
            }
            cout << endl;
        }
    }

    void Prim(int start) const {
        if (isDirected) {
            cerr << "Prim算法仅适用于无向图！" << endl;
            return;
        }
        if (start < 0 || start >= vertexNum) {
            cerr << "起点索引无效！" << endl;
            return;
        }

        vector<int> parent(vertexNum, -1);
        vector<int> key(vertexNum, INT_MAX);
        vector<bool> inMST(vertexNum, false);
        int totalWeight = 0;

        key[start] = 0;

        for (int count = 0; count < vertexNum; ++count) {
            int minKey = INT_MAX, minIndex = -1;
            for (int v = 0; v < vertexNum; ++v) {
                if (!inMST[v] && key[v] < minKey) {
                    minKey = key[v];
                    minIndex = v;
                }
            }

            if (minIndex == -1) break;
            inMST[minIndex] = true;
            totalWeight += minKey;

            for (int v = 0; v < vertexNum; ++v) {
                if (adjMatrix[minIndex][v] != 0 && !inMST[v] && adjMatrix[minIndex][v] < key[v]) {
                    parent[v] = minIndex;
                    key[v] = adjMatrix[minIndex][v];
                }
            }
        }

        cout << "Prim算法最小支撑树（起点" << char('A' + start) << "）：" << endl;
        cout << "边\t\t权重" << endl;
        for (int i = 0; i < vertexNum; ++i) {
            if (parent[i] != -1) {
                cout << char('A' + parent[i]) << " - " << char('A' + i) << "\t\t" << adjMatrix[parent[i]][i] << endl;
            }
        }
        cout << "最小支撑树总权重：" << totalWeight << endl;
    }

    void Tarjan() const {
        vector<int> disc(vertexNum, -1);
        vector<int> low(vertexNum, -1);
        vector<bool> visited(vertexNum, false);
        vector<bool> ap(vertexNum, false);
        int time = 0;

        for (int i = 0; i < vertexNum; ++i) {
            if (!visited[i]) {
                tarjanHelper(i, -1, disc, low, visited, ap, time);
            }
        }

        cout << "关节点：";
        bool hasAP = false;
        for (int i = 0; i < vertexNum; ++i) {
            if (ap[i]) {
                cout << char('A' + i) << " ";
                hasAP = true;
            }
        }
        if (!hasAP) {
            cout << "无";
        }
        cout << endl;
    }
};

void Graph::dfsHelper(int curr, vector<bool>& visited) const {
    visited[curr] = true;
    cout << char('A' + curr) << " ";

    for (int i = 0; i < vertexNum; ++i) {
        if (adjMatrix[curr][i] != 0 && !visited[i]) {
            dfsHelper(i, visited);
        }
    }
}

void Graph::tarjanHelper(int u, int parent, vector<int>& disc, vector<int>& low, 
                         vector<bool>& visited, vector<bool>& ap, int& time) const {
    int children = 0;
    visited[u] = true;
    disc[u] = low[u] = ++time;

    for (int v = 0; v < vertexNum; ++v) {
        if (adjMatrix[u][v] == 0) continue;

        if (!visited[v]) {
            children++;
            tarjanHelper(v, u, disc, low, visited, ap, time);
            low[u] = min(low[u], low[v]);

            if (parent == -1 && children > 1) {
                ap[u] = true;
            }
            if (parent != -1 && low[v] >= disc[u]) {
                ap[u] = true;
            }
        } else if (v != parent) {
            low[u] = min(low[u], disc[v]);
        }
    }
}

#endif

#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>
#include <queue>
using namespace std;


void parallelBFS(int start,vector<vector<int>>& graph,vector<bool>& visited) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // Process the current node
#pragma omp parallel for
        for (int i = 0; i < graph[current].size(); ++i) {
            int neighbor = graph[current][i];
            if (!visited[neighbor]) {
#pragma omp critical
                {
                    if (!visited[neighbor]) {
                        q.push(neighbor);
                        visited[neighbor] = true;
                    }
                }
            }
        }
    }
}

// Function to perform parallel DFS
void parallelDFS(int start,vector<vector<int>>& graph,vector<bool>& visited) {
    stack<int> st;
    st.push(start);
    visited[start] = true;

    while (!st.empty()) {
        int current = st.top();
        st.pop();

        // Process the current node
#pragma omp parallel for
        for (int i = 0; i < graph[current].size(); ++i) {
            int neighbor = graph[current][i];
            if (!visited[neighbor]) {
#pragma omp critical
                {
                    if (!visited[neighbor]) {
                        st.push(neighbor);
                        visited[neighbor] = true;
                    }
                }
            }
        }
    }
}

int main() {
    // Create the graph
    vector<vector<int>> graph = {
        {1, 2},         // Node 0 is connected to nodes 1 and 2
        {0, 3, 4},      // Node 1 is connected to nodes 0, 3, and 4
        {0, 5},         // Node 2 is connected to nodes 0 and 5
        {1},            // Node 3 is connected to node 1
        {1, 6, 7},      // Node 4 is connected to nodes 1, 6, and 7
        {2},            // Node 5 is connected to node 2
        {4},            // Node 6 is connected to node 4
        {4}             // Node 7 is connected to node 4
    };

    // Number of nodes in the graph
    int numNodes = graph.size();

    // Create a visited array to track visited nodes
    vector<bool> visited(numNodes, false);
    vector<bool> visited1(numNodes, false);

    cout << "\nFOR DFS :\n";


    // Perform parallel DFS from node 0
    parallelDFS(0, graph, visited);

    // Print the visited array
    for (int i = 0; i < numNodes; ++i) {
        cout << "Node " << i << ": " << (visited[i] ? "Visited" : "Not Visited") <<endl;
    }

    cout << "\nFOR BFS :\n";

    parallelBFS(0, graph, visited1);

    // Print the visited array
    for (int i = 0; i < numNodes; ++i) {
        cout << "Node " << i << ": " << (visited1[i] ? "Visited" : "Not Visited") <<endl;
    }

    return 0;
}

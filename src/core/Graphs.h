#ifndef GRAPHS_H
#define GRAPHS_H
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
using namespace std;

class Graph
{  
private:
    static const int maxSize = 20;
    int adjMatrix[maxSize][maxSize];
    vector<int> vertList;

public:
    Graph();
    Graph(int size);
    ~Graph();

    int GetVertPos(int vertex) const;
    int GetAmountVerts() const;
    int GetAmountEdges() const;
    int GetWeight(int v1, int v2) const;
    int GetVertex(int index) const;
    vector<int> GetNbrs(int vertex) const;

    bool InsertVertex(int vertex);
    bool InsertEdge(int v1, int v2, int weight);
    bool IsEmpty() const;
    bool IsFull() const;
    bool RemoveVertex(int vertex);
    bool RemoveEdge(int u, int v);
    string PrintToString() const;
};

#endif // GRAPHS_H

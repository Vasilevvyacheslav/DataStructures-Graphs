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
    int GetWeight(int vertex1, int vertex2) const;
    vector<T> GetNbrs(int vertex) const;

    bool InsertVertex(int vertex);
    bool InsertEdge(int vertex1, int vertex2, int weight);
    bool IsEmpty() const;
    bool IsFull() const;
    string PrintToString() const;
};


#ifndef GRAPHALGORITHMS_H
#define GRAPHALGORITHMS_H
#include "Graphs.h"
#include <vector>

class GraphAlgorithms
{
public:
    static void DFS(const Graph& g, int startVertex);
    static void BFS(const Graph& g, int startVertex);
    static std::vector<int> Dijkstra(const Graph& g, int start, int end);
    static std::vector<std::vector<int>> Floyd(const Graph& g);

private:
    static void DFS_Util(const Graph& g, int vIdx, std::vector<bool>& visited);
};

#endif // GRAPHALGORITHMS_H

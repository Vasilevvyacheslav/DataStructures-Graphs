#ifndef GRAPHALGORITHMS_H
#define GRAPHALGORITHMS_H

#include "Graphs.h"
#include <vector>
#include <algorithm>
#include <utility>
#include <queue>
#include <string>
using namespace std;

struct BFSStep {
    int currentVertex;// Вершина, которую сейчас обрабатываем (подсвечиваем красным/желтым)
    vector<int> visitedVertices;// Список всех посещенных на данный момент вершин (подсвечиваем зеленым)
    string logText;// Текстовый вывод того, что произошло (например: "Посетили вершину 1, добавили соседей...")
};

struct DFSStep {
    int currentVertex;
    vector<int> visitedVertices;
    vector<int> currentPath; // Стек рекурсии
    string logText;
};

struct DijkstraStep {
    int currentVertex; // Текущая вершина (обрабатываемая)
    vector<int> distances; // Расстояния до всех вершин (метки)
    vector<bool> visited; // Обработанные вершины
    string logText; // Описание шага
};

struct FloydStep {
    int k;  // Текущая промежуточная вершина
    int i;  // Текущая строка (откуда)
    int j;  // Текущий столбец (куда)
    vector<vector<int>> dist;  // Текущая матрица расстояний
    string logText;  // Описание шага
    bool wasUpdated;  // Было ли обновление на этом шаге
};
class GraphAlgorithms
{
public:
    static vector<BFSStep> GenerateBFSSteps(const Graph& g, int startVertex);
    static vector<DFSStep> GenerateDFSSteps(const Graph& g, int startVertex);
    static vector<DijkstraStep> GenerateDijkstraSteps(const Graph& g, int startVertex);
    static vector<FloydStep> GenerateFloydSteps(const Graph& g);
};

#endif // GRAPHALGORITHMS_H

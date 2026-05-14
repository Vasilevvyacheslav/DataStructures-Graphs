#include "GraphAlgorithms.h"
#include <queue>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>

using namespace std;
const int INF = 10000;

vector<BFSStep> GraphAlgorithms::GenerateBFSSteps(const Graph& g, int startVertex) {
    vector<BFSStep> steps;
    int n = g.GetAmountVerts();

    vector<bool> visited(n, false);
    queue<int> q;

    int startIdx = g.GetVertPos(startVertex);
    if (startIdx == -1) return steps;

    // Старт
    BFSStep startStep;
    startStep.currentVertex = startVertex;
    startStep.logText = "Старт BFS от вершины " + to_string(startVertex);
    steps.push_back(startStep);

    q.push(startIdx);
    visited[startIdx] = true;

    BFSStep visitStep = startStep;
    visitStep.visitedVertices.push_back(startVertex);
    visitStep.logText = "Посещаем вершину " + to_string(startVertex);
    steps.push_back(visitStep);

    while (!q.empty()) {
        int uIdx = q.front();
        q.pop();
        int uVal = g.GetVertex(uIdx);

        BFSStep popStep;
        popStep.currentVertex = uVal;
        popStep.visitedVertices = visitStep.visitedVertices;
        popStep.logText = "Извлекаем " + to_string(uVal) + " из очереди";
        steps.push_back(popStep);

        for (int vIdx = 0; vIdx < n; ++vIdx) {
            int vVal = g.GetVertex(vIdx);
            if (g.GetWeight(uVal, vVal) != 0 && !visited[vIdx]) {

                visited[vIdx] = true;
                q.push(vIdx);
                visitStep.visitedVertices.push_back(vVal);

                BFSStep neighborStep;
                neighborStep.currentVertex = uVal;
                neighborStep.visitedVertices = visitStep.visitedVertices;
                neighborStep.logText = "Найден сосед " + to_string(vVal) + " (от " + to_string(uVal) + ")";
                steps.push_back(neighborStep);
            }
        }
    }

    BFSStep endStep;
    endStep.currentVertex = -1;
    endStep.visitedVertices = visitStep.visitedVertices;
    endStep.logText = "BFS завершен";
    steps.push_back(endStep);

    return steps;
}

// Вспомогательная рекурсивная функция
void DFS_StepUtil(const Graph& g, int uIdx, vector<bool>& visited,
                  vector<int>& path, vector<DFSStep>& steps) {

    visited[uIdx] = true;
    int uVal = g.GetVertex(uIdx);
    path.push_back(uVal);

    // Собираем список посещённых значений
    vector<int> visList;
    for (int i = 0; i < g.GetAmountVerts(); ++i)
    {
        if (visited[i]) visList.push_back(g.GetVertex(i));
    }
    // Посетили вершину
    DFSStep s1;
    s1.currentVertex = uVal;
    s1.visitedVertices = visList;
    s1.currentPath = path;
    s1.logText = "Посещаем вершину " + to_string(uVal);
    steps.push_back(s1);

    // Проходим по всем возможным соседям
    for (int i = 0; i < g.GetAmountVerts(); ++i) {
        int vVal = g.GetVertex(i);
        if (g.GetWeight(uVal, vVal) != 0 && !visited[i]) {

            // Перед переходом к соседу
            DFSStep s2 = s1;
            s2.logText = "Идём вглубь к соседу " + to_string(vVal);
            steps.push_back(s2);

            // Рекурсивный вызов
            DFS_StepUtil(g, i, visited, path, steps);

            // Вернулись
            DFSStep s3;
            s3.currentVertex = uVal;
            s3.visitedVertices = visList;
            s3.currentPath = path;
            s3.logText = "Возврат из " + to_string(vVal) + " в " + to_string(uVal);
            steps.push_back(s3);
        }
    }

    path.pop_back(); // Убираем из стека при возврате
}

vector<DFSStep> GraphAlgorithms::GenerateDFSSteps(const Graph& g, int startVertex) {
    vector<DFSStep> steps;
    int n = g.GetAmountVerts();
    vector<bool> visited(n, false);
    vector<int> path;

    int startIdx = g.GetVertPos(startVertex);
    if (startIdx == -1) return steps;

    DFS_StepUtil(g, startIdx, visited, path, steps);

    DFSStep endStep;
    endStep.currentVertex = -1;
    endStep.visitedVertices = vector<int>();
    endStep.currentPath = vector<int>();
    endStep.logText = "DFS завершен";
    steps.push_back(endStep);

    return steps;
}

vector<DijkstraStep> GraphAlgorithms::GenerateDijkstraSteps(const Graph& g, int startVertex) {
    vector<DijkstraStep> steps;
    int n = g.GetAmountVerts();

    vector<int> dist(n, INF);
    vector<bool> visited(n, false);

    int startIdx = g.GetVertPos(startVertex);
    if (startIdx == -1) return steps;

    dist[startIdx] = 0;

    // Инициализация
    DijkstraStep initStep;
    initStep.currentVertex = startVertex;
    initStep.distances = dist;
    initStep.visited = visited;
    initStep.logText = "Инициализация: расстояние до вершины " + to_string(startVertex) + " = 0, остальные = ∞";
    steps.push_back(initStep);

    for (int count = 0; count < n; count++) {
        int u = -1;
        int minDist = INF;

        // Поиск непосещённой вершины с минимальным расстоянием
        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] <= minDist) {
                minDist = dist[i];
                u = i;
            }
        }

        if (u == -1) break; // Все достижимые вершины найдены

        visited[u] = true;
        int uVal = g.GetVertex(u);

        // Выбрали вершину с минимальным расстоянием
        DijkstraStep selectStep;
        selectStep.currentVertex = uVal;
        selectStep.distances = dist;
        selectStep.visited = visited;
        selectStep.logText = "Выбираем вершину " + to_string(uVal) + " (минимальная метка: " +
                            (minDist >= INF ? "∞" : to_string(minDist)) + ")";
        steps.push_back(selectStep);

        // Обновление соседей
        for (int v = 0; v < n; v++) {
            int vVal = g.GetVertex(v);
            int weight = g.GetWeight(uVal, vVal);

            if (weight != 0 && !visited[v]) {
                int oldDist = dist[v];

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;

                    // Обновили расстояние
                    DijkstraStep updateStep;
                    updateStep.currentVertex = uVal;
                    updateStep.distances = dist;
                    updateStep.visited = visited;
                    updateStep.logText = "Обновляем расстояние до " + to_string(vVal) + ": " +
                                        (oldDist >= INF ? "∞" : to_string(oldDist)) + " → " +
                                        to_string(dist[v]) + " (через " + to_string(uVal) +
                                        ", вес ребра " + to_string(weight) + ")";
                    steps.push_back(updateStep);
                }
            }
        }
    }

    // Финальный шаг
    DijkstraStep endStep;
    endStep.currentVertex = -1;
    endStep.distances = dist;
    endStep.visited = visited;
    endStep.logText = "Алгоритм Дейкстры завершён";
    steps.push_back(endStep);

    return steps;
}

vector<FloydStep> GraphAlgorithms::GenerateFloydSteps(const Graph& g) {
    vector<FloydStep> steps;
    int n = g.GetAmountVerts();

    // Инициализация матрицы
    vector<vector<int>> dist(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                dist[i][j] = 0;
            } else {
                int w = g.GetWeight(g.GetVertex(i), g.GetVertex(j));
                dist[i][j] = (w != 0) ? w : INF;
            }
        }
    }

    // Начальная матрица
    FloydStep initStep;
    initStep.k = -1; initStep.i = -1; initStep.j = -1;
    initStep.dist = dist;
    initStep.wasUpdated = false;
    initStep.logText = "Инициализация матрицы расстояний";
    steps.push_back(initStep);

    // Основной алгоритм
    for (int k = 0; k < n; k++) {
        // Новая промежуточная вершина
        FloydStep newKStep;
        newKStep.k = k; newKStep.i = -1; newKStep.j = -1;
        newKStep.dist = dist;
        newKStep.wasUpdated = false;
        newKStep.logText = "Рассматриваем вершину " + to_string(g.GetVertex(k)) + " как промежуточную";
        steps.push_back(newKStep);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int oldDist = dist[i][j];
                int byK = dist[i][k] + dist[k][j];

                // Проверяем, улучшает ли путь через k
                if (dist[i][k] < INF && dist[k][j] < INF && byK < dist[i][j]) {
                    dist[i][j] = byK;

                    // Обновили расстояние
                    FloydStep updateStep;
                    updateStep.k = k; updateStep.i = i; updateStep.j = j;
                    updateStep.dist = dist;
                    updateStep.wasUpdated = true;
                    updateStep.logText = "Улучшаем путь " + to_string(g.GetVertex(i)) + "→" +
                                        to_string(g.GetVertex(j)) + ": " +
                                        (oldDist >= INF ? "∞" : to_string(oldDist)) + " → " +
                                        to_string(dist[i][j]) + " (через " + to_string(g.GetVertex(k)) + ")";
                    steps.push_back(updateStep);
                }
            }
        }
    }

    // Финальный шаг
    FloydStep endStep;
    endStep.k = -2; endStep.i = -1; endStep.j = -1;
    endStep.dist = dist;
    endStep.wasUpdated = false;
    endStep.logText = "Алгоритм Флойда-Уоршелла завершён";
    steps.push_back(endStep);

    return steps;
}

#ifndef ALGORITHMVISUALIZER_H
#define ALGORITHMVISUALIZER_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QColor>
#include <QPointF>
#include <vector>
#include <string>
#include "Graphs.h"
#include "GraphAlgorithms.h"

class AlgorithmVisualizer
{
public:
    AlgorithmVisualizer(QGraphicsScene* scene, const Graph* graph);

    // Основные методы отрисовки
    void drawBFSStep(const BFSStep& step);
    void drawDFSStep(const DFSStep& step);
    void drawDijkstraStep(const DijkstraStep& step);
    void drawFloydStep(const FloydStep& step);
    // Сброс к обычному виду
    void resetView();
    void drawGraph();
    // Настройки цветов
    void setColors(QColor unvisited, QColor visited, QColor active);
    // Обновление графа
    void setGraph(const Graph* newGraph);

private:
    QGraphicsScene* m_scene;
    const Graph* m_graph;

    // Цвета по умолчанию
    QColor m_colorUnvisited = QColor(60, 60, 60);// Серый
    QColor m_colorVisited = QColor(0, 180, 0);// Зелёный (BFS)
    QColor m_colorActive = QColor(200, 200, 0);// Жёлтый (текущая)
    QColor m_colorDijkstraActive = QColor(200, 200, 0); // Жёлтый (текущая)
    QColor m_colorDijkstraVisited = QColor(0, 150, 0); // Зелёный (обработанная)
    QColor m_colorDijkstraUnvisited = QColor(100, 100, 150); // Синеватый (непосещённая)

    static constexpr int CENTER_X = 200;
    static constexpr int CENTER_Y = 200;
    static constexpr int RADIUS = 150;
    static constexpr double PI = 3.1415926535;

    static constexpr int VERTEX_RADIUS = 20;      // Радиус круга вершины
    static constexpr int EDGE_WEIGHT_OFFSET = 12; // Отступ веса от линии

    // Шрифты
    QFont m_fontVertex;
    QFont m_fontWeight;

    // Вспомогательный метод для отрисовки вершины
    void drawVertex(int val, QPointF pos, bool isVisited, bool isActive, bool isDFS = false);
    void drawEdges();
    void drawDistanceMatrix(const vector<vector<int>>& dist, int highlightK, int highlightI, int highlightJ);

    // Вычисление координат
    QPointF getVertexPosition(int index, int n);
};

#endif // ALGORITHMVISUALIZER_H

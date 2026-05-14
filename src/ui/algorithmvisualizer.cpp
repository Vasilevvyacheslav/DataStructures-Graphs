#include "algorithmvisualizer.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QColor>
#include <algorithm>
#include <cmath>

AlgorithmVisualizer::AlgorithmVisualizer(QGraphicsScene* scene, const Graph* graph)
    : m_scene(scene), m_graph(graph)
{
    m_fontVertex = QFont("Arial", 12, QFont::Bold);
    m_fontWeight = QFont("Arial", 11, QFont::Bold);
}

// Простая отрисовка графа (кружочки)
void AlgorithmVisualizer::drawGraph() {
    if (!m_scene || !m_graph) return;
    m_scene->clear();

    // Рисуем рёбра и веса
    drawEdges();

    // Рисуем вершины
    int n = m_graph->GetAmountVerts();
    for (int i = 0; i < n; ++i) {
        int val = m_graph->GetVertex(i);
        QPointF pos = getVertexPosition(i, n);
        drawVertex(val, pos, false, false);
    }
}

void AlgorithmVisualizer::drawEdges() {
    int n = m_graph->GetAmountVerts();
    if (n == 0) return;

    QFont fontLabel("Arial", 10, QFont::Bold);

    for (int i = 0; i < n; ++i) {
        int v1 = m_graph->GetVertex(i);
        QPointF pos1 = getVertexPosition(i, n);

        for (int j = i + 1; j < n; ++j) {
            int v2 = m_graph->GetVertex(j);
            int weight = m_graph->GetWeight(v1, v2);

            if (weight != 0) {
                QPointF pos2 = getVertexPosition(j, n);

                // Рисуем линию
                QGraphicsLineItem* line = m_scene->addLine(pos1.x(), pos1.y(), pos2.x(), pos2.y(), QPen(Qt::white, 2));
                line->setZValue(0);

                // Рисуем вес
                QPointF mid = (pos1 + pos2) / 2;

                double dx = pos2.x() - pos1.x();
                double dy = pos2.y() - pos1.y();
                double len = std::sqrt(dx*dx + dy*dy);

                if (len > 0) {
                    double nx = -dy / len;
                    double ny = dx / len;
                    int offset = ((v1 + v2) % 2 != 0) ? -12 : 12;
                    mid += QPointF(nx * offset, ny * offset);
                }

                QGraphicsTextItem* textW = m_scene->addText(QString::number(weight));
                textW->setFont(fontLabel);
                textW->setDefaultTextColor(QColor(255, 215, 0)); // Золотой
                textW->setZValue(1); // Вес чуть выше линий, но ниже вершин

                QRectF rect = textW->boundingRect();
                textW->setPos(mid.x() - rect.width()/2, mid.y() - rect.height()/2);
            }
        }
    }
}
void AlgorithmVisualizer::drawVertex(int val, QPointF pos, bool isVisited, bool isActive, bool isDFS) {
    QColor brush = m_colorUnvisited;
    QPen pen(Qt::white, 2);

    if (isActive) {
        brush = m_colorActive;
        pen.setColor(QColor(255, 255, 0));
        pen.setWidth(4);
    } else if (isVisited) {
        brush = isDFS ? QColor(100, 50, 150) : m_colorVisited;
        pen.setColor(isDFS ? QColor(150, 100, 255) : QColor(0, 255, 0));
    }

    // Круг вершины
    int r = VERTEX_RADIUS;
    m_scene->addEllipse(pos.x() - r, pos.y() - r, 2*r, 2*r, pen, QBrush(brush));

    // Текст (номер)
    QGraphicsTextItem* text = m_scene->addText(QString::number(val));
    text->setFont(m_fontVertex);
    text->setDefaultTextColor(Qt::white);

    // Центрирование текста
    QRectF rect = text->boundingRect();
    text->setPos(pos.x() - rect.width() / 2, pos.y() - rect.height() / 2);
}
// Отрисовка шага BFS
void AlgorithmVisualizer::drawBFSStep(const BFSStep& step) {
    if (!m_scene || !m_graph) return;
    m_scene->clear();

    int n = m_graph->GetAmountVerts();
    if (n == 0) return;

    // Рёбра
    drawEdges();

    // Вершины с подсветкой
    for (int i = 0; i < n; ++i) {
        int val = m_graph->GetVertex(i);
        QPointF pos = getVertexPosition(i, n);

        bool isVisited = std::find(step.visitedVertices.begin(), step.visitedVertices.end(), val) != step.visitedVertices.end();
        bool isActive = (step.currentVertex == val);

        drawVertex(val, pos, isVisited, isActive, false); // false - стиль для отрисовки BFS
    }
}

// Отрисовка шага DFS
void AlgorithmVisualizer::drawDFSStep(const DFSStep& step) {
    if (!m_scene || !m_graph) return;
    m_scene->clear();

    int n = m_graph->GetAmountVerts();
    if (n == 0) return;

    // Рёбра
    drawEdges();

    // Вершины с подсветкой DFS
    for (int i = 0; i < n; ++i) {
        int val = m_graph->GetVertex(i);
        QPointF pos = getVertexPosition(i, n);

        bool isVisited = std::find(step.visitedVertices.begin(), step.visitedVertices.end(), val) != step.visitedVertices.end();
        bool inPath = std::find(step.currentPath.begin(), step.currentPath.end(), val) != step.currentPath.end();
        bool isActive = (step.currentVertex == val);

        if (inPath) {
            drawVertex(val, pos, true, true, true);
        } else {
            drawVertex(val, pos, isVisited, isActive, true);
        }
    }
}

QPointF AlgorithmVisualizer::getVertexPosition(int index, int n) {
    double angle = (2 * PI * index) / n;
    return QPointF(CENTER_X + RADIUS * cos(angle), CENTER_Y + RADIUS * sin(angle));
}

void AlgorithmVisualizer::drawDijkstraStep(const DijkstraStep& step) {
    if (!m_scene || !m_graph) return;
    m_scene->clear();

    int n = m_graph->GetAmountVerts();
    if (n == 0) return;

    // Рисуем рёбра и веса
    drawEdges();

    // Рисуем вершины и метки
    QFont fontDistance("Arial", 12, QFont::Bold);

    for (int i = 0; i < n; ++i) {
        int val = m_graph->GetVertex(i);
        QPointF pos = getVertexPosition(i, n);

        bool isVisited = step.visited[i];
        bool isCurrent = (step.currentVertex == val);
        int distance = step.distances[i];

        // Рисуем цветную вершину
        drawVertex(val, pos, isVisited, isCurrent);

        // Рисуем метку расстояния над вершиной
        QString labelText = (distance >= 10000) ? "∞" : QString::number(distance);
        QGraphicsTextItem* label = m_scene->addText(labelText);
        label->setFont(fontDistance);
        label->setDefaultTextColor(QColor(255, 255, 0)); // Золотой
        label->setZValue(10); // Поверх всего

        QRectF rectLabel = label->boundingRect();
        // Позиция: над вершиной (на 45 пикселей выше центра)
        label->setPos(pos.x() - rectLabel.width()/2, pos.y() - 45);
    }
}
void AlgorithmVisualizer::drawFloydStep(const FloydStep& step) {
    if (!m_scene || !m_graph) return;
    m_scene->clear();

    int n = m_graph->GetAmountVerts();
    if (n == 0) return;

    // 1. Рисуем рёбра
    drawEdges();

    // 2. Подсвечиваем вершины k, i, j
    for (int idx = 0; idx < n; ++idx) {
        int val = m_graph->GetVertex(idx);
        QPointF pos = getVertexPosition(idx, n);

        bool isK = (step.k == idx);
        bool isI = (step.i == idx);
        bool isJ = (step.j == idx);

        QColor brush = m_colorUnvisited;
        QPen pen(Qt::white, 2);

        if (isK) {
            brush = QColor(255, 100, 100); // Красная (промежуточная)
            pen.setColor(QColor(255, 0, 0));
            pen.setWidth(5);
        } else if (isI || isJ) {
            brush = QColor(100, 200, 255); // Голубая (пара)
            pen.setColor(QColor(0, 200, 255));
            pen.setWidth(4);
        }

        drawVertex(val, pos, true, false);

        // Перерисовываем выделенные вершины
        if (isK || isI || isJ) {
            int r = VERTEX_RADIUS;
            m_scene->addEllipse(pos.x() - r, pos.y() - r, 2*r, 2*r, pen, QBrush(brush));

            QGraphicsTextItem* txt = m_scene->addText(QString::number(val));
            txt->setFont(m_fontVertex);
            txt->setDefaultTextColor(Qt::white);
            QRectF rect = txt->boundingRect();
            txt->setPos(pos.x() - rect.width()/2, pos.y() - rect.height()/2);
        }
    }
    // Текст для текущей итерации
    if (step.k >= 0) {
        QString kStr = QString::number(m_graph->GetVertex(step.k));
        QString iStr = (step.i >= 0) ? QString::number(m_graph->GetVertex(step.i)) : "-";
        QString jStr = (step.j >= 0) ? QString::number(m_graph->GetVertex(step.j)) : "-";

        QGraphicsTextItem* info = m_scene->addText(
            "k = " + kStr + "\ni = " + iStr + "\nj = " + jStr
        );
        info->setFont(QFont("Arial", 11, QFont::Bold));
        info->setDefaultTextColor(Qt::white);
        info->setPos(10, 10);
        info->setZValue(100);
    }
}

void AlgorithmVisualizer::drawDistanceMatrix(const vector<vector<int>>& dist, int highlightK, int highlightI, int highlightJ) {
    int n = dist.size();
    if (n == 0) return;

    QFont fontMatrix("Arial", 11);
    QFont fontBold("Arial", 11, QFont::Bold);
    int cellSize = 45;
    int startX = 420;  // Справа от графа
    int startY = 20;

    // Заголовок
    QGraphicsTextItem* title = m_scene->addText("Матрица расстояний:");
    title->setFont(QFont("Arial", 12, QFont::Bold));
    title->setDefaultTextColor(Qt::white);
    title->setPos(startX, startY - 25);

    // Заголовки столбцов
    for (int j = 0; j < n; j++) {
        QGraphicsTextItem* txt = m_scene->addText(QString::number(j));
        txt->setFont(fontBold);
        txt->setDefaultTextColor(QColor(255, 215, 0));
        txt->setPos(startX + j * cellSize + 10, startY - 20);
    }

    // Ячейки матрицы
    for (int i = 0; i < n; i++) {
        // Заголовок строки
        QGraphicsTextItem* rowLabel = m_scene->addText(QString::number(i));
        rowLabel->setFont(fontBold);
        rowLabel->setDefaultTextColor(QColor(255, 215, 0));
        rowLabel->setPos(startX - 20, startY + i * cellSize + 12);

        for (int j = 0; j < n; j++) {
            int val = dist[i][j];
            QString text = (val >= 10000) ? "∞" : QString::number(val);

            // Цвет ячейки
            QColor bgColor = QColor(40, 40, 40);
            QColor textColor = Qt::white;

            // Подсветка текущей ячейки
            if (i == highlightI && j == highlightJ) {
                bgColor = QColor(255, 200, 0); // Жёлтая (сейчас проверяем)
                textColor = Qt::black;
            } else if (i == highlightK || j == highlightK) {
                bgColor = QColor(100, 100, 200); // Синяя (связана с k)
            }

            // Рисуем ячейку
            m_scene->addRect(startX + j * cellSize, startY + i * cellSize,
                           cellSize - 2, cellSize - 2,
                           QPen(Qt::white), QBrush(bgColor));

            // Текст в ячейке
            QGraphicsTextItem* cellText = m_scene->addText(text);
            cellText->setFont((i == highlightI && j == highlightJ) ? fontBold : fontMatrix);
            cellText->setDefaultTextColor(textColor);

            QRectF rect = cellText->boundingRect();
            cellText->setPos(startX + j * cellSize + (cellSize - rect.width()) / 2,
                           startY + i * cellSize + (cellSize - rect.height()) / 2 - 2);
        }
    }
}

void AlgorithmVisualizer::setGraph(const Graph* newGraph) {
    m_graph = newGraph;
}

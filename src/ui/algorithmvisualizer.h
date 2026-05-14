#ifndef ALGORITHMVISUALIZER_H
#define ALGORITHMVISUALIZER_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include "Graphs.h"
#include "GraphAlgorithms.h"
class AlgorithmVisualizer
{
public:
    AlgorithmVisualizer(QGraphicsScene* scene, const Graph* graph);
private:
    QGraphicsScene* m_scene;
    const Graph* m_graph;

};

#endif // ALGORITHMVISUALIZER_H

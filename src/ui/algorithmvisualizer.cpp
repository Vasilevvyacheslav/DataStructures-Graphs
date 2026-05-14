#include "algorithmvisualizer.h"
#include <QGraphicsScene>
AlgorithmVisualizer::AlgorithmVisualizer(QGraphicsScene* scene, const Graph* graph)
    : m_scene(scene), m_graph(graph)
{
}

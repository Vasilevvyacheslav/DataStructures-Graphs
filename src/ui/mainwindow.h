#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Graphs.h"
#include "GraphAlgorithms.h"
#include "algorithmvisualizer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddVertex();
    void onAddEdge();

    void onRunBFS();
    void onRunDFS();
    void onRunDijkstra();
    void onRunFloyd();

    void onClearGraph();
    void onReset();

    void onStepBFS();
    void onStepDFS();
    void onStepDijkstra();
    void onStepFloyd();

    void onMatrixCellChanged(int row, int column);

private:
    void updateMatrix();
    void showResult(const QString& title, const std::vector<int>& data);
    void onRemoveVertex();
    void onRemoveEdge();
    void showFloydMatrix(const FloydStep& step);

private:
    AlgorithmVisualizer* visualizer;
    Ui::MainWindow *ui;
    Graph* graph;
    vector<BFSStep> bfsSteps;
    vector<DFSStep> dfsSteps;
    int currentBFSIndex;
    int currentDFSIndex;
    vector<DijkstraStep> dijkstraSteps;
    int currentDijkstraIndex;
    vector<FloydStep> floydSteps;
    int currentFloydIndex;
};

#endif // MAINWINDOW_H

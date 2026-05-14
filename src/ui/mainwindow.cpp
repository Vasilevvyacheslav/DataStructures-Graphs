#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GraphAlgorithms.h"
#include <algorithm>
#include <QInputDialog>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QTableWidgetItem>
#include <QPainterPath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    graph = new Graph();

    // Создаем сцену для рисования графа
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    visualizer = new AlgorithmVisualizer(ui->graphicsView->scene(), graph);

    // Подключаем кнопки к слотам
    // Добавление вершин/ребёр
    connect(ui->btnAddVertex, &QPushButton::clicked, this, &MainWindow::onAddVertex);
    connect(ui->btnAddEdge, &QPushButton::clicked, this, &MainWindow::onAddEdge);

    //Алгоритмы BFS/DFS/Dijkstra/Floyd
    connect(ui->btnBFS, &QPushButton::clicked, this, &MainWindow::onRunBFS);
    connect(ui->btnDFS, &QPushButton::clicked, this, &MainWindow::onRunDFS);
    connect(ui->btnDijkstra, &QPushButton::clicked, this, &MainWindow::onRunDijkstra);
    connect(ui->btnFloyd, &QPushButton::clicked, this, &MainWindow::onRunFloyd);

    // Удаление вершин/ребёр
    connect(ui->btnRemoveVertex, &QPushButton::clicked, this, &MainWindow::onRemoveVertex);
    connect(ui->btnRemoveEdge, &QPushButton::clicked, this, &MainWindow::onRemoveEdge);
    // Удаление всего графа
    connect(ui->btnClearGraph, &QPushButton::clicked, this, &MainWindow::onClearGraph);
    // Пошаговый BFS/DFS/Dijkstra/Floyd
    connect(ui->btnStepBFS, &QPushButton::clicked, this, &MainWindow::onStepBFS);
    connect(ui->btnStepDFS, &QPushButton::clicked, this, &MainWindow::onStepDFS);
    connect(ui->btnStepDijkstra, &QPushButton::clicked, this, &MainWindow::onStepDijkstra);
    connect(ui->btnStepFloyd, &QPushButton::clicked, this, &MainWindow::onStepFloyd);
    // Сброс алгоритмов
    connect(ui->btnReset, &QPushButton::clicked, this, &MainWindow::onReset);
    // Взаимодействие с матрицей
    connect(ui->tableMatrix, &QTableWidget::cellChanged,
            this, &MainWindow::onMatrixCellChanged);
    ui->textEditOutput->setFont(QFont("Consolas", 10));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete graph;
    delete visualizer;
}

void MainWindow::onAddVertex() {
    int val = ui->spinBoxVertex->value();
    if (graph->InsertVertex(val)) {
        updateMatrix();
        visualizer->drawGraph();
        ui->textEditOutput->appendPlainText(QString("Добавлена вершина: %1").arg(val));
    } else {
        QMessageBox::warning(this, "Ошибка", "Граф полон или вершина уже существует!");
    }
}

void MainWindow::onAddEdge() {
     // Считываем значения из полей
    int v1 = ui->spinBoxFrom->value();
    int v2 = ui->spinBoxTo->value();
    int w = ui->spinBoxWeight->value();

    // Проверка: нельзя добавить ребро из вершины в саму себя
    if (v1 == v2) {
        QMessageBox::warning(this, "Ошибка", "Нельзя соединить вершину саму с собой!");
        return;
    }
    if (graph->InsertEdge(v1, v2, w)) {
        updateMatrix();
        visualizer->drawGraph();
        ui->textEditOutput->appendPlainText(QString("Добавлено ребро %1-%2 (вес %3)").arg(v1).arg(v2).arg(w));
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить ребро (вершины не найдены).");
    }
}

// Обновление таблицы матрицы смежности
void MainWindow::updateMatrix() {
    // Отключаем сигнал перед программным изменением
    disconnect(ui->tableMatrix, &QTableWidget::cellChanged,
                   this, &MainWindow::onMatrixCellChanged);

    int n = graph->GetAmountVerts();
    ui->tableMatrix->setRowCount(n);
    ui->tableMatrix->setColumnCount(n);

    // Заголовки строк и столбцов (значения вершин)
    for (int i = 0; i < n; ++i) {
        int val = graph->GetVertex(i);
        QString sVal = QString::number(val);
        ui->tableMatrix->setHorizontalHeaderItem(i, new QTableWidgetItem(sVal));
        ui->tableMatrix->setVerticalHeaderItem(i, new QTableWidgetItem(sVal));
    }

    // Заполнение ячеек
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            QTableWidgetItem *item = new QTableWidgetItem();

            if (i == j) {
                // Диагональ: всегда 0, редактирование запрещено
                item->setText("0");
                item->setTextAlignment(Qt::AlignCenter);
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);

                // Чуть затемняем фон, чтобы было видно, что ячейка заблокирована
                item->setBackground(QColor(45, 45, 45));
            } else {
                // Обычные ячейки
                int v1 = graph->GetVertex(i);
                int v2 = graph->GetVertex(j);
                int weight = graph->GetWeight(v1, v2);
                item->setText(QString::number(weight));
                item->setTextAlignment(Qt::AlignCenter);
            }
            ui->tableMatrix->setItem(i, j, item);
        }
    }
    // Включаем сигнал обратно
    connect(ui->tableMatrix, &QTableWidget::cellChanged,
                this, &MainWindow::onMatrixCellChanged);
}

void MainWindow::onRunBFS() {
    int start = ui->spinBoxStart->value();

    // Генерируем все шаги
    bfsSteps = GraphAlgorithms::GenerateBFSSteps(*graph, start);
    currentBFSIndex = 0;

    if (bfsSteps.empty()) {
        QMessageBox::warning(this, "Ошибка", "Вершина не найдена в графе");
        return;
    }

    ui->textEditOutput->clear();
    ui->btnStepBFS->setEnabled(true);// Активируем кнопку шага
    visualizer->drawBFSStep(bfsSteps[0]);// Показываем 0-й шаг (старт)
    ui->textEditOutput->appendPlainText(QString::fromStdString(bfsSteps[0].logText));
}

void MainWindow::onStepBFS() {
    currentBFSIndex++;

    if (currentBFSIndex < bfsSteps.size()) {
        visualizer->drawBFSStep(bfsSteps[currentBFSIndex]);
        ui->textEditOutput->appendPlainText(QString::fromStdString(bfsSteps[currentBFSIndex].logText));
    } else {
        ui->btnStepBFS->setEnabled(false);// Отключаем, когда шаги кончились
        ui->textEditOutput->appendPlainText("BFS завершён");
    }
}

void MainWindow::onRunDFS() {
    int start = ui->spinBoxStart->value();

    // Генерируем шаги для анимации
    dfsSteps = GraphAlgorithms::GenerateDFSSteps(*graph, start);
    currentDFSIndex = 0;

    if (dfsSteps.empty()) {
        QMessageBox::warning(this, "Ошибка", "Вершина не найдена");
        return;
    }

    ui->textEditOutput->clear();
    ui->btnStepDFS->setEnabled(true); // Активируем кнопку "Шаг"
    visualizer->drawDFSStep(dfsSteps[0]);         // Показываем первый кадр
    ui->textEditOutput->appendPlainText(QString::fromStdString(dfsSteps[0].logText));
}

void MainWindow::onStepDFS() {
    currentDFSIndex++;

    if (currentDFSIndex < dfsSteps.size()) {
        visualizer->drawDFSStep(dfsSteps[currentDFSIndex]);
        ui->textEditOutput->appendPlainText(QString::fromStdString(dfsSteps[currentDFSIndex].logText));
    } else {
        ui->btnStepDFS->setEnabled(false);
        ui->textEditOutput->appendPlainText("DFS завершён");
    }
}

void MainWindow::onRunDijkstra() {
    int start = ui->spinBoxStart->value();

    if (graph->GetVertPos(start) == -1) {
        QMessageBox::warning(this, "Ошибка", "Вершина не найдена");
        return;
    }

    // Генерируем шаги
    dijkstraSteps = GraphAlgorithms::GenerateDijkstraSteps(*graph, start);
    currentDijkstraIndex = 0;

    if (dijkstraSteps.empty()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сгенерировать шаги");
        return;
    }

    ui->textEditOutput->clear();
    ui->btnStepDijkstra->setEnabled(true);

    // Показываем первый шаг
    visualizer->drawDijkstraStep(dijkstraSteps[0]);
    ui->textEditOutput->appendPlainText(QString::fromStdString(dijkstraSteps[0].logText));
}

void MainWindow::onStepDijkstra() {
    currentDijkstraIndex++;

    if (currentDijkstraIndex < dijkstraSteps.size()) {
        visualizer->drawDijkstraStep(dijkstraSteps[currentDijkstraIndex]);
        ui->textEditOutput->appendPlainText(
            QString::fromStdString(dijkstraSteps[currentDijkstraIndex].logText)
        );
    } else {
        ui->btnStepDijkstra->setEnabled(false);
        ui->textEditOutput->appendPlainText("Алгоритм Дейкстры завершён");
    }
}

void MainWindow::onRunFloyd() {
    floydSteps = GraphAlgorithms::GenerateFloydSteps(*graph);
    currentFloydIndex = 0;

    if (floydSteps.empty()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сгенерировать шаги");
        return;
    }

    ui->textEditOutput->clear();
    ui->btnStepFloyd->setEnabled(true);

    // Показываем первый шаг
    visualizer->drawFloydStep(floydSteps[0]);
    showFloydMatrix(floydSteps[0]);  //Выводим матрицу текстом
    ui->textEditOutput->appendPlainText(QString::fromStdString(floydSteps[0].logText));
}

void MainWindow::onStepFloyd() {
    currentFloydIndex++;

    if (currentFloydIndex < floydSteps.size()) {
        visualizer->drawFloydStep(floydSteps[currentFloydIndex]);
        showFloydMatrix(floydSteps[currentFloydIndex]);  // Выводим матрицу
        ui->textEditOutput->appendPlainText(
            QString::fromStdString(floydSteps[currentFloydIndex].logText)
        );
    } else {
        ui->btnStepFloyd->setEnabled(false);
        ui->textEditOutput->appendPlainText("Алгоритм Флойда-Уоршелла завершён");
    }
}
void MainWindow::showFloydMatrix(const FloydStep& step) {
    int n = step.dist.size();
    if (n == 0) return;

    // Заголовок
    QString header;
    if (step.k == -2) header = "\n=== ИТОГОВАЯ МАТРИЦА ===";
    else if (step.k == -1) header = "\n=== НАЧАЛЬНАЯ МАТРИЦА ===";
    else header = "\nШаг: k = " + QString::number(graph->GetVertex(step.k)) + " (промежуточная)";

    ui->textEditOutput->appendPlainText(header);

    // Настройки таблицы
    int colWidth = 6; // Ширина одной ячейки
    QString rowPrefix = "   |"; // Отступ для номеров строк

    // Рисуем шапку (номера столбцов)
    QString headerRow = rowPrefix;
    for (int j = 0; j < n; j++) {
        int realVal = graph->GetVertex(j);
        QString valStr = QString::number(realVal);
        headerRow += QString(valStr).rightJustified(colWidth, ' ');
    }
    ui->textEditOutput->appendPlainText(headerRow);

    // Рисуем разделительную линию
    QString line = "---+";
    line += QString("-").repeated(n * colWidth);
    ui->textEditOutput->appendPlainText(line);

    // Рисуем строки матрицы
    for (int i = 0; i < n; i++) {
        int realRowVal = graph->GetVertex(i);
        // Номер строки (выравнивание по правому краю, ширина 3)
        QString row = QString::number(realRowVal).rightJustified(3, ' ') + " |";

        for (int j = 0; j < n; j++) {
            int val = step.dist[i][j];
            QString cellText = (val >= 10000) ? " INF" : QString::number(val);

            // Если это текущая проверяемая ячейка — выделяем скобками
            if (i == step.i && j == step.j) {
                row += QString("[" + cellText + "]").rightJustified(colWidth, ' ');
            } else {
                // Обычная ячейка
                row += QString(" " + cellText + " ").rightJustified(colWidth, ' ');
            }
        }
        ui->textEditOutput->appendPlainText(row);
    }
    ui->textEditOutput->appendPlainText(""); // Пустая строка в конце
}

void MainWindow::onMatrixCellChanged(int row, int column) {
    //Диагональ — 0, не трогаем
    if (row == column) {
        ui->tableMatrix->item(row, column)->setText("0");
        return;
    }

    // Отключаем сигнал, чтобы программное изменение симметричной ячейки не вызвало новый вызов этой функции
    disconnect(ui->tableMatrix, &QTableWidget::cellChanged,
               this, &MainWindow::onMatrixCellChanged);

    QTableWidgetItem* item = ui->tableMatrix->item(row, column);
    if (!item) return;

    // Читаем новое значение
    bool ok;
    int newWeight = item->text().toInt(&ok);

    if (!ok || newWeight < 0) {
        QMessageBox::warning(this, "Ошибка", "Введите неотрицательное число!");
        // Откат
        item->setText("0");
    } else {
        // Обновляем граф
        int v1 = graph->GetVertex(row);
        int v2 = graph->GetVertex(column);

        // Обновляем ребро
        graph->InsertEdge(v1, v2, newWeight);

        // Обновляем зеркальную ячейку в таблице
        QTableWidgetItem* symmetricItem = ui->tableMatrix->item(column, row);
        if (symmetricItem) {
            symmetricItem->setText(QString::number(newWeight));
        }

        // Сообщение в лог
        ui->textEditOutput->appendPlainText(
            QString("Вес ребра %1-%2 установлен на %3").arg(v1).arg(v2).arg(newWeight)
        );
    }

    // Включаем сигнал обратно
    connect(ui->tableMatrix, &QTableWidget::cellChanged,
            this, &MainWindow::onMatrixCellChanged);

    // Перерисовка графа
    visualizer->drawGraph();
}

// Вспомогательная функция для вывода вектора
void MainWindow::showResult(const QString& title, const std::vector<int>& data) {
    QString text = title + ": ";
    for (int v : data) text += QString::number(v) + " ";
    ui->textEditOutput->setPlainText(text);
}

void MainWindow::onRemoveVertex() {
    int val = ui->spinBoxVertex->value(); // Берем номер из поля "Вершина"

    if (graph->RemoveVertex(val)) {
        updateMatrix();
        visualizer->drawGraph();
        ui->textEditOutput->appendPlainText(QString("Вершина №1 удалена.").arg(val));
    } else {
        QMessageBox::warning(this, "Ошибка", "Такой вершины нет в графе.");
    }
}

void MainWindow::onRemoveEdge() {
    int u = ui->spinBoxFrom->value();
    int v = ui->spinBoxTo->value();

    if (graph->RemoveEdge(u, v)) {
        updateMatrix();
        visualizer->drawGraph();
        ui->textEditOutput->appendPlainText(QString("Ребро №1-№2 удалено.").arg(u).arg(v));
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось удалить ребро (проверьте вершины).");
    }
}

void MainWindow::onClearGraph() {
    // Очищаем объект графа
    delete graph;
    graph = new Graph();

    // Обновляем ссылку в визуализаторе
    visualizer->setGraph(graph);
    // Очищаем сцену и рисуем пустой граф
    visualizer->drawGraph();

    // Очищаем таблицу и вывод
    ui->tableMatrix->clearContents();
    ui->tableMatrix->setRowCount(0);
    ui->tableMatrix->setColumnCount(0);


    // Сбрасываем спинбоксы в 0
    ui->spinBoxVertex->setValue(0);
    ui->spinBoxFrom->setValue(0);
    ui->spinBoxTo->setValue(0);
    ui->spinBoxWeight->setValue(0);
    ui->spinBoxStart->setValue(0);

    ui->textEditOutput->clear();
    ui->textEditOutput->appendPlainText("Граф очищен.");
}

void MainWindow::onReset() {
    // Сбрасываем состояние алгоритмов
    bfsSteps.clear(); currentBFSIndex = 0;
    dfsSteps.clear(); currentDFSIndex = 0;
    dijkstraSteps.clear(); currentDijkstraIndex = 0;
    floydSteps.clear(); currentFloydIndex = 0;
    // Отключаем все кнопки шагов
    ui->btnStepBFS->setEnabled(false);
    ui->btnStepDFS->setEnabled(false);
    ui->btnStepDijkstra->setEnabled(false);
    ui->btnStepFloyd->setEnabled(false);
    // Очищаем текстовый вывод
    ui->textEditOutput->clear();
    ui->textEditOutput->appendPlainText("Граф и визуализация сброшены.");

    // Рисуем чистый граф (без подсветки)
    visualizer->drawGraph();
}

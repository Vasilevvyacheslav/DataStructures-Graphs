#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GraphAlgorithms.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    graph = new Graph();
MainWindow::~MainWindow()
{
    delete ui;
    delete graph;
}
}

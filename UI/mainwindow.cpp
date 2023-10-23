#include "mainwindow.h"

#include <qboxlayout.h>
#include <qmenubar.h>
#include <qpushbutton.h>

#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      central_window(new QWidget(this)),
      splitter(new QSplitter(central_window)),
      left_widget(new LeftWidget(splitter)) {
    this->setFixedSize(1393, 762);
    this->setWindowTitle("Focus On Now");
    this->setMenuBar(new QMenuBar());
    this->menuBar()->addMenu("file");
    this->menuBar()->addMenu("option");

    splitter->setOrientation(Qt::Horizontal);

    QWidget *right_window = new QWidget(splitter);

    splitter->addWidget(left_widget);

    QHBoxLayout *right_hlayout = new QHBoxLayout(this->central_window);

    right_hlayout->addWidget(new QPushButton("three"));
    right_hlayout->addWidget(new QPushButton("four"));
    right_window->setLayout(right_hlayout);

    splitter->addWidget(right_window);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    splitter->setStyleSheet("QSplitter::handle{background-color: rgb(125, 125, 125)}");
    splitter->setHandleWidth(1);

    QVBoxLayout *hlayout = new QVBoxLayout(this->central_window);
    hlayout->addWidget(splitter);
    central_window->setLayout(hlayout);

    this->setCentralWidget(this->central_window);
}

MainWindow::~MainWindow() { delete central_window; }

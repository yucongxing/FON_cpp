#include "mainwindow.h"

#include <qboxlayout.h>
#include <qmenubar.h>
#include <qpushbutton.h>

#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      central_window(new QWidget(this)),
      splitter(new QSplitter(central_window)),
      left_widget(new LeftWidget(splitter)),
      right_widget(new QWidget(splitter)),
      start_cap(new QPushButton("start")),
      camera_worker(new CameraWorker(this)),
      show_frame1(new QLabel()),
      show_frame2(new QLabel())
{
    this->setWindowTitle("Focus On Now");
    this->setMenuBar(new QMenuBar());
    this->menuBar()->addMenu("file");
    this->menuBar()->addMenu("option");

    splitter->setOrientation(Qt::Horizontal);

    splitter->addWidget(left_widget);
    splitter->addWidget(right_widget);

    QVBoxLayout *right_hlayout = new QVBoxLayout(right_widget);

    QHBoxLayout *label_layout  = new QHBoxLayout();

    show_frame1->setFixedSize(500, 500);
    show_frame2->setFixedSize(500, 500);
    label_layout->addWidget(show_frame1);
    label_layout->addWidget(show_frame2);
    right_hlayout->addLayout(label_layout);

    right_hlayout->addWidget(start_cap);
    right_widget->setLayout(right_hlayout);

    splitter->setStretchFactor(splitter->indexOf(left_widget), 1);
    splitter->setStretchFactor(splitter->indexOf(right_widget), 3);

    splitter->setStyleSheet("QSplitter::handle{background-color: rgb(125, 125, 125)}");
    splitter->setHandleWidth(1);

    QHBoxLayout *hlayout = new QHBoxLayout(this->central_window);
    hlayout->addWidget(splitter);
    central_window->setLayout(hlayout);

    this->setCentralWidget(this->central_window);
    setSignalAndSlots();
}

void MainWindow::setSignalAndSlots() {
    connect(camera_worker, &CameraWorker::frameReady, this, &MainWindow::refreshFrame);
    connect(left_widget, &LeftWidget::startProcessSignal, camera_worker, &CameraWorker::startCapture);
    connect(left_widget, &LeftWidget::endProcessSignal,   camera_worker, &CameraWorker::stopCapture);
}

void MainWindow::refreshFrame(const QImage &img) {
    QPixmap px = QPixmap::fromImage(img).scaled(show_frame1->size(), Qt::KeepAspectRatio);
    show_frame1->setPixmap(px);
    show_frame2->setPixmap(px);
}

MainWindow::~MainWindow() {
    camera_worker->stopCapture();
    delete central_window;
}

#include "mainwindow.h"

#include <qboxlayout.h>
#include <qmenubar.h>

#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      central_window(new QWidget(this)),
      splitter(new QSplitter(central_window)),
      left_widget(new LeftWidget(splitter)),
      right_widget(new QWidget(splitter)),
      camera_worker(new CameraWorker(this)),
      show_frame1(new QLabel()),
      show_frame2(new QLabel()),
      score_overlay(new QLabel("--", show_frame2))
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

    show_frame1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    show_frame1->setMinimumSize(200, 150);
    show_frame1->setAlignment(Qt::AlignCenter);
    show_frame2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    show_frame2->setMinimumSize(200, 150);
    show_frame2->setAlignment(Qt::AlignCenter);
    label_layout->addWidget(show_frame1);
    label_layout->addWidget(show_frame2);
    right_hlayout->addLayout(label_layout);
    right_widget->setLayout(right_hlayout);

    splitter->setStretchFactor(splitter->indexOf(left_widget), 1);
    splitter->setStretchFactor(splitter->indexOf(right_widget), 3);

    splitter->setStyleSheet("QSplitter::handle{background-color: rgb(125, 125, 125)}");
    splitter->setHandleWidth(1);

    QHBoxLayout *hlayout = new QHBoxLayout(this->central_window);
    hlayout->addWidget(splitter);
    central_window->setLayout(hlayout);

    this->setCentralWidget(this->central_window);

    score_overlay->setStyleSheet(
        "background-color: rgba(0,0,0,160);"
        "color: white;"
        "padding: 4px 8px;"
        "border-radius: 4px;"
        "font-weight: bold;");
    score_overlay->adjustSize();
    score_overlay->move(8, 8);
    score_overlay->raise();

    setSignalAndSlots();
}

void MainWindow::setSignalAndSlots() {
    connect(camera_worker, &CameraWorker::frameReady,         this, &MainWindow::refreshFrame);
    connect(camera_worker, &CameraWorker::focusScoreUpdated,  this, &MainWindow::onFocusScoreUpdated);
    connect(left_widget,   &LeftWidget::startProcessSignal,   camera_worker, &CameraWorker::startCapture);
    connect(left_widget,   &LeftWidget::endProcessSignal,     camera_worker, &CameraWorker::stopCapture);
}

void MainWindow::refreshFrame(const QImage &original, const QImage &analyzed) {
    show_frame1->setPixmap(QPixmap::fromImage(original).scaled(show_frame1->size(), Qt::KeepAspectRatio));
    show_frame2->setPixmap(QPixmap::fromImage(analyzed).scaled(show_frame2->size(), Qt::KeepAspectRatio));
}

void MainWindow::onFocusScoreUpdated(int score) {
    score_overlay->setText(QString("专注度 %1%").arg(score));
    score_overlay->adjustSize();
}

MainWindow::~MainWindow() {
    camera_worker->stopCapture();
    delete central_window;
}

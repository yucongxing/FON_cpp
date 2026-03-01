#ifndef UI_MAINWINDOWN_H
#define UI_MAINWINDOWN_H

#include <QMainWindow>
#include <QSplitter>
#include <QWidget>

#include "cameraWorker.h"
#include "leftWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

public slots:
    void refreshFrame(const QImage &original, const QImage &analyzed);
    void onFocusScoreUpdated(int score);

private:
    QWidget      *central_window;
    QSplitter    *splitter{nullptr};
    LeftWidget   *left_widget;
    QWidget      *right_widget;
    QLabel       *show_frame1;
    QLabel       *show_frame2;
    QLabel       *score_overlay;
    CameraWorker *camera_worker;

    void setSignalAndSlots();
};

#endif  // !UI_MAINWINDOWN_H

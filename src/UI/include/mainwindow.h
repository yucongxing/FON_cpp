#ifndef UI_MAINWINDOWN_H
#define UI_MAINWINDOWN_H

#include <QMainWindow>
#include <QPushButton>
#include <QSplitter>
#include <QTimer>
#include <QWidget>

#include "cameraThead.h"
#include "leftWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;
signals:

public slots:
    void refreshFrame(const QImage &img);

private:
    QWidget     *central_window;
    QSplitter   *splitter{nullptr};
    LeftWidget  *left_widget;
    QWidget     *right_widget;
    QPushButton *start_cap;
    QLabel *show_frame1;
    QLabel *show_frame2;

    QTimer      *timer_show_frame;
    CameraThead *camera_thead;

    void         setSignalAndSlots();
};

#endif  // !UI_MAINWINDOWN_H

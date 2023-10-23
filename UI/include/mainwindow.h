#ifndef UI_MAINWINDOWN_H
#define UI_MAINWINDOWN_H

#include <qmainwindow.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qtextlayout.h>
#include <qwidget.h>

#include "leftWidget.h"

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    QWidget    *central_window{this};
    QSplitter  *splitter{nullptr};
    LeftWidget *left_widget;

};

#endif  // !UI_MAINWINDOWN_H

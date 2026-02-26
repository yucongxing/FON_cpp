#pragma once

#include <qlabel.h>
#include <qlcdnumber.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qwidget.h>
#include <QObject>

class LeftWidget : public QWidget {
    Q_OBJECT
public:
    LeftWidget(QWidget *parent);

signals:
    void startProcessSignal();
    void endProcessSignal();

private:
    QLabel    *task_name;
    QLabel    *task_time;
    QLineEdit *task_name_edit;
    QLineEdit *task_time_edit_min;
    QLineEdit *task_time_edit_sec;

    QPushButton *p_start;
    QPushButton *p_end;
    QPushButton *p_pause;
    bool m_pause;

    QTimer *timer;
    QLCDNumber *lcd;

    int val;

    void onEnd();
    void exchangeText();
};

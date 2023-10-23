#include "leftWidget.h"

#include <qboxlayout.h>
#include <qdebug.h>
#include <qlabel.h>
#include <qlcdnumber.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qtextlayout.h>
#include <qtimer.h>
#include <qvalidator.h>
#include <qwidget.h>

LeftWidget::LeftWidget(QWidget *patrent)
    : QWidget(patrent),
      task_name(new QLabel("专注任务", this)),
      task_time(new QLabel("专注时间", this)),

      task_name_edit(new QLineEdit(this)),
      task_time_edit_min(new QLineEdit(this)),
      task_time_edit_sec(new QLineEdit(this)),

      p_start(new QPushButton("开始", this)),
      p_end(new QPushButton("停止", this)),
      p_pause(new QPushButton("暂停", this)) {
    // UI

    QVBoxLayout *layout           = new QVBoxLayout(this);

    QHBoxLayout *task_name_layout = new QHBoxLayout();
    task_name_layout->addWidget(task_name);

    task_name_edit->setPlaceholderText("任务名称");
    task_name_layout->addWidget(task_name_edit);
    layout->addLayout(task_name_layout);

    QHBoxLayout *task_time_layout = new QHBoxLayout();
    task_time_layout->addWidget(task_time);
    task_time_edit_min->setInputMask("0009");
    task_time_edit_min->setValidator(new QIntValidator(0, 24 * 60, task_time_edit_min));
    task_time_edit_sec->setInputMask("09");
    task_time_edit_sec->setValidator(new QIntValidator(0, 60, task_time_edit_sec));

    task_time_layout->addWidget(task_time_edit_min);
    task_time_layout->addWidget(new QLabel(":", this));
    task_time_layout->addWidget(task_time_edit_sec);
    layout->addLayout(task_time_layout);

    layout->addWidget(new QLabel("倒计时"), 0, Qt::AlignHCenter);

    lcd = new QLCDNumber(5, this);
    layout->addWidget(lcd);

    layout->addWidget(p_start);
    layout->addWidget(p_pause);
    layout->addWidget(p_end);

    // logic
    val   = 10;
    timer = new QTimer(this);
    timer->setInterval(1000);
    lcd->display("00:00");
    p_pause->setEnabled(false);
    p_end->setEnabled(false);
    m_stop  = true;
    m_pause = false;

    connect(p_start, &QPushButton::clicked, this, [&]() {
        QString min_str = task_time_edit_min->text();
        QString sec_str = task_time_edit_min->text();
        if (min_str.isEmpty() && sec_str.isEmpty()) {
            this->val = 120;
        } else {
            int min_num = min_str.toInt();
            int sec_num = min_str.toInt();
            this->val   = min_num * 60 + sec_num;
        }

        this->timer->start();

        this->p_start->setEnabled(false);
        this->p_end->setEnabled(true);
        this->p_pause->setEnabled(true);
    });

    connect(p_pause, &QPushButton::clicked, this, [&]() {
        if (!this->m_pause) {
            this->timer->stop();
            this->m_pause = true;
        } else {
            this->timer->setInterval(1000);
            this->timer->start();
            this->m_pause = false;
        }
        exchangeText();
    });

    connect(p_end, &QPushButton::clicked, this, &LeftWidget::onEnd);

    connect(timer, &QTimer::timeout, this, [&]() {
        this->val--;

        QString min_str = QString::number(val / 60);
        QString sec_str = QString::number(val % 60);
        min_str         = min_str.length() == 1 ? "0" + min_str : min_str;
        sec_str         = sec_str.length() == 1 ? "0" + sec_str : sec_str;
        this->lcd->display(min_str + ":" + sec_str);

        if (val == 0) {
            onEnd();
        }
    });
}

void LeftWidget::exchangeText() {
    if (this->m_pause) {
        this->p_pause->setText("继续");
    } else {
        this->p_pause->setText("暂停");
    }
}

void LeftWidget::onEnd() {
    m_pause = false;
    exchangeText();

    m_stop = true;
    p_start->setEnabled(true);
    p_pause->setEnabled(false);
    p_end->setEnabled(false);

    timer->stop();
    lcd->display("00:00");
    QMessageBox *mes = new QMessageBox(QMessageBox::Information,"result" ,"process is done!", QMessageBox::NoButton, this);
    mes->show();
}

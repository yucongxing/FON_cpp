#include "loginDialog.h"

#include <qformlayout.h>
#include <qguiapplication.h>
#include <qlabel.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qscreen.h>
#include <qwidget.h>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent),
      loginBtn(new QPushButton("Login", this)),
      m_usernameLineEdit(new QLineEdit(this)),
      m_userpasswordLineEdit(new QLineEdit(this)) {
    this->setWindowTitle("Login");
    QFormLayout *layout = new QFormLayout(this);
    layout->addRow(new QLabel("Username:"), m_usernameLineEdit);
    layout->addRow(new QLabel("Password:"), m_userpasswordLineEdit);
    m_userpasswordLineEdit->setEchoMode(QLineEdit::Password);
    layout->addRow(loginBtn);
    QObject::connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::login);
    this->moveToCenter();
}

void LoginDialog::moveToCenter() {
    auto screen   = QGuiApplication::primaryScreen()->geometry();
    auto win_size = this->geometry();
    int  new_left = (screen.width() - win_size.width()) / 2;
    int  new_top  = (screen.height() - win_size.height()) / 2;
    this->move(new_left, new_top);
}

LoginDialog::~LoginDialog() {}

void LoginDialog::login() {
    emit loginSuccess();
    this->close();
}

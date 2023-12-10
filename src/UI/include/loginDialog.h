#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qwidget.h>

#include "mainwindow.h"

class LoginDialog : public QDialog {
public:
  LoginDialog(QWidget *parent = nullptr);
  void setMainWindown(MainWindow *mw);

  ~LoginDialog() override;

private:
  QPushButton *loginBtn;
  QLineEdit *m_usernameLineEdit;
  QLineEdit *m_userpasswordLineEdit;

  void moveToCenter();
  void login();
};

#endif // !UI_LOGINDIALOG_H

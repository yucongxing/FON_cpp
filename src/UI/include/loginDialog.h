#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qwidget.h>

class LoginDialog : public QDialog {
    Q_OBJECT
public:
  LoginDialog(QWidget *parent = nullptr);

  ~LoginDialog() override;

signals:
  void loginSuccess();

private:
  QPushButton *loginBtn;
  QLineEdit *m_usernameLineEdit;
  QLineEdit *m_userpasswordLineEdit;

  void moveToCenter();
  void login();
};

#endif // !UI_LOGINDIALOG_H

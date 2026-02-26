#include <QApplication>

#include "mainwindow.h"
#include "loginDialog.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  LoginDialog login{};
  QObject::connect(&login, &LoginDialog::loginSuccess, [&]() {
    MainWindow *w = new MainWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
  });
  login.show();
  return app.exec();
}

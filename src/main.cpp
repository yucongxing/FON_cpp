#include <QApplication>
#include <QtWidgets>
#include <QLabel>

#include "mainwindow.h"
#include "loginDialog.h"
int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  LoginDialog login{};
  login.show();
  return app.exec();
}

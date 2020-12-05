#include "GUI/MainWindow.h"
#include <QApplication>
#include <locale.h>

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "en_GB");
  setlocale(LC_NUMERIC, "C");

  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}

#include "GUI/MainWindow.h"
#include <QApplication>
#include <locale.h>
#include <QStyle>

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "en_GB");
  setlocale(LC_NUMERIC, "C");
  srand((unsigned)time(NULL));

  QApplication a(argc, argv);

  QPalette lightPalette;
  lightPalette.setColor(QPalette::Window, QColor(240, 240, 240));
  lightPalette.setColor(QPalette::WindowText, Qt::black);
  lightPalette.setColor(QPalette::Base, Qt::white);
  lightPalette.setColor(QPalette::AlternateBase, QColor(233, 233, 233));
  lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
  lightPalette.setColor(QPalette::ToolTipText, Qt::black);
  lightPalette.setColor(QPalette::Text, Qt::black);
  lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));
  lightPalette.setColor(QPalette::ButtonText, Qt::black);
  lightPalette.setColor(QPalette::BrightText, Qt::red);
  lightPalette.setColor(QPalette::Link, QColor(42, 130, 218));
  lightPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
  lightPalette.setColor(QPalette::HighlightedText, Qt::white);

  QApplication::setPalette(lightPalette);

  MainWindow w;
  w.show();

  return a.exec();
}

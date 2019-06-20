// Qt
#include <QApplication>

// Local
#include "MainWindow.h"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  MainWindow w;
  w.show();
  return a.exec();
}

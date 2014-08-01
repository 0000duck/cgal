#include "MainWindow.h"

#if QT_VERSION >= 0x050000
#include <QApplication>
#else
#include <QtGui/QApplication>
#endif

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  
  w.ui->viewer->restoreStateFromFile();
  
  w.show();

  a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
  a.connect(w.ui->actionExit, SIGNAL(triggered()), &a, SLOT(quit()));
  
  return a.exec();
}

// main.cpp

#include <QApplication>
#include "HPDF.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  HPDF hpdf;
  hpdf.show();
  return app.exec();
}


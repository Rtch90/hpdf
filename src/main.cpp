// main.cpp

#include <QApplication>
#include "PDFFactory.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  PDFFactory pdffactory;
  pdffactory.show();
  return app.exec();
}


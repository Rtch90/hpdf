#include <QApplication>
#include "pdf_factory.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  PDFFactory pdffactory;
  pdffactory.show();

  return a.exec();
}

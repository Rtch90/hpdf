#pragma once
#include <QObject>
#include <poppler-qt5.h>

class QString;
class QImage;

class PDFFile : public QObject {
  Q_OBJECT
public:
  PDFFile(const QString& fileName);
  QImage* getPageImage(int pageNumber);

private:
  Poppler::Document* document;
  QString fileName;
};


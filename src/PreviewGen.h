#pragma once
#include <QThread>
#include <QMutex>
#include <poppler-qt5.h>
#include <QImage>
#include "PDFPreviewWidget.h"

class QImage;

class PreviewGen: public QThread
{
  Q_OBJECT

public:
  PreviewGen();
  void render(Poppler::Page*,double,Poppler::Page::Rotation);

protected:
  void run();
  Poppler::Page* pPage;
  double dpi;
  Poppler::Page::Rotation rotation;
  QMutex mutex;

public:
  double calcDpi(Poppler::Page* pdfPage, double dpi);

signals:
  void updatePreview(QImage);
};

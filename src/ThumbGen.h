#pragma once

#include <QThread>
#include <QMutex>
#include <poppler-qt5.h>
#include <QImage>
class PDFPageWidget;
class QImage;

class ThumbGen: public QThread
{
  Q_OBJECT

public:
  ThumbGen();
  void render(PDFPageWidget*,Poppler::Page*);

protected:
  void run();
  QVector<PDFPageWidget*> pWidgets;
  QVector<Poppler::Page*> pPages;

  QMutex mutex;

public:
  double calcDpi(Poppler::Page *pdfPage, QSize targetSize);

signals:
  void updateThumbnail(QImage, PDFPageWidget*);
};

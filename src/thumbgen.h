#pragma once
#include <QThread>
#include <QMutex>
#include <QImage>
#include <poppler-qt5.h>

class PDFPageWidget;
class QImage;

class ThumbGen : public QThread {
  Q_OBJECT
public:
  ThumbGen(void);
  void render(PDFPageWidget*, Poppler::Page*);

protected:
  void run(void);
  QVector<PDFPageWidget*> pWidgets;
  QVector<Poppler::Page*> pPages;

  QMutex mutex;

private:
  double calcDpi(Poppler::Page* pdfPage, QSize targetSize);

signals:
  void updateThumbnail(QImage, PDFPageWidget*);

};


#pragma once
#include <QWidget>
#include <poppler-qt5.h>

class QImage;
class QSize;
class QPoint;
class QWheelEvent;
class QPaintEvent;
class QMouseEvent;

class PDFPreviewWidget : public QWidget {
  Q_OBJECT
public:
  explicit PDFPreviewWidget(QWidget* parent = 0);

public slots:
  void regenImage(void);
  void previewUpdate(Poppler::Page*);

protected:
  void wheelEvent(QWheelEvent* event);
  void paintEvent(QPaintEvent* event);

private:
  QImage previewImage;
  Poppler::Page* pPage;
 
  QSize currentPixmapSize;
  QPoint currentPixmapPos;

signals:
  void updatePreview(QImage);
};


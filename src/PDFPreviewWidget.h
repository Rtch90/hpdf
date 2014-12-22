#pragma once

#include <QFrame>
#include <poppler-qt5.h>
#include "PreviewGen.h"


class QImage;
class QPixmap;
class QSize;
class QPoint;
class QResizeEvent;
class QWheelEvent;
class QPaintEvent;
class QMouseEvent;
class PreviewGen;

class PDFPreviewWidget : public QFrame
{
  Q_OBJECT

public:
  explicit PDFPreviewWidget(QWidget* parent = 0);

public slots:
  void regenImage(void);
  void regenPixmap(void);
  void repositionPixmap(void);
  void previewUpdate(Poppler::Page*, Poppler::Page::Rotation);
  void checkPreviewUpdate(Poppler::Page*, Poppler::Page::Rotation);
  void checkPagePreviewExisted(Poppler::Page*);

protected:
  void wheelEvent(QWheelEvent* event);
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);

public:
  Poppler::Page* pPage;

private:
  QImage previewImage;
  QPixmap pixmap;
  Poppler::Page::Rotation rotation;

  QSize   currentPixmapSize;
  QPoint  currentPixmapPos;
  QPoint  lastPixmapPos;
  QPoint  dragStartPos;

private slots:
  void updateImage(QImage);

signals:
  void updatePreview(QImage);
};

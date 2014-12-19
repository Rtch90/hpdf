#pragma once
#include <QFrame>
#include <poppler-qt5.h>

class QImage;
class QPixmap;
class QSize;
class QPoint;
class QResizeEvent;
class QWheelEvent;
class QPaintEvent;
class QMouseEvent;

class PDFPreviewWidget : public QFrame {
  Q_OBJECT
public:
  explicit PDFPreviewWidget(QWidget* parent = 0);

public slots:
  void regenImage(void);
  void regenPixmap(void);
  void repositionPixmap(void);
  void previewUpdate(Poppler::Page*);

protected:
  void wheelEvent(QWheelEvent* event);
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);

private:
  QImage previewImage;
  QPixmap pixmap;
  Poppler::Page* pPage;
 
  QSize   currentPixmapSize;
  QPoint  currentPixmapPos;
  QPoint  lastPixmapPos;
  QPoint  dragStartPos;

signals:
  void updatePreview(QImage);
};


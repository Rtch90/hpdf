#pragma once
#include <QWidget>
#include <poppler-qt5.h>

class QImage;
class QSize;
class QResizeEvent;
class QPaintEvent;
class QMouseEvent;

class PDFPreviewWidget : public QWidget {
  Q_OBJECT
public:
  explicit PDFPreviewWidget(QWidget* parent = 0);

public slots:
  void setImage(QImage image);
  void regenImage(void);
  void previewUpdate(Poppler::Page*);

protected:
  void resizeEvent(QResizeEvent* event);
  void paintEvent(QPaintEvent* event);

private:
  QImage previewImage;
  Poppler::Page* pPage;
 
  QSize currentPixmapSize;

signals:
  void updatePreview(QImage);
};


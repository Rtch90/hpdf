#pragma once
#include <QWidget>
#include <poppler-qt5.h>

class QImage;
class QPaintEvent;
class QMouseEvent;

class PDFPreviewWidget : public QWidget {
  Q_OBJECT
public:
  explicit PDFPreviewWidget(QWidget* parent = 0);

public slots:
  void setImage(QImage image);
  void regenImage(void);
  void pageClicked(QMouseEvent* mouseEvent, QImage image);
  void previewUpdate(Poppler::Page*);

protected:
  void paintEvent(QPaintEvent* event);

private:
  QImage previewImage;
  void resizeEvent(QResizeEvent* event);
  /*PreviewGen tgen;*/
  Poppler::Page* pPage;
 
signals:
  void updatePreview(QImage);
};


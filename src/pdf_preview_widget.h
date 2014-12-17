#pragma once
#include <QWidget>

class QImage;
class QPaintEvent;
class QMouseEvent;

class PDFPreviewWidget : public QWidget {
  Q_OBJECT
public:
  explicit PDFPreviewWidget(QWidget* parent = 0);
  void setImage(QImage image);

public slots:
  void pageClicked(QMouseEvent* mouseEvent, QImage image);

protected:
  void paintEvent(QPaintEvent* event);

private:
  QImage previewImage;
};


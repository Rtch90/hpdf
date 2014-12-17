#pragma once
#include <QFrame>

class QWidget;
class QPushButton;
class QImage;
class QPixmap;
class QPaintEvent;
class QEvent;
class QSize;

class PDFPageWidget : public QFrame {
  Q_OBJECT
public:
  PDFPageWidget(QWidget* parent = 0);
  void setThumbnail(QImage* pageImage);
  void setButton(QPushButton* btn);
  QSize sizeHint() const;
  /*void setButtonImage(QImage* pageImage);*/

protected:
  void paintEvent(QPaintEvent* event);
  void enterEvent(QEvent* event);
  void leaveEvent(QEvent* event);

private:
  /*QVector<QPushButton> buttons;*/
  QPushButton* button;
  QImage* image;
  QPixmap pixmap;
  QPushButton* btn1, *btn2;
};


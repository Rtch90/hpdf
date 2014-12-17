#pragma once
#include <QFrame>
#include <poppler-qt5.h>

class QWidget;
class QPushButton;
class QImage;
class QPixmap;
class QPaintEvent;
class QEvent;
class QMouseEvent;
class QSize;

class PDFPageWidget : public QFrame {
  Q_OBJECT
public:
  PDFPageWidget(QWidget* parent = 0);
  void setThumbnail(QImage pageImage);
  void setPopplerPage(Poppler::Page*);

  void setButton(QPushButton* btn);
  QSize sizeHint() const;
  /*void setButtonImage(QImage* pageImage);*/

protected:
  void paintEvent(QPaintEvent* event);
  void enterEvent(QEvent* event);
  void leaveEvent(QEvent* event);
  void mousePressEvent(QMouseEvent* event);

private:
  /*QVector<QPushButton> buttons;*/
  QPushButton*    button;
  QImage          image;
  Poppler::Page*  pPage;
  QPixmap         pixmap;
  QPushButton*    btn1, *btn2;

signals:
  void pageClicked(QMouseEvent* event, QImage pageImage);
  void previewUpdate(Poppler::Page*);
};


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
  QSize sizeHint(void) const;

  void setThumbnail(QImage pageImage);
  void setPopplerPage(Poppler::Page*);
  void setAncestor(QWidget* ancestor);
  void setFather(QWidget* father);
  QWidget* getFather(void) const { return father; }
  void registerName(QString name) { path = name; }
  QString getName(void) { return path; }

  void setSelected(bool select);
  bool isSelected(void) { return selected; }

protected:
  void paintEvent(QPaintEvent* event);
  void enterEvent(QEvent* event);
  void leaveEvent(QEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void dragEnterEvent(QDragEnterEvent* event);
  void dropEvent(QDropEvent* event);

private:
  /*QVector<QPushButton> buttons;*/
  QString         path;
  QWidget*        ancestor;
  QWidget*        father;

  QImage          image;
  Poppler::Page*  pPage;
  QPixmap         pixmap;
  QPushButton*    btnRotate, *btnCut, *btnCopy;

  bool            selected;

signals:
  void pageClicked(PDFPageWidget*, QMouseEvent*, QString);
  void pageDropped(PDFPageWidget*, QDropEvent*, QString, QString);
  void previewUpdate(Poppler::Page*);
};


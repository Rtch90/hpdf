#pragma once

#include <poppler-qt5.h>
#include <QFrame>

class QWidget;
class QPushButton;
class QImage;
class QPixmap;
class QEvent;
class QMouseEvent;
class QPaintEvent;
class QDragEnterEvent;
class QDropEvent;
class QSize;
class QString;

class PDFPageWidget : public QFrame
{
  Q_OBJECT

public:
  PDFPageWidget(QWidget *parent = 0);
  QSize sizeHint() const;

  void setThumbnail(QImage pageImage);
  void setOriginInfo(QString path, int num) { pageNo = num;oriFilePath=path; }
  void setPopplerPage(Poppler::Page*);
  void setThumbPopplerPage(Poppler::Page*);
  void setAncestor(QWidget* ancestor);
  void setFather(QWidget* father);
  void rotate90(void);
  Poppler::Page::Rotation getRotation();
  int getIntRotation(void)        { return rotation;    }
  void setIntRotation(int r)      { rotation = r;       }
  QWidget* getFather(void) const  { return father;      }
  void registerName(QString name) { path = name;        }
  QString getName(void)           { return path;        }
  int getPageNo(void)             { return pageNo;      }
  QString getOriFilePath()        { return oriFilePath; }
  Poppler::Page* getNewThumbPopplerPage();

  void setSelected(bool select);
  bool isSelected(void) { return selected; }

  Poppler::Page* getPage(void)      { return previewPage; }
  Poppler::Page* getThumbPage(void) { return thumbPage;   }

protected:
  void paintEvent(QPaintEvent* event);
  void enterEvent(QEvent* event);
  void leaveEvent(QEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void dragEnterEvent(QDragEnterEvent* event);
  void dropEvent(QDropEvent* event);

private:
  //QVector<QPushButton> buttons;
  QString           path;
  int               pageNo;
  QString           oriFilePath;
  QWidget*          ancestor;
  QWidget*          father;

  QImage            image;
  Poppler::Page*    previewPage,*thumbPage;
  QPixmap           pixmap;
  QPushButton*      btnRotate, *btnCut, *btnCopy, *btnDelete;

  int             rotation;


  bool selected;

signals:
  void pageClicked(PDFPageWidget*, QMouseEvent*, QString);
  void pageDropped(PDFPageWidget*, QDropEvent*, QString, QString);

private slots:
  void pageDelete();
  void pageCopy();
  void pageRotate();
  void pageCut();
};

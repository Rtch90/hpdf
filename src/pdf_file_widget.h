#pragma once
#include <QWidget>
#include <QFrame>
#include <poppler-qt5.h>
#include "pdf_page_widget.h"
#include "thumbgen.h"

class QImage;
class QHBoxLayout;
class QGridLayout;
class QScrollArea;
class QPushButton;
class QLabel;
class QString;
class QSize;
class QPoint;
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
class QPaintEvent;
class ThumbGen;
class PDFPageWidget;

class PagesContainerWidget : public QWidget {
  Q_OBJECT
public:
  PagesContainerWidget(QWidget* parent = 0);
  QSize sizeHint() const;
  QVector<PDFPageWidget*> pageWidgets;
  QHBoxLayout*            mainLayout;

  void addPageWidget(PDFPageWidget* image);
  void setAncestor(QWidget* ancestor) {this->ancestor = ancestor; }

protected:
  void dragEnterEvent(QDragEnterEvent* event);
  void dropEvent(QDropEvent* event);

private:
  int getPagesCount() const;
  QWidget* ancestor;
};

class PDFFileWidget : public QFrame {
  Q_OBJECT
  Q_PROPERTY(bool collapsed READ isCollapsed WRITE setCollapsed)

public:
  PDFFileWidget(QWidget* parent = 0);

  void  setAncestor(QWidget* ancestor);
  void  setDocument(Poppler::Document* document, QString fileName);
  int   removeChild(PDFPageWidget* child);
  int   indexChild(PDFPageWidget* child);
  void  insertChildAt(PDFPageWidget* child, int pos);

  void setSelected(bool select);
  bool isSelected(void) { return selected; }
  bool isCollapsed(void) { return collapsed; }
  void setCollapsed(bool collapsed);

protected:
  void mousePressEvent(QMouseEvent* event);
  void paintEvent(QPaintEvent* event);

private slots:
  void removeButtonClicked(void);
  void collapsedButtonClicked(void);
  void updateThumbnail(QImage, PDFPageWidget*);

private:
  ThumbGen                tgen;
  QGridLayout*            topLayout;

  QLabel*                 fileNameLabel;
  QPushButton*            collapseButton;
  QPushButton*            removeButton;
  PagesContainerWidget*   pagesContainerWidget;
  QScrollArea*            scrollArea;
  QWidget*                ancestor;
  bool                    collapsed;
  bool                    selected;

signals:
  void fileRemoveButtonClicked(PDFFileWidget*);
  void fileClicked(PDFFileWidget*, QMouseEvent*);
};


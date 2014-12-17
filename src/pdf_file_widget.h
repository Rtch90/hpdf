#pragma once
#include <QWidget>
#include <poppler-qt5.h>
#include "pdf_page_widget.h"

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
class PDFPageWidget;

class FilesContainerWidget : public QWidget {
  Q_OBJECT
public:
  FilesContainerWidget(QWidget* parent = 0);
  QSize sizeHint() const;

  void addPageWidget(PDFPageWidget* image);

protected:
  void dragEnterEvent(QDragEnterEvent* event);
  void dropEvent(QDropEvent* event);
  void mousePressEvent(QMouseEvent* event);
private:
  QVector<PDFPageWidget*> pageWidgets;
  QHBoxLayout*            mainLayout;

  int findPageContainingClickEvent(QPoint pos);
  int findPageWidgetInLayout(PDFPageWidget* pageWidgets);
  int getPagesCount() const;
};

class PDFFileWidget : public QWidget {
  Q_OBJECT
  Q_PROPERTY(bool collapsed READ isCollapsed WRITE setCollapsed)

public:
  PDFFileWidget(QWidget* parent = 0);

  void setAncestor(QWidget* ancestor) { this->ancestor = ancestor; }
  void setDocument(Poppler::Document* document, QString fileName);

  bool isCollapsed(void) { return collapsed; }
  void setCollapsed(bool collapsed);

protected:
  

private slots:
  void collapsedButtonClick();

private:
  QGridLayout* topLayout;

  QLabel*                 fileNameLabel;
  QPushButton*            collapseButton;
  FilesContainerWidget*   filesContainerWidget;
  QScrollArea*            scrollArea;
  QWidget*                ancestor;
  bool                    collapsed;
};


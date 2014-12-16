#pragma once
#include <QWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QFrame>
#include "pdf_page_widget.h"
#include "pdf_file.h"

class QPushButton;
class QWidget;
class QScrollArea;

class TableView : public QWidget {
  Q_OBJECT
public:
  TableView(QWidget* parent = 0);
  QVector<PDFFile> files;

protected:
  
private:
  QScrollArea*  scrollArea;
  QVBoxLayout*  layout;
  QFrame*       frame;
  QVBoxLayout*  outerLayout;

  void paintEvent(QPaintEvent* event);
};


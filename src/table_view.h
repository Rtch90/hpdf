#pragma once
#include <QWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QFrame>
#include <poppler-qt5.h>
#include "pdf_page_widget.h"
#include "pdf_file_widget.h"

class QPushButton;
class QWidget;
class QScrollArea;

class TableView : public QWidget {
  Q_OBJECT
public:
  TableView(QWidget* parent = 0);

protected:
  
private:
  QScrollArea*  scrollArea;
  QVBoxLayout*  layout;
  QFrame*       frame;
  QVBoxLayout*  outerLayout;

  void paintEvent(QPaintEvent* event);
  void loadFile(QString fileName);

  QVector<Poppler::Document*> files;
  QVector<QString>            filenames;
  QVector<PDFFileWidget*>     fileWidgets;
};


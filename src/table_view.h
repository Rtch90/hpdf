#pragma once
#include <QWidget>
#include <poppler-qt5.h>
#include "pdf_page_widget.h"
#include "pdf_file_widget.h"

class QPushButton;
class QScrollArea;
class QVBoxLayout;
class QSplitter;
class QFrame;
class QPaintEvent;
class QString;

class PDFFileWidget;

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
  QVector<QString>            fileNames;
  QVector<PDFFileWidget*>     fileWidgets;
};


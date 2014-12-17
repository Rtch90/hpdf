#pragma once
#include <QFrame>
#include <poppler-qt5.h>

class QVBoxLayout;
class QString;

class PDFFileWidget;

class PDFTableView : public QFrame {
  Q_OBJECT
public:
  PDFTableView(QWidget* parent = 0);

protected:
  
private:
  QVBoxLayout*  outerLayout;

  void loadFile(QString fileName);

  QVector<Poppler::Document*> files;
  QVector<QString>            fileNames;
  QVector<PDFFileWidget*>     fileWidgets;
};


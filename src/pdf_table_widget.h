#pragma once
#include <QFrame>
#include <poppler-qt5.h>

class QVBoxLayout;
class QString;
class QScrollArea;

class PDFFileWidget;

class PDFTableWidget : public QFrame {
  Q_OBJECT
public:
  PDFTableWidget(QWidget* parent = 0);
  void loadFile(QString fileName);

protected:
  
private:
  QVBoxLayout*  outerLayout;

  QScrollArea* scrollArea;
  QWidget* containerWidget;
  QVBoxLayout* containerLayout;

  QVector<Poppler::Document*> files;
  QVector<QString>            fileNames;
  QVector<PDFFileWidget*>     fileWidgets;
};


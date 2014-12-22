#pragma once
#include <poppler-qt5.h>
#include "PDFJam.h"

#include <QFrame>
#include <QHash>
#include "PDFPageWidget.h"
#include "ThumbGen.h"

class QImage;
class QVBoxLayout;
class QString;
class QScrollArea;
class QMouseEvent;
class QPoint;

class PDFFileWidget;

class PDFTableWidget: public QFrame
{
  Q_OBJECT
public:
  PDFTableWidget(QWidget* parent = 0);
  void loadFile (QString fileName);
  void registerPage(PDFPageWidget *child);


public:
  PDFJam pdfJam;
  ThumbGen mainTgen;

private:
  QVBoxLayout*  outerLayout;
  QScrollArea*  scrollArea;
  QWidget*      containerWidget;
  QVBoxLayout*  containerLayout;

  QVector<Poppler::Document*> files;
  QVector<QString>            fileNames;
  QVector<PDFFileWidget*>     fileWidgets;

  QHash<QString, PDFPageWidget*> pageChilds;

signals:
  void previewUpdate(Poppler::Page*, Poppler::Page::Rotation);
  void checkPreviewUpdate(Poppler::Page*, Poppler::Page::Rotation);
  void checkPagePreviewExisted(Poppler::Page*);

private slots:
  void fileRemoveButtonClicked(PDFFileWidget*);
  void pageClicked(PDFPageWidget*, QMouseEvent*, QString);
  void pageDropped(PDFPageWidget*, QDropEvent*, QString, QString);

  void fileClicked(PDFFileWidget*, QMouseEvent*);

private:
  QVector<PDFPageWidget*> selectedPages;
  QVector<PDFFileWidget*> selectedFiles;

  QVector<PDFPageWidget*> copiedPages;

public:
  QVector<PDFFileWidget*> getSelectedFiles();
  QVector<int> getSelectedIndices();
  QVector<QString> getSelectedFileNames();

  QVector<PDFFileWidget*> getVisibleFiles();
  QVector<int> getVisibleIndices();
  QVector<QString> getVisibleFileNames();

public:
  void moveSelectedPages(QString, QString);
  void moveSelectedPages(QString, PDFPageWidget* page);
  void deletePage(PDFPageWidget* pageWidget);
  void deletePageSkipSelection(PDFPageWidget* pageWidget);
  void copyPage(PDFPageWidget* pageWidget);
  void pastePage(PDFFileWidget* fileWidget, int pageID);
  void rotatePage(PDFPageWidget* pageWidget);
  void cutPage(PDFPageWidget* pageWidget);
  void clearClipboard(void);
  bool hasClipboard(void) { return copiedPages.size() > 0; }

public slots:
  void rotateSelected();
  void copySelected();
  void cutSelected();
  void deleteSelected();
};

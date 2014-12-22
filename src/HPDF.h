#pragma once
#include <QMainWindow>

class PDFTableWidget;
class PDFPreviewWidget;

class QAction;
class QWidget;
class QTabWidget;
class QScrollArea;
class QSplitter;
class QToolBar;

class HPDF: public QMainWindow
{
  Q_OBJECT

public:
  HPDF(void);

private slots:
  void openFile(void);
  void exportFile(void);
  void exportAllFiles(void);
  void about(void);

private:
  void createWidgets(void);
  void createActions(void);
  void createToolBars(void);
  void createRibbon(void);
  void createStatusBar(void);

  QAction*  openAction;
  QAction*  exportAction;
  QAction*  exportAllAction;
  QAction*  cutAction;
  QAction*  copyAction;
  QAction*  pasteAction;
  QAction*  deleteAction;
  QAction*  rotateAction;
  QAction*  aboutAction;
  QAction*  searchAction;

  QWidget* centralWidget;

  QTabWidget*       ribbon;
  QSplitter*        splitter;
  PDFTableWidget*   pdfTableView;
  PDFPreviewWidget* pdfPreview;

  QToolBar*         fileToolBar;
  QToolBar*         editToolBar;
  QToolBar*         toolsToolBar;
  QToolBar*         helpToolBar;
};

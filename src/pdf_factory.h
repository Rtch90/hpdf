#pragma once
#include <QMainWindow>
#include "pdf_table_widget.h"

class QAction;
class QWidget;
class QTabWidget;
class QScrollArea;
class QSplitter;
class QToolBar;

class PDFFactory : public QMainWindow {
  Q_OBJECT
public:
  PDFFactory();

protected:
  /*void closeEvent(QCloseEvent* event);*/

private slots:
  void openFile(void);
  /*void exportFile(void);
  void exportAllFiles(void);*/

private:
  void createWidgets(void);
  void createActions(void);
  void createToolBars(void);
  void createRibbon();
  void createStatusBar();

  QAction* openAction;
  QAction* exportAction;
  QAction* exportAllAction;
  QAction* cutAction;
  QAction* copyAction;
  QAction* pasteAction;
  QAction* aboutAction;

  QWidget* centralWidget;

  QTabWidget*     ribbon;
  QScrollArea*    scrollArea;
  PDFTableWidget* pdfTableView;
  QSplitter*      splitter;
  QWidget*        pdfPreview;

  QToolBar* fileToolBar;
  QToolBar* editToolBar;
  QToolBar* helpToolBar;
};


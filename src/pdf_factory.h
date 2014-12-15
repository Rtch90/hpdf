#pragma once
#include <QMainWindow>

class QAction;
class QToolBar;
class QTabWidget;
class QWidget;

class PDFFactory : public QMainWindow {
  Q_OBJECT
public:
  PDFFactory();

protected:
  /*void closeEvent(QCloseEvent* event);*/

private slots:
  /*void openFile(void);
  void exportFile(void);
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

  QTabWidget* ribbon;

  QToolBar* fileToolBar;
  QToolBar* editToolBar;
};


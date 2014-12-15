#include <QtWidgets>
#include "pdf_factory.h"

PDFFactory::PDFFactory(void) {
  createWidgets();
  createActions();
  createToolBars();
  createRibbon();
  createStatusBar();
}

/* Create the shit we need for our window. */
void PDFFactory::createWidgets(void) {
  centralWidget = new QWidget();
  QVBoxLayout* layout = new QVBoxLayout();
  centralWidget->setLayout(layout);
  setCentralWidget(centralWidget);

  /* Create ribbon. */
  ribbon = new QTabWidget();
  ribbon->addTab(new QWidget(), tr("File"));
  ribbon->addTab(new QWidget(), tr("Edit"));
  ribbon->addTab(new QWidget(), tr("View"));
  ribbon->addTab(new QWidget(), tr("Help"));
  ribbon->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  ribbon->setFixedHeight(100);
  layout->addWidget(ribbon);

  /*setWindowIcon(QIcon(":/img/hpdf.png"));*/
  setGeometry(0, 0, 550, 650);
}

void PDFFactory::createActions(void) {
  openAction = new QAction(tr("&Open"), this);
  openAction->setIcon(QIcon(":/img/open.png"));
  openAction->setShortcut(tr("Ctrl+O"));
  openAction->setStatusTip(tr("Open a PDF"));
  connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

  exportAction = new QAction(tr("&Export"), this);
  exportAction->setIcon(QIcon(":/img/save.png"));
  exportAction->setShortcut(tr("Ctrl+S"));
  exportAction->setStatusTip(tr("Export the selected frame to a new PDF"));
  /*connect(saveAction, SIGNAL(triggered()), this, SLOT(save()))*/

  exportAllAction = new QAction(tr("Combine all and export"), this);
  exportAllAction->setIcon(QIcon(":/img/saveas.png"));
  exportAllAction->setShortcut(tr("Shift+Ctrl+S"));
  exportAllAction->setStatusTip(tr("Combine all and export as one PDF"));
  /*connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()))*/
  
  cutAction = new QAction(tr("C&ut"), this);
  cutAction->setIcon(QIcon(":/img/cut.png"));
  cutAction->setShortcut(tr("Ctrl+X"));
  cutAction->setStatusTip(tr("Cut selected contents to clipboard"));
  /*connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()))*/

  copyAction = new QAction(tr("&Copy"), this);
  copyAction->setIcon(QIcon(":/img/copy.png"));
  copyAction->setShortcut(tr("Ctrl+C"));
  copyAction->setStatusTip(tr("Copy selected contents to clipboard"));
  /*connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()))*/

  pasteAction = new QAction(tr("&Paste"), this);
  pasteAction->setIcon(QIcon(":/img/paste.png"));
  pasteAction->setShortcut(tr("Ctrl+V"));
  pasteAction->setStatusTip(tr("Paste clipboard's contents into current"
                               "selection"));
  /*connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()))*/
}

void PDFFactory::createToolBars() {
  fileToolBar = new QToolBar(tr("File"));
  fileToolBar->addAction(openAction);
  fileToolBar->addAction(exportAction);
  fileToolBar->addAction(exportAllAction);

  editToolBar = new QToolBar(tr("Edit"));
  editToolBar->addAction(cutAction);
  editToolBar->addAction(copyAction);
  editToolBar->addAction(pasteAction);
}

void PDFFactory::createRibbon(void) {
 QWidget* tabFile = ribbon->widget(0);
 QVBoxLayout* layoutTabFile = new QVBoxLayout();
 tabFile->setLayout(layoutTabFile);
 layoutTabFile->addWidget(fileToolBar);
}

void PDFFactory::createStatusBar(void) {
  statusBar()->showMessage(tr(""));
}


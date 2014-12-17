#include <QtWidgets>
#include <QtGlobal>
#include "pdf_factory.h"
#include "pdf_table_view.h"
#include "pdf_page_widget.h"

PDFFactory::PDFFactory(void) {
  createWidgets();
  createActions();
  createToolBars();
  createRibbon();
  createStatusBar();
}

/* Create the shit we need for our window. */
void PDFFactory::createWidgets(void) {
  /* Set central widget to be the container root. */
  centralWidget = new QWidget();
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(2, 2, 2, 2);
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

  /* Create main area (table). */
  pdfTableView = new PDFTableView();
  pdfTableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  splitter = new QSplitter();
  /* TODO: change pdfPreview widget. */
  pdfPreview = new QWidget();
  pdfPreview->setMinimumWidth(100);
  splitter->setOrientation(Qt::Horizontal);
  splitter->addWidget(pdfTableView);
  splitter->addWidget(pdfPreview);
  QList<int> splitterWidgetSizes;
  splitterWidgetSizes << 600 << 400;
  splitter->setSizes(splitterWidgetSizes);
  splitter->setStretchFactor(0, 1);
  splitter->setStretchFactor(1, 0.5);
  layout->addWidget(splitter);

  setWindowIcon(QIcon(":/img/hpdf.png"));
  setWindowTitle(tr("HPDF"));
  setGeometry(0, 0, 1000, 650);
}

void PDFFactory::createActions(void) {
  openAction = new QAction(tr("&Open"), this);
  openAction->setIcon(QIcon(":/img/open.png"));
  openAction->setShortcut(tr("Ctrl+O"));
  openAction->setStatusTip(tr("Open a PDF"));
  connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

  exportAction = new QAction(tr("&Export"), this);
  exportAction->setIcon(QIcon(":/img/export.png"));
  exportAction->setShortcut(tr("Ctrl+S"));
  exportAction->setStatusTip(tr("Export the selected frame to a new PDF"));
  /*connect(saveAction, SIGNAL(triggered()), this, SLOT(save()))*/

  exportAllAction = new QAction(tr("Combine all and export"), this);
  exportAllAction->setIcon(QIcon(":/img/exportall.png"));
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
  pasteAction->setStatusTip(tr("Paste clipboard's contents into current selection"));
  /*connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()))*/

  aboutAction = new QAction(tr("A&bout"), this);
  aboutAction->setIcon(QIcon(":/img/about.png"));
  aboutAction->setStatusTip(tr("About this program"));
}

void PDFFactory::createToolBars() {
  fileToolBar = new QToolBar(tr("File"));
  fileToolBar->addAction(openAction);
  fileToolBar->addAction(exportAction);
  fileToolBar->addAction(exportAllAction);
  fileToolBar->setIconSize(QSize(48, 48));

  editToolBar = new QToolBar(tr("Edit"));
  editToolBar->addAction(cutAction);
  editToolBar->addAction(copyAction);
  editToolBar->addAction(pasteAction);
  editToolBar->setIconSize(QSize(48, 48));

  helpToolBar = new QToolBar(tr("Help"));
  helpToolBar->addAction(aboutAction);
  helpToolBar->setIconSize(QSize(48, 48));
}

void PDFFactory::createRibbon(void) {
 QWidget* tabFile = ribbon->widget(0);
 QVBoxLayout* layoutTabFile = new QVBoxLayout();
 layoutTabFile->setContentsMargins(2, 0, 2, 0);
 layoutTabFile->addWidget(fileToolBar);
 tabFile->setLayout(layoutTabFile);

 QWidget* tabEdit = ribbon->widget(1);
 QVBoxLayout* layoutTabEdit = new QVBoxLayout();
 layoutTabEdit->setContentsMargins(2, 0, 2, 0);
 layoutTabEdit->addWidget(editToolBar);
 tabEdit->setLayout(layoutTabEdit);

 QWidget* tabHelp = ribbon->widget(3);
 QVBoxLayout* layoutTabHelp = new QVBoxLayout();
 layoutTabHelp->setContentsMargins(2, 0, 2, 0);
 layoutTabHelp->addWidget(helpToolBar);
 tabHelp->setLayout(layoutTabHelp);
}

void PDFFactory::createStatusBar(void) {
  statusBar()->showMessage(tr(""));
}


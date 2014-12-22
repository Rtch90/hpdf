#include <QtWidgets>
#include <QtGlobal>
#include "HPDF.h"
#include "PDFExportDialog.h"
#include "PDFTableWidget.h"
#include "PDFPreviewWidget.h"
#include "PDFPageWidget.h"

HPDF::HPDF(void) {
  createWidgets();
  createActions();
  createToolBars();
  createRibbon();
  createStatusBar();
}

void HPDF::createWidgets(void) {
  /* Set central widget to be the container root */
  centralWidget = new QWidget();
  QVBoxLayout *layout = new QVBoxLayout();
  layout->setContentsMargins(2,2,2,2);
  centralWidget->setLayout(layout);
  setCentralWidget(centralWidget);

  /*Create ribbon */
  ribbon = new QTabWidget();
  ribbon->addTab(new QWidget(), tr("File"));
  ribbon->addTab(new QWidget(), tr("Edit"));
  ribbon->addTab(new QWidget(), tr("Tools"));
  ribbon->addTab(new QWidget(), tr("Help"));
  ribbon->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  ribbon->setFixedHeight(100);
  layout->addWidget(ribbon);

  /* Create main area (table) */

  pdfTableView = new PDFTableWidget();
  pdfPreview = new PDFPreviewWidget();
  pdfPreview->setMinimumWidth(100);
  splitter = new QSplitter();
  splitter->setOrientation(Qt::Horizontal);
  splitter->addWidget(pdfTableView);
  splitter->addWidget(pdfPreview);
  QList<int> splitterWidgetSizes;
  splitterWidgetSizes << 200 << 1000;
  splitter->setSizes(splitterWidgetSizes);
  splitter->setStretchFactor(0, 1);
  splitter->setStretchFactor(1, 0.5);
  layout->addWidget(splitter);

  connect(pdfTableView, SIGNAL(previewUpdate(Poppler::Page*, Poppler::Page::Rotation)), pdfPreview, SLOT(previewUpdate(Poppler::Page*, Poppler::Page::Rotation)));
  connect(pdfTableView, SIGNAL(checkPreviewUpdate(Poppler::Page*, Poppler::Page::Rotation)), pdfPreview, SLOT(checkPreviewUpdate(Poppler::Page*, Poppler::Page::Rotation)));
  connect(pdfTableView, SIGNAL(checkPagePreviewExisted(Poppler::Page*)), pdfPreview, SLOT(checkPagePreviewExisted(Poppler::Page*)));

  setWindowIcon(QIcon(":/img/hpdf.png"));
  setWindowTitle(tr("PDF Factory"));
  setGeometry(0, 0, 1300, 650);
}


void HPDF::createActions(void) {
  openAction = new QAction(tr("&Open"), this);
  openAction->setIcon(QIcon(":/img/open.png"));
  openAction->setShortcut(tr("Ctrl+O"));
  openAction->setStatusTip(tr("Open a PDF"));
  connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

  exportAction = new QAction(tr("&Export a single file"), this);
  exportAction->setIcon(QIcon(":/img/export.png"));
  exportAction->setShortcut(tr("Ctrl+S"));
  exportAction->setStatusTip(tr("Export the selected file to a new PDF"));
  connect(exportAction, SIGNAL(triggered()), this, SLOT(exportFile()));

  exportAllAction = new QAction(tr("Export all"), this);
  exportAllAction->setIcon(QIcon(":/img/exportall.png"));
  exportAllAction->setShortcut(tr("Shift+Ctrl+S"));
  exportAllAction->setStatusTip(tr("Export all to multiple PDF files"));
  connect(exportAllAction, SIGNAL(triggered()), this, SLOT(exportAllFiles()));

  cutAction = new QAction(tr("C&ut"), this);
  cutAction->setIcon(QIcon(":/img/cut.png"));
  cutAction->setShortcut(tr("Ctrl+X"));
  cutAction->setStatusTip(tr("Cut selected contents to clipboard"));
  connect(cutAction, SIGNAL(triggered()), pdfTableView, SLOT(cutSelected()));

  copyAction = new QAction(tr("&Copy"), this);
  copyAction->setIcon(QIcon(":/img/copy.png"));
  copyAction->setShortcut(tr("Ctrl+C"));
  copyAction->setStatusTip(tr("Copy selected contents to clipboard"));
  connect(copyAction, SIGNAL(triggered()), pdfTableView, SLOT(copySelected()));

  pasteAction = new QAction(tr("&Paste"), this);
  pasteAction->setIcon(QIcon(":/img/paste.png"));
  pasteAction->setShortcut(tr("Ctrl+V"));
  pasteAction->setStatusTip(tr("Paste clipboard's contents into current selection"));
  //connect(pasteAction, SIGNAL(triggered()), textEdit, SLOT(paste()));

  deleteAction = new QAction(tr("&Delete"), this);
  deleteAction->setIcon(QIcon(":/img/delete.png"));
  deleteAction->setShortcut(tr("Ctrl+D"));
  deleteAction->setStatusTip(tr("Delete selected contents"));
  connect(deleteAction, SIGNAL(triggered()), pdfTableView, SLOT(deleteSelected()));

  rotateAction = new QAction(tr("&Rotate"), this);
  rotateAction->setIcon(QIcon(":/img/rotate.png"));
  rotateAction->setShortcut(tr("Ctrl+R"));
  rotateAction->setStatusTip(tr("Rotate selected pages"));
  connect(rotateAction, SIGNAL(triggered()), pdfTableView, SLOT(rotateSelected()));

  aboutAction = new QAction(tr("A&bout"), this);
  aboutAction->setIcon(QIcon(":/img/about.png"));
  aboutAction->setStatusTip(tr("About this program"));
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void HPDF::createToolBars(void) {
  fileToolBar = new QToolBar(tr("File"));
  fileToolBar->addAction(openAction);
  fileToolBar->addAction(exportAction);
  fileToolBar->addAction(exportAllAction);
  fileToolBar->setIconSize(QSize(48, 48));

  editToolBar = new QToolBar(tr("Edit"));
  editToolBar->addAction(cutAction);
  editToolBar->addAction(copyAction);
  editToolBar->addAction(pasteAction);
  editToolBar->addSeparator();
  editToolBar->addAction(deleteAction);
  editToolBar->setIconSize(QSize(48, 48));

  toolsToolBar = new QToolBar(tr("Tools"));
  toolsToolBar->addAction(rotateAction);
  toolsToolBar->setIconSize(QSize(48, 48));

  helpToolBar = new QToolBar(tr("Help"));
  helpToolBar->addAction(aboutAction);
  helpToolBar->setIconSize(QSize(48, 48));
}

void HPDF::createRibbon(void) {
  QWidget *tabFile = ribbon->widget(0);
  QVBoxLayout *layoutTabFile = new QVBoxLayout();
  layoutTabFile->setContentsMargins(2,0,2,0);
  layoutTabFile->addWidget(fileToolBar);
  tabFile->setLayout(layoutTabFile);

  QWidget *tabEdit = ribbon->widget(1);
  QVBoxLayout *layoutTabEdit = new QVBoxLayout();
  layoutTabEdit->setContentsMargins(2,0,2,0);
  layoutTabEdit->addWidget(editToolBar);
  tabEdit->setLayout(layoutTabEdit);

  QWidget *tabTools = ribbon->widget(2);
  QVBoxLayout *layoutTabTools = new QVBoxLayout();
  layoutTabTools->setContentsMargins(2,0,2,0);
  layoutTabTools->addWidget(toolsToolBar);
  tabTools->setLayout(layoutTabTools);

  QWidget *tabHelp = ribbon->widget(3);
  QVBoxLayout *layoutTabHelp = new QVBoxLayout();
  layoutTabHelp->setContentsMargins(2,0,2,0);
  layoutTabHelp->addWidget(helpToolBar);
  tabHelp->setLayout(layoutTabHelp);
}

void HPDF::createStatusBar(void) {
  statusBar()->showMessage(tr(""));
}

void HPDF::openFile(void) {
  QStringList fileNames = QFileDialog::getOpenFileNames(this,
                                                        tr("Open PDF file"), ".",
                                                        tr("PDF file (*.pdf)"));

  for (int i = 0; i < fileNames.size(); i++) {
    QString fileName = fileNames.at(i);
    if (!fileName.isEmpty()) {
      pdfTableView->loadFile(fileName);
    }
  }
}

void HPDF::exportFile(void) {
  PDFExportDialog *exportDialog = new PDFExportDialog();

  QVector<PDFFileWidget*> selectedFiles = pdfTableView->getSelectedFiles();
  if (selectedFiles.size() > 0) {
    exportDialog->setFilesToExport(selectedFiles, pdfTableView->getSelectedFileNames(), pdfTableView->getSelectedIndices());
    exportDialog->show();
  }
}

void HPDF::exportAllFiles(void) {
  PDFExportDialog *exportDialog = new PDFExportDialog();

  QVector<PDFFileWidget*> visibleFiles = pdfTableView->getVisibleFiles();

  if (visibleFiles.size() > 0) {
    exportDialog->setFilesToExport(visibleFiles, pdfTableView->getVisibleFileNames(), pdfTableView->getVisibleIndices());
    exportDialog->show();
  }
}

void HPDF::about(void) {
  QMessageBox::information(this, tr("HPDF"), tr("HPDF (Harringtons PDF) "
        "version 0.1.0\nWritten by: Ritchie Cunningham\n\n"
        "HPDF is primarily a PDF viewer with the prospect of becoming a full "
        "featured PDF editor.\n\n"
        "This software is unique in the fact that it automates tasks "
        "unique to Harringtons Advisory.\n\n"
        "Need support? Email customerservice@sccsolutions.biz"));
}

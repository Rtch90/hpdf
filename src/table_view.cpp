#include <QtWidgets>
#include <QWidget>
#include <assert.h>
#include "table_view.h"
#include "pdf_page_widget.h"
#include "pdf_file_widget.h"

PDFFileWidget*  test[10];

TableView::TableView(QWidget* parent) : QWidget(parent) {
  outerLayout = new QVBoxLayout();
  scrollArea  = new QScrollArea();
  frame       = new QFrame(scrollArea);
  layout      = new QVBoxLayout();

  setLayout(outerLayout);
  outerLayout->addWidget(scrollArea);
  scrollArea->setWidget(frame);
  frame->setLayout(layout);
  frame->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  frame->adjustSize();
  loadFile("/home/allanis/docs/loa.pdf");
  loadFile("/home/allanis/docs/loa.pdf");
}

void TableView::loadFile(QString fileName) {
  filenames.append(fileName);
  Poppler::Document* doc = Poppler::Document::load(fileName);
  files.append(doc);
  PDFFileWidget* pfw = new PDFFileWidget();
  pfw->setAncestor(this);
  pfw->setDocument(doc, fileName);

  fileWidgets.append(pfw);
  layout->addWidget(pfw);
}

void TableView::paintEvent(QPaintEvent* event) {
  frame->adjustSize();
  int newWidth = width() - 50;
 
  for(int i = 0; i < fileWidgets.size(); i++) {
    fileWidgets.at(i)->resize(newWidth, fileWidgets.at(i)->height());
  }

  frame->resize(newWidth, frame->height());
}


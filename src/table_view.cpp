#include <QtWidgets>
#include "table_view.h"
#include "pdf_page_widget.h"
#include "pdf_file_widget.h"

TableView::TableView(QWidget* parent) : QWidget(parent) {
  layout      = new QVBoxLayout();

  frame->setLayout(layout);
  frame = new QFrame();
  frame->setLayout(layout);
  frame->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  frame->adjustSize();
  
  scrollArea = new QScrollArea();
  scrollArea->setWidget(frame);

  outerLayout = new QVBoxLayout();
  setLayout(outerLayout);
  outerLayout->addWidget(scrollArea);

  loadFile("/home/allanis/docs/loa.pdf");
}

void TableView::loadFile(QString fileName) {
  fileNames.append(fileName);
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


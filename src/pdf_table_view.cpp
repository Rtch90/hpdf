#include <QtWidgets>
#include "pdf_table_view.h"
#include "pdf_page_widget.h"
#include "pdf_file_widget.h"

PDFTableView::PDFTableView(QWidget* parent) : QFrame(parent) {
  setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  outerLayout = new QVBoxLayout();
  outerLayout->setContentsMargins(0, 0, 0, 0);

  /* Spacer item. */
  QWidget* spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  outerLayout->addWidget(spacer);

  setLayout(outerLayout);

  loadFile("/home/allanis/docs/loa.pdf");
}

void PDFTableView::loadFile(QString fileName) {
  fileNames.append(fileName);
  
  Poppler::Document* doc = Poppler::Document::load(fileName);
  files.append(doc);
  
  PDFFileWidget* fileWidget = new PDFFileWidget();
  fileWidget->setAncestor(this);
  fileWidget->setDocument(doc, fileName);

  fileWidgets.append(fileWidget);
  outerLayout->insertWidget(outerLayout->count()-1, fileWidget);
}


#include <QtWidgets>
#include <QWidget>
#include <assert.h>
#include "table_view.h"
#include "pdf_page_widget.h"

PDFPageWidget* test[10];

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
  int i = 0;

  /* TODO: Page widget is placed for testing only.
   * To remove pagewidget and add filewidgets later,
   * use QVector<PDFFile> files to store files.
   */
  for(i = 0; i < 10; i++) {
    test[i] = new PDFPageWidget();
    test[i]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    
    layout->addWidget(test[i]);
  }
  /* End TODO. */
  frame->adjustSize();
}

void TableView::paintEvent(QPaintEvent* event) {
  frame->adjustSize();
  int newWidth = width();
  
  for(int i = 0; i < 10; i++) {
    test[i]->resize(newWidth, test[i]->height());
  }

  frame->resize(newWidth, frame->height());
}


#include <QtWidgets>
#include "pdf_table_widget.h"
#include "pdf_page_widget.h"
#include "pdf_file_widget.h"

PDFTableWidget::PDFTableWidget(QWidget* parent) : QFrame(parent) {
  /* Frame (Expanding with VBox) - Scroll Area (Expanding)
   * Container (Expanding with VBox) - Children.
   */
  setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  outerLayout = new QVBoxLayout();
  outerLayout->setContentsMargins(0, 0, 0, 0);

  scrollArea = new QScrollArea();
  scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  scrollArea->setWidgetResizable(true);

  containerLayout = new QVBoxLayout();
  containerWidget = new QWidget();
  containerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  QWidget* spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  containerLayout->addWidget(spacer);
  containerWidget->setLayout(containerLayout);

  scrollArea->setWidget(containerWidget);

  outerLayout->addWidget(scrollArea);
  
  setLayout(outerLayout);
}

void PDFTableWidget::loadFile(QString fileName) {
  Poppler::Document* doc = Poppler::Document::load(fileName);
  files.append(doc);
  
  PDFFileWidget* fileWidget = new PDFFileWidget();
  fileWidget->setAncestor(this);
  fileWidget->setDocument(doc, fileName);

  fileWidgets.append(fileWidget);

  fileNames.append(fileName);

  containerLayout->insertWidget(containerLayout->count()-1, fileWidget);
}

void PDFTableWidget::registerPage(PDFPageWidget* child) {
  /* Need new name? */
  QString name = QString("/home/pdfpage").append(QString::number(pageChilds.size()));
  pageChilds[name] = child;
  child->registerName(name);
}

void PDFTableWidget::pageClicked(QMouseEvent* event, QString path) {
  if(event->button() == Qt::LeftButton) {
    /* Left click to start dragging. */
    QDrag*      drag      = new QDrag(this);
    QMimeData*  mimeData  = new QMimeData();

    mimeData->setText(path);
    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap(":/img/copy.png"));

    Qt::DropAction dropAction = drag->exec();
  }
}

void PDFTableWidget::droppedPage(QString pathFrom, QString pathTo) {
  /*
   * We have dragged the page and dropped it.
   * So we'll handle backend operations here.
   */

  /* Frontend operations here.. */
  qDebug() << pathFrom;
  qDebug() << pathTo;
  PDFPageWidget* childFrom  = pageChilds[pathFrom];
  PDFPageWidget* childTo    = pageChilds[pathTo];

  PDFFileWidget* fileFrom   = (PDFFileWidget*) childFrom->getFather();
  PDFFileWidget* fileTo     = (PDFFileWidget*) childTo->getFather();

  int posFrom               = fileFrom->removeChild(childFrom);
  int posTo                 = fileTo->removeChild(childTo);

  fileTo->insertChildAt(childFrom, posTo);
  fileFrom->insertChildAt(childTo, posFrom);
}


#include <QtWidgets>
#include <QtGlobal>

#include "pdf_file_widget.h"
#include "pdf_page_widget.h"

#define COLLAPSE_BUTTON_WIDTH   60
#define COLLAPSE_BUTTON_HEIGHT  40

#define CHILD_AREA_WIDTH        150
#define CHILD_AREA_HEIGHT       150

FileWidget::FileWidget(QWidget* parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  setAcceptDrops(true);
  mainLayout = new QHBoxLayout();

  setLayout(mainLayout);
}

int FileWidget::getPagesCount() const {
  return pageWidgets.size();
}

QSize FileWidget::sizeHint() const {
  return QSize(CHILD_AREA_WIDTH*getPagesCount(), CHILD_AREA_HEIGHT + 20);
}

void FileWidget::addPageWidget(QImage* image) {
  PDFPageWidget* newPageWidget;
  newPageWidget = new PDFPageWidget();
  newPageWidget->setThumbnail(image);
  
  pageWidgets.push_back(newPageWidget);

  mainLayout->addWidget(newPageWidget);
  adjustSize();
}

void FileWidget::dragEnterEvent(QDragEnterEvent* event) {
  if(event->mimeData()->hasFormat("text/plain"))
    event->acceptProposedAction();
}

void FileWidget::dropEvent(QDropEvent* event) {
  int from  = event->mimeData()->text().toInt();
  int to    = findPageWidgetInLayout(pageWidgets[findPageContainingClickEvent(event->pos())]);

  mainLayout->removeWidget(pageWidgets[from]);
  mainLayout->insertWidget(to, pageWidgets[from]);

  event->acceptProposedAction();
}

void FileWidget::mousePressEvent(QMouseEvent* event) {
  if(event->button() == Qt::LeftButton) {
    int draggedChild = (findPageContainingClickEvent(event->pos()));

    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;

    mimeData->setText(QString::number(draggedChild));
    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap(":/img/copy.png"));

    Qt::DropAction dropAction = drag->exec();
  }
}

int FileWidget::findPageContainingClickEvent(QPoint pos) {
  for(int i = 0; i < getPagesCount(); i++)
    if(pageWidgets[i]->geometry().contains(pos))
      return i;
  
  return getPagesCount()-1;
}

int FileWidget::findPageWidgetInLayout(PDFPageWidget* pageWidget) {
  for(int i = 0; i < getPagesCount(); i++)
    if(mainLayout->itemAt(i)->widget() == pageWidget)
      return i;

  return getPagesCount()-1;
}

PDFFileWidget::PDFFileWidget(QWidget* parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  topLayout     = new QGridLayout();

  scrollArea    = new QScrollArea();
  fileWidget    = new FileWidget();
  scrollArea->setWidget(fileWidget);

  fileNameLabel = new QLabel();
  fileNameLabel->setText(tr("File 1"));

  topLayout->addWidget(fileNameLabel, 0, 1);
  
  collapseButton = new QPushButton(tr("X"));
  collapseButton->setMinimumSize(QSize(COLLAPSE_BUTTON_WIDTH, COLLAPSE_BUTTON_HEIGHT));
  collapseButton->setMaximumSize(QSize(COLLAPSE_BUTTON_WIDTH, COLLAPSE_BUTTON_HEIGHT));
  connect(collapseButton, SIGNAL(released()), this, SLOT(collapsedButtonClick()));
  topLayout->addWidget(collapseButton, 0, 0);

  topLayout->addWidget(scrollArea, 1, 0, 1, 5);
  setLayout(topLayout);

  setCollapsed(false);
  adjustSize();
}

#if 0
QSize PDFFileWidget::sizeHint() const {
  if(collapsed == true)
    return QSize(mainChild->width(), collapseButton->height());
  else
    return QSize(mainChild->width(), collapseButton->height() + mainChild->height() + 50);
}
#endif

void PDFFileWidget::setCollapsed(bool state) {
  if(state == true) {
    collapsed = true;
    setFixedHeight(collapseButton->height() + 80);
    scrollArea->hide();
  } else {
    collapsed = false;
    setFixedHeight(collapseButton->height() + fileWidget->height() + 50);
    scrollArea->show();
  }

  adjustSize();
}

void PDFFileWidget::collapsedButtonClick(void) {
  if(collapsed == true)
    setCollapsed(false);
  else
    setCollapsed(true);
}

void PDFFileWidget::setDocument(Poppler::Document* document, QString fileName) {
  int numPages = document->numPages();
  for(int i; i < numPages; i++) {
    Poppler::Page* pdfPage = document->page(i);
    QImage* image = new QImage();
    *image = pdfPage->renderToImage(144, 144);

    fileWidget->addPageWidget(image);
  }
}


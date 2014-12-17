#include <QtWidgets>
#include <QtGlobal>

#include "pdf_file_widget.h"
#include "pdf_page_widget.h"

#define COLLAPSE_BUTTON_WIDTH   32
#define COLLAPSE_BUTTON_HEIGHT  32

#define CHILD_AREA_WIDTH        150
#define CHILD_AREA_HEIGHT       150

PagesContainerWidget::PagesContainerWidget(QWidget* parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  setAcceptDrops(true);
  mainLayout = new QHBoxLayout();

  setLayout(mainLayout);
}

int PagesContainerWidget::getPagesCount() const {
  return pageWidgets.size();
}

QSize PagesContainerWidget::sizeHint() const {
  return QSize(CHILD_AREA_WIDTH*getPagesCount(), CHILD_AREA_HEIGHT + 20);
}

void PagesContainerWidget::addPageWidget(PDFPageWidget* pageWidget) {
  pageWidgets.append(pageWidget);
  mainLayout->addWidget(pageWidget);
  adjustSize();
}

void PagesContainerWidget::dragEnterEvent(QDragEnterEvent* event) {
  if(event->mimeData()->hasFormat("text/plain"))
    event->acceptProposedAction();
}

void PagesContainerWidget::dropEvent(QDropEvent* event) {
  int from  = event->mimeData()->text().toInt();
  int to    = findPageWidgetInLayout(pageWidgets[findPageContainingClickEvent(event->pos())]);

  mainLayout->removeWidget(pageWidgets[from]);
  mainLayout->insertWidget(to, pageWidgets[from]);

  event->acceptProposedAction();
}

void PagesContainerWidget::mousePressEvent(QMouseEvent* event) {
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

int PagesContainerWidget::findPageContainingClickEvent(QPoint pos) {
  for(int i = 0; i < getPagesCount(); i++)
    if(pageWidgets[i]->geometry().contains(pos))
      return i;
  
  return getPagesCount()-1;
}

int PagesContainerWidget::findPageWidgetInLayout(PDFPageWidget* pageWidget) {
  for(int i = 0; i < getPagesCount(); i++)
    if(mainLayout->itemAt(i)->widget() == pageWidget)
      return i;

  return getPagesCount()-1;
}

PDFFileWidget::PDFFileWidget(QWidget* parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  topLayout       = new QGridLayout();

  collapseButton  = new QPushButton();
  collapseButton->resize(QSize(COLLAPSE_BUTTON_WIDTH, COLLAPSE_BUTTON_HEIGHT));
  collapseButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  collapseButton->setIcon(QIcon(":/img/collapse.png"));
  connect(collapseButton, SIGNAL(released()), this, SLOT(collapsedButtonClick()));
  topLayout->addWidget(collapseButton, 0, 0);

  fileNameLabel = new QLabel();
  topLayout->addWidget(fileNameLabel, 0, 1);
  
  pagesContainerWidget = new PagesContainerWidget();
  scrollArea = new QScrollArea();
  scrollArea->setWidget(pagesContainerWidget);
  topLayout->addWidget(scrollArea, 1, 0, 1, 5);

  setLayout(topLayout);

  setCollapsed(false);

  connect(&tgen, SIGNAL(updateThumbnail(QImage,PDFPageWidget*)), this,
          SLOT(updateThumbnail(QImage,PDFPageWidget*)));
}

void PDFFileWidget::updateThumbnail(QImage img, PDFPageWidget* pw) {
  pw->setThumbnail(img);
}

void PDFFileWidget::setCollapsed(bool state) {
  collapsed = state;
  if(state == true) {
    scrollArea->hide();
    collapseButton->setIcon(QIcon(":/img/expand.png"));
  } else {
    scrollArea->show();
    collapseButton->setIcon(QIcon(":img/collapse.png"));
  }
}

void PDFFileWidget::collapsedButtonClick(void) {
  setCollapsed(!collapsed);
}

void PDFFileWidget::pageClickedHandler(QMouseEvent*, QImage) {

}

void PDFFileWidget::setDocument(Poppler::Document* document, QString fileName) {
  document->setRenderHint(Poppler::Document::TextAntialiasing);
  Poppler::Document* doc = Poppler::Document::load(fileName);
  doc->setRenderHint(Poppler::Document::TextAntialiasing);

  int numPages = document->numPages();
  for(int i = 0; i < numPages; i++) {
    Poppler::Page* pdfPage = document->page(i);

    PDFPageWidget* pageWidget = new PDFPageWidget();
  
    pageWidget->setPopplerPage(doc->page(i));
    tgen.render(pageWidget, pdfPage);

    connect(pageWidget, SIGNAL(pageClicked(QMouseEvent*,QImage)), this,
            SIGNAL(pageClicked(QMouseEvent*,QImage)));
    connect(pageWidget, SIGNAL(previewUpdate(Poppler::Page*)), this,
            SIGNAL(previewUpdate(Poppler::Page*)));

    pagesContainerWidget->addPageWidget(pageWidget);
    /* Process event. */
    /*qApp->processEvents();*/
  }
  tgen.start();
  fileNameLabel->setText(fileName);
}


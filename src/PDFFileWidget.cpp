#include <QtGlobal>
#include <QtWidgets>

#include "PDFFileWidget.h"
#include "PDFPageWidget.h"
#include "PDFTableWidget.h"

#define COLLAPSE_BUTTON_WIDTH   32
#define COLLAPSE_BUTTON_HEIGHT  32

#define CHILD_AREA_WIDTH        150
#define CHILD_AREA_HEIGHT       150
#define CHILD_AREA_SIDE_MARGIN  20

#define DEFAULT_CHILD_SIZE      169

PagesContainerWidget::PagesContainerWidget(QWidget *parent) {
  (void) parent;
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  setAcceptDrops(true);
  mainLayout = new QHBoxLayout();

  this->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
          this, SLOT(ShowContextMenu(const QPoint&)));

  setLayout(mainLayout);
}

void PagesContainerWidget::ShowContextMenu(const QPoint& pos){
  if (((PDFTableWidget*)ancestor)->hasClipboard()){
    QPoint globalPos = this->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction(QIcon(":/img/remove.png"), "Paste");

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
      int page;

      if (pageWidgets.size()>=2)
        page = (pos.x() / (pageWidgets.at(1)->geometry().x() -
                           pageWidgets.at(0)->geometry().x()) );
      else
        page = (pos.x() / DEFAULT_CHILD_SIZE );


      PDFTableWidget *tableview = (PDFTableWidget*) ancestor;
      tableview->pastePage((PDFFileWidget*)father, page);
    }
  }

}
int PagesContainerWidget::getPagesCount() const {
  return pageWidgets.size();
}

QSize PagesContainerWidget::sizeHint() const {
  QSize temp = QSize((CHILD_AREA_SIDE_MARGIN + CHILD_AREA_WIDTH) * getPagesCount(), CHILD_AREA_HEIGHT + 30);
  return temp;
}

void PagesContainerWidget::addPageWidget(PDFPageWidget *pageWidget){
  pageWidgets.append(pageWidget);
  mainLayout->addWidget(pageWidget);
  adjustSize();
}

void PagesContainerWidget::dragEnterEvent(QDragEnterEvent *event) {
  event->acceptProposedAction();
}

void PagesContainerWidget::dropEvent(QDropEvent *event) {
  QPoint pos = event->pos();
  int page = (pos.x() / (CHILD_AREA_SIDE_MARGIN + CHILD_AREA_WIDTH));
  ((PDFTableWidget*)ancestor)->moveSelectedPages(event->mimeData()->text()  , pageWidgets.at(page));
  event->acceptProposedAction();
}

// ======================================================================

PDFFileWidget::PDFFileWidget(QWidget *parent):QFrame(parent){
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  setFrameStyle(QFrame::Box);

  topLayout      = new QGridLayout();

  collapseButton = new QPushButton();
  collapseButton->resize(QSize(COLLAPSE_BUTTON_WIDTH,COLLAPSE_BUTTON_HEIGHT));
  collapseButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  collapseButton->setIcon(QIcon(":/img/collapse.png"));
  connect(collapseButton, SIGNAL(released()), this, SLOT(collapsedButtonClicked()));
  topLayout->addWidget(collapseButton, 0, 0);

  fileNameLabel = new QLabel();
  fileNameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  topLayout->addWidget(fileNameLabel, 0, 1);

  removeButton = new QPushButton();
  removeButton->resize(QSize(COLLAPSE_BUTTON_WIDTH,COLLAPSE_BUTTON_HEIGHT));
  removeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  removeButton->setIcon(QIcon(":/img/remove.png"));
  connect(removeButton, SIGNAL(released()), this, SLOT(removeButtonClicked()));
  topLayout->addWidget(removeButton, 0, 2);

  pagesContainerWidget = new PagesContainerWidget();
  pagesContainerWidget->setFather(this);
  scrollArea = new QScrollArea();
  scrollArea->setWidget(pagesContainerWidget);
  scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  topLayout->addWidget(scrollArea, 1, 0, 1, 3);

  topLayout->setContentsMargins(6, 10, 6, 10);
  setLayout(topLayout);

  setCollapsed(false);
  selected = false;

  connect(&tgen, SIGNAL(updateThumbnail(QImage,PDFPageWidget*)), this, SLOT(updateThumbnail(QImage,PDFPageWidget*)));
}

void PDFFileWidget::setAncestor(QWidget* ancestor) {
  this->ancestor = ancestor;
  pagesContainerWidget->setAncestor(ancestor);
  connect(this, SIGNAL(fileClicked(PDFFileWidget*, QMouseEvent*)), ancestor, SLOT(fileClicked(PDFFileWidget*, QMouseEvent*)));
}

void PDFFileWidget::setSelected(bool select) {
  selected = select;
  update();
}

void PDFFileWidget::mousePressEvent(QMouseEvent *event) {
  emit fileClicked(this, event);
}

void PDFFileWidget::paintEvent(QPaintEvent *event) {
  QPalette palette = this->palette();
  QPalette labelPalette = fileNameLabel->palette();
  if (selected) {
    palette.setColor( foregroundRole(), palette.color(QPalette::Highlight) );
    labelPalette.setColor( foregroundRole(), palette.color(QPalette::Highlight) );
  } else {
    palette.setColor( foregroundRole(), palette.color(QPalette::Dark) );
    labelPalette.setColor( foregroundRole(), palette.color(QPalette::Text) );
  }
  this->setPalette(palette);
  fileNameLabel->setPalette(labelPalette);

  QFrame::paintEvent(event);
}

void PDFFileWidget::updateThumbnail(QImage img, PDFPageWidget* pageWidget){
  pageWidget->setThumbnail(img);
}

void PDFFileWidget::setCollapsed(bool state){
  collapsed = state;
  if (state == true) {
    scrollArea->hide();
    collapseButton->setIcon(QIcon(":/img/expand.png"));
  } else {
    scrollArea->show();
    collapseButton->setIcon(QIcon(":/img/collapse.png"));
  }
}

void PDFFileWidget::collapsedButtonClicked(){
  setCollapsed(!collapsed);
}

void PDFFileWidget::removeButtonClicked() {
  emit fileRemoveButtonClicked(this);
}

void PDFFileWidget::setDocument(Poppler::Document* document, QString fileName){
  document->setRenderHint(Poppler::Document::TextAntialiasing);
  Poppler::Document *previewDoc = Poppler::Document::load(fileName);
  previewDoc->setRenderHint(Poppler::Document::TextAntialiasing);
  Poppler::Document *thumbDoc = Poppler::Document::load(fileName);
  thumbDoc->setRenderHint(Poppler::Document::TextAntialiasing);

  int numPages = document -> numPages();
  for (int i = 0; i < numPages; i++){
    Poppler::Page *pdfPage = document->page(i);

    PDFPageWidget *pageWidget = new PDFPageWidget();

    pageWidget->setAncestor(ancestor);
    pageWidget->setFather(this);
    pageWidget->setPopplerPage(previewDoc->page(i));
    pageWidget->setThumbPopplerPage(thumbDoc->page(i));
    pageWidget->setOriginInfo(fileName,i);
    tgen.render(pageWidget,pdfPage);

    pagesContainerWidget->addPageWidget(pageWidget);
  }
  tgen.start();

  fileNameLabel->setText(fileName);
}

int PDFFileWidget::removeChild(PDFPageWidget* child){
  int pos = pagesContainerWidget->pageWidgets.indexOf(child);
  pagesContainerWidget->pageWidgets.remove(pos);
  pagesContainerWidget->mainLayout->removeWidget(child);
  child->hide();
  //pagesContainerWidget->mainLayout->removeItem(pagesContainerWidget->mainLayout->itemAt(pos));

  pagesContainerWidget->adjustSize();
  return pos;
}

int PDFFileWidget::indexChild(PDFPageWidget* child){
  return pagesContainerWidget->pageWidgets.indexOf(child);
}

void PDFFileWidget::insertChildAt(PDFPageWidget* child, int pos){
  child->setFather(this);
  pagesContainerWidget->mainLayout->insertWidget(pos, child);
  pagesContainerWidget->pageWidgets.insert(pos,child);
  tgen.render(child,child->getNewThumbPopplerPage());
  tgen.start();
  child->show();

  pagesContainerWidget->adjustSize();
}

int PDFFileWidget::getChildCount() {
  return pagesContainerWidget->getPagesCount();
}

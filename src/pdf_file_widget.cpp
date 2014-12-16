#include <QDebug>
#include <QSizePolicy>

#include <QPixmap>
#include <QSize>
#include <QDrag>
#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QDropEvent>
#include <QMimeData>

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

int FileWidget::getChildCount() const {
  return child.size();
}

QSize FileWidget::sizeHint() const {
  qDebug() << size();
  return QSize(CHILD_AREA_WIDTH*getChildCount(), CHILD_AREA_HEIGHT + 20);
}

void FileWidget::addChild(QString name) {
  PDFPageWidget* newchild;
  newchild = new PDFPageWidget();
  newchild->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  newchild->resize(CHILD_AREA_WIDTH, CHILD_AREA_HEIGHT);

  child.push_back(newchild);

  mainLayout->addWidget(newchild);
  adjustSize();
}

void FileWidget::addChild(QImage* image) {
  PDFPageWidget* newchild;
  newchild = new PDFPageWidget();
  newchild->setThumbnail(image);

  child.push_back(newchild);
  
  mainLayout->addWidget(newchild);
  adjustSize();
}

void FileWidget::dragEnterEvent(QDragEnterEvent* event) {
  if(event->mimeData()->hasFormat("text/plain"))
    event->acceptProposedAction();
}

void FileWidget::dropEvent(QDropEvent* event) {
  int from  = event->mimeData()->text().toInt();
  int to    = findChildPositionInLayout(child[findClickEventChild(event->pos())]);

  qDebug() << "Moving " + QString::number(from) + " " + QString::number(to);

  for(int i = 0; i < child.size(); i++) {
    qDebug() << child[i]->size();
  }

  mainLayout->removeWidget(child[from]);
  mainLayout->insertWidget(to, child[from]);
  
  event->acceptProposedAction();
}

void FileWidget::mousePressEvent(QMouseEvent* event) {
  if(event->button() == Qt::LeftButton) {
    int draggedChild = (findClickEventChild(event->pos()));

    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;

    mimeData->setText(QString::number(draggedChild));
    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap(":/img/copy.png"));

    Qt::DropAction dropAction = drag->exec();
  }
}

int FileWidget::findClickEventChild(QPoint pos) {
  for(int i = 0; i < getChildCount(); i++)
    if(child[i]->geometry().contains(pos))
      return i;
  
  return getChildCount()-1;
}

int FileWidget::findChildPositionInLayout(PDFPageWidget* child) {
  for(int i = 0; i < getChildCount(); i++)
    if(mainLayout->itemAt(i)->widget() == child)
      return i;

  return getChildCount()-1;
}

PDFFileWidget::PDFFileWidget(QWidget* parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  topLayout   = new QGridLayout();

  scrollArea  = new QScrollArea();
  mainChild   = new FileWidget();

  widgetName  = new QLabel();
  widgetName->setText(tr("File 1"));
  topLayout->addWidget(widgetName, 0, 1);
  
  collapseButton = new QPushButton(tr("X"));
  collapseButton->setMinimumSize(QSize(COLLAPSE_BUTTON_WIDTH, COLLAPSE_BUTTON_HEIGHT));
  collapseButton->setMaximumSize(QSize(COLLAPSE_BUTTON_WIDTH, COLLAPSE_BUTTON_HEIGHT));
  connect(collapseButton, SIGNAL(released()), this, SLOT(collapsedButtonClick()));
  topLayout->addWidget(collapseButton, 0, 0);

  topLayout->addWidget(scrollArea, 1, 0, 1, 5);
  scrollArea->setWidget(mainChild);

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
    setFixedHeight(collapseButton->height() + mainChild->height() + 50);
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

void PDFFileWidget::setDocument(Poppler::Document* document, QString filename) {
  int numPages = document->numPages();
  for(int i; i < numPages; i++) {
    Poppler::Page* pdfPage = document->page(i);
    QImage* image = new QImage();
    *image = pdfPage->renderToImage();

    mainChild->addChild(image);
  }
}


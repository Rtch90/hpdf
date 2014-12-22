#include <QtGlobal>
#include <QtWidgets>
#include "ThumbGen.h"
#include "PDFPageWidget.h"
#include "PDFTableWidget.h"

#define PAGE_WIDTH      150
#define PAGE_HEIGHT     150

PDFPageWidget::PDFPageWidget(QWidget *parent) :
  QFrame(parent)
{
  selected = false;
  rotation = 0;
  setAcceptDrops(true);

  this->resize(PAGE_WIDTH, PAGE_HEIGHT);
  this->setMinimumSize(PAGE_WIDTH, PAGE_HEIGHT);
  this->setMaximumSize(PAGE_WIDTH, PAGE_HEIGHT);
  this->setAutoFillBackground(true);

  this->setMouseTracking(true);
  this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  QHBoxLayout *topHBox = new QHBoxLayout();
  btnRotate = new QPushButton("", this);
  btnRotate->setObjectName("rotate");
  btnRotate->setIcon(QPixmap::fromImage(QImage(":/img/rotate.png")));
  btnRotate->hide();
  topHBox->addWidget(btnRotate, 0, Qt::AlignLeft);

  btnDelete = new QPushButton("", this);
  btnDelete->setObjectName("rotate");
  btnDelete->setIcon(QPixmap::fromImage(QImage(":/img/remove.png")));
  btnDelete->hide();
  topHBox->addWidget(btnDelete, 1, Qt::AlignRight);

  QHBoxLayout *bottomHBox = new QHBoxLayout();
  btnCut = new QPushButton("", this);
  btnCut->setObjectName("cut");
  btnCut->setIcon(QPixmap::fromImage(QImage(":/img/cut.png")));
  btnCut->hide();

  btnCopy = new QPushButton("", this);
  btnCopy->setObjectName("copy");
  btnCopy->setIcon(QPixmap::fromImage(QImage(":/img/copy.png")));
  btnCopy->hide();

  bottomHBox->addWidget(btnCut, 0, Qt::AlignLeft);
  bottomHBox->addWidget(btnCopy, 1, Qt::AlignRight);

  QVBoxLayout *vbox = new QVBoxLayout();
  vbox->addLayout(topHBox);
  vbox->addStretch(1);
  vbox->addLayout(bottomHBox);
  vbox->setContentsMargins(0, 3, 0, 3);
  this->setLayout(vbox);
}

void PDFPageWidget::setAncestor(QWidget* ancestor){
  this->ancestor = ancestor;
  ((PDFTableWidget*)ancestor)->registerPage(this);
  connect(this, SIGNAL(pageClicked(PDFPageWidget*, QMouseEvent*, QString)), ancestor, SLOT(pageClicked(PDFPageWidget*, QMouseEvent*, QString)));
  connect(this, SIGNAL(pageDropped(PDFPageWidget*, QDropEvent*, QString, QString)), ancestor, SLOT(pageDropped(PDFPageWidget*, QDropEvent*, QString, QString)));
  connect(btnDelete, SIGNAL(clicked()), this, SLOT(pageDelete()));
  connect(btnCopy, SIGNAL(clicked()), this, SLOT(pageCopy()));
  connect(btnRotate, SIGNAL(clicked()), this, SLOT(pageRotate()));
  connect(btnCut, SIGNAL(clicked()), this, SLOT(pageCut()));
}

void PDFPageWidget::pageCut(){
  ((PDFTableWidget*)ancestor)->clearClipboard();
  ((PDFTableWidget*)ancestor)->cutPage(this);
}

void PDFPageWidget::pageDelete(){
  ((PDFTableWidget*)ancestor)->deletePage(this);
}

void PDFPageWidget::pageCopy(){
  ((PDFTableWidget*)ancestor)->clearClipboard();
  ((PDFTableWidget*)ancestor)->copyPage(this);
}

void PDFPageWidget::pageRotate(){
  ((PDFTableWidget*)ancestor)->rotatePage(this);
}

void PDFPageWidget::rotate90(void) {
  rotation += 90;
  if (rotation == 360) rotation = 0;

  ThumbGen *tgen = new ThumbGen();
  double dpi=tgen->calcDpi(thumbPage,size());

  image = thumbPage->renderToImage(dpi, dpi, -1, -1, -1, -1, getRotation());
  setThumbnail(image);
}

Poppler::Page::Rotation PDFPageWidget::getRotation() {
  if (rotation == 0) return Poppler::Page::Rotate0;
  else if (rotation == 90) return Poppler::Page::Rotate90;
  else if (rotation == 180) return Poppler::Page::Rotate180;
  else if (rotation == 270) return Poppler::Page::Rotate270;
}

void PDFPageWidget::setFather(QWidget *father){
  this->father = father;
}

void PDFPageWidget::setPopplerPage(Poppler::Page* pp) {
  previewPage = pp;
}
Poppler::Page* PDFPageWidget::getNewThumbPopplerPage(){
  Poppler::Document *thumbDoc = Poppler::Document::load(oriFilePath);
  thumbDoc->setRenderHint(Poppler::Document::TextAntialiasing);
  return thumbDoc->page(pageNo);
}
void PDFPageWidget::setThumbPopplerPage(Poppler::Page* pp) {
  thumbPage = pp;
}


void PDFPageWidget::setThumbnail(QImage pageImage) {
  image = pageImage;
  pixmap = QPixmap::fromImage(image);
  pixmap = pixmap.scaled(size() - QSize(6, 6), Qt::KeepAspectRatio);

  update();
}

void PDFPageWidget::setSelected(bool select) {
  selected = select;
  update();
}

void PDFPageWidget::mousePressEvent(QMouseEvent *event) {
  if (previewPage!=NULL){
    emit pageClicked(this, event, path);
  }
}

void PDFPageWidget::dragEnterEvent(QDragEnterEvent *event) {
  event->acceptProposedAction();
}

void PDFPageWidget::dropEvent(QDropEvent *event) {
  emit pageDropped(this, event, event->mimeData()->text(), path);
  event->acceptProposedAction();
}

void PDFPageWidget::leaveEvent(QEvent *event) {
  (void)event;
  btnRotate->hide();
  btnCut->hide();
  btnCopy->hide();
  btnDelete->hide();

  this->setFrameStyle(QFrame::Plain);
}

void PDFPageWidget::enterEvent(QEvent *event) {
  (void)event;
  btnRotate->show();
  btnCut->show();
  btnCopy->show();
  btnDelete->show();

  this->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
}

void PDFPageWidget::paintEvent(QPaintEvent *event) {
  QPalette palette = this->palette();
  if (selected)
    palette.setColor( backgroundRole(), palette.color(QPalette::Highlight) );
  else
    palette.setColor( backgroundRole(), palette.color(QPalette::AlternateBase) );
  this->setPalette( palette );

  QFrame::paintEvent(event);
  QPainter painter(this);
  painter.drawPixmap(QRect((size().width() - pixmap.width()) / 2, (size().height() - pixmap.height()) / 2, pixmap.width(), pixmap.height()), pixmap);
}

QSize PDFPageWidget::sizeHint() const {
  return size();
}

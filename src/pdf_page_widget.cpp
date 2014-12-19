#include <QtGlobal>
#include <QtWidgets>
#include "pdf_page_widget.h"
#include "pdf_table_widget.h"

PDFPageWidget::PDFPageWidget(QWidget* parent) :
    QFrame(parent) {

  selected = false;
  setAcceptDrops(true);

  this->resize(150, 150);
  this->setMinimumSize(150, 150);
  this->setMaximumSize(150, 150);
  this->setAutoFillBackground(true);

  this->setMouseTracking(true);
  this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  QHBoxLayout* hbox = new QHBoxLayout();

  btn1 = new QPushButton("", this);
  btn1->setObjectName("new");
  btn2 = new QPushButton("", this);
  btn2->setObjectName("open");

  QPixmap pm, pm2;
  pm.load("../img/new.png");
  pm2.load("../img/open.png");
  
  btn1->setIcon(pm);
  btn2->setIcon(pm2);
  btn1->hide();
  btn2->hide();

  hbox->addWidget(btn1, 1, Qt::AlignRight);
  hbox->addWidget(btn2, 0, Qt::AlignLeft);

  QVBoxLayout* vbox = new QVBoxLayout();
  vbox->addStretch(1);
  vbox->addLayout(hbox);
  vbox->setContentsMargins(0, 3, 0, 3);
  this->setLayout(vbox);
}

void PDFPageWidget::setAncestor(QWidget* ancestor) {
  this->ancestor = ancestor;
  ((PDFTableWidget*)ancestor)->registerPage(this);
  connect(this, SIGNAL(previewUpdate(Poppler::Page*)), ancestor,
          SIGNAL(previewUpdate(Poppler::Page*)));

  connect(this, SIGNAL(pageClicked(PDFPageWidget*, QMouseEvent*, QString)),
          ancestor, SLOT(pageClicked(PDFPageWidget*, QMouseEvent*, QString)));
  connect(this, SIGNAL(pageDropped(PDFPageWidget*, QDropEvent*, QString,QString)),
          ancestor, SLOT(pageDropped(PDFPageWidget*, QDropEvent*, QString,QString)));
}

void PDFPageWidget::setFather(QWidget* father) {
  this->father = father;
}

void PDFPageWidget::setButton(QPushButton* btn) {
  button = btn;
  update();
}

void PDFPageWidget::setPopplerPage(Poppler::Page* pp) {
  pPage = pp;
}

void PDFPageWidget::setThumbnail(QImage pageImage) {
  image   = pageImage;
  pixmap  = QPixmap::fromImage(image);
  pixmap  = pixmap.scaled(size() - QSize(6,6), Qt::KeepAspectRatio);

  update();
}

void PDFPageWidget::setSelected(bool select) {
  selected = select;
  update();
}

void PDFPageWidget::mousePressEvent(QMouseEvent* event) {
  if(pPage != NULL) {
    emit pageClicked(this, event, path);
    emit previewUpdate(pPage);

    selected = !selected;
    update();
  }
}

void PDFPageWidget::dragEnterEvent(QDragEnterEvent* event) {
  event->acceptProposedAction();
}

void PDFPageWidget::dropEvent(QDropEvent* event) {
  emit pageDropped(this, event, event->mimeData()->text(), path);
  event->acceptProposedAction();
}

void PDFPageWidget::leaveEvent(QEvent* event) {
 btn1->hide();
 btn2->hide();

 this->setFrameStyle(QFrame::Plain);
}

void PDFPageWidget::enterEvent(QEvent* event) {
  btn1->show();
  btn2->show();

  this->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
}

void PDFPageWidget::paintEvent(QPaintEvent* event) {
  QPalette palette = this->palette();
  if(selected)
    palette.setColor(backgroundRole(), palette.color(QPalette::Highlight));
  else
    palette.setColor(backgroundRole(), palette.color(QPalette::AlternateBase));
  this->setPalette(palette);

  QFrame::paintEvent(event);
  QPainter painter(this);
  painter.drawPixmap(QRect((size().width() - pixmap.width()) / 2,
                     (size().height() - pixmap.height()) / 2,
                     pixmap.width(), pixmap.height()), pixmap);
}

QSize PDFPageWidget::sizeHint() const {
  return size();
}


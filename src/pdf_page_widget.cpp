#include <QtGlobal>
#include <QtWidgets>
#include "pdf_page_widget.h"

PDFPageWidget::PDFPageWidget(QWidget* parent) :
    QFrame(parent) {

  /* Resize widget. */
  this->resize(150, 150);
  this->setMinimumSize(150, 150);
  this->setMinimumSize(150, 150);

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

void PDFPageWidget::setButton(QPushButton* btn) {
  button = btn;
  update();
}

void PDFPageWidget::setPopplerPage(Poppler::Page* pp) {
  pPage = pp;
}

void PDFPageWidget::setThumbnail(QImage pageImage) {
  image = pageImage;
  pixmap = QPixmap::fromImage(image);
  pixmap = pixmap.scaled(size(), Qt::KeepAspectRatio);

  update();
}

void PDFPageWidget::mousePressEvent(QMouseEvent* event) {
  if(pPage != NULL) {
    emit pageClicked(event, image);
    emit previewUpdate(pPage);
  }
}

void PDFPageWidget::leaveEvent(QEvent* event) {
 btn1->hide();
 btn2->hide();
}

void PDFPageWidget::enterEvent(QEvent* event) {
  btn1->show();
  btn2->show();
}

void PDFPageWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.drawPixmap(QRect((size().width() - pixmap.width()) / 2,
                     (size().height() - pixmap.height()) / 2,
                     pixmap.width(), pixmap.height()), pixmap);
}

QSize PDFPageWidget::sizeHint() const {
  return size();
}


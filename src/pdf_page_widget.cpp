#include <QtWidgets>
#include <QSize>
#include "pdf_page_widget.h"

PDFPageWidget::PDFPageWidget(QWidget* parent) :
    QWidget(parent) {

  /* Resize widget. */
  this->resize(150, 150);
  this->setMinimumHeight(150);

  this->setMouseTracking(true);
  this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  /* Initialize buttons. */
  /*button = new QPushButton();
  QPushButton* btn = new QPusBbutton();
  QGridLayout* lo = new QGridLayout();
  btn->setFixedSize(100, 100);
  qDebug() << btn->geometry();
  *lo->addWidget(btn);*/

  QVBoxLayout* vbox = new QVBoxLayout(this);
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

  vbox->addStretch(1);
  vbox->addLayout(hbox);

  this->setLayout(vbox);

  this->setThumbnail(new QImage("../img/img.jpg"));
}

void PDFPageWidget::setButton(QPushButton* btn) {
  button = btn;
  update();
}

void PDFPageWidget::setThumbnail(QImage* pageImage) {
  image = pageImage;
  pixmap = QPixmap::fromImage(*image);
  update();
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
  painter.drawPixmap(QRect(0, 0, width(), height()), pixmap);
}

QSize PDFPageWidget::sizeHint() const {
  return QSize(150, 150);
}


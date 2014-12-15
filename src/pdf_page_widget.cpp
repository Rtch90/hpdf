#include <QtWidgets>
#include "pdf_page_widget.h"

PDFPageWidget::PDFPageWidget(QWidget* parent) :
    QWidget(parent) {

  this->resize(500, 600);
}

void PDFPageWidget::setPageImage(QImage* pageImage) {
  image = pageImage;
  pixmap = QPixmap::fromImage(*image);
  update();
}

void PDFPageWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.drawPixmap(QRect(0, 0, pixmap.width(), pixmap.height()), pixmap);
}


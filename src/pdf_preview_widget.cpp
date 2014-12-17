#include <QtWidgets>
#include "pdf_preview_widget.h"

PDFPreviewWidget::PDFPreviewWidget(QWidget* parent) : QWidget(parent) {
  
}

void PDFPreviewWidget::setImage(QImage image) {
  previewImage = image;
  update();
}

void PDFPreviewWidget::pageClicked(QMouseEvent* mouseEvent, QImage image) {
  setImage(image);
}

void PDFPreviewWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);

  QPixmap pixmap = QPixmap::fromImage(previewImage);
  pixmap = pixmap.scaled(size(), Qt::KeepAspectRatio);
  painter.drawPixmap(QRect((size().width() - pixmap.width()) / 2,
                    (size().height() - pixmap.height()) / 2,
                    pixmap.width(), pixmap.height()), pixmap);
}


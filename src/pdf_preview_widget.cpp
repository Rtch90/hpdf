#include <QtWidgets>
#include "pdf_preview_widget.h"

PDFPreviewWidget::PDFPreviewWidget(QWidget* parent) : QWidget(parent) {
  pPage = NULL;
}

void PDFPreviewWidget::setImage(QImage image) {
  previewImage = image;
  update();
}

void PDFPreviewWidget::regenImage(void) {
  double dpi;
  double dpi2;
  QSize targetSize  = this->size();
  QSizeF oriSize    = pPage->pageSizeF();
  double oriDpi     = 72;
  dpi   = targetSize.height() / (float)oriSize.height()*72;
  dpi2  = targetSize.width()  / (float)oriSize.width()*72;
  dpi   = dpi<dpi2?dpi:dpi2;
  previewImage = pPage->renderToImage(dpi, dpi);
}

void PDFPreviewWidget::previewUpdate(Poppler::Page* pp) {
  pPage = pp;
  regenImage();
  update();
}

void PDFPreviewWidget::resizeEvent(QResizeEvent* event) {
  if(pPage != NULL) regenImage();
}

void PDFPreviewWidget::paintEvent(QPaintEvent* event) {
  if(pPage != NULL) {

    QPainter painter(this);
    QPixmap pixmap = QPixmap::fromImage(previewImage);
    pixmap = pixmap.scaled(size(), Qt::KeepAspectRatio);
    painter.drawPixmap(QRect((size().width() - pixmap.width()) / 2,
                      (size().height() - pixmap.height()) / 2,
                      pixmap.width(), pixmap.height()), pixmap);
  }
}


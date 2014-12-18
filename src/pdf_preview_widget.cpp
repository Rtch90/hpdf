#include <QtWidgets>
#include "pdf_preview_widget.h"

PDFPreviewWidget::PDFPreviewWidget(QWidget* parent) : QWidget(parent) {
  pPage = NULL;
  currentPixmapSize = QSize(0,0);
}

void PDFPreviewWidget::regenImage(void) {
  double dpi;
  double dpi2;
  QSize targetSize;

  if(currentPixmapSize == QSize(0,0))
    currentPixmapSize = this->size();

  targetSize = currentPixmapSize;
  QSizeF oriSize    = pPage->pageSizeF();
  double oriDpi     = 72;
  dpi   = targetSize.height() / (float)oriSize.height()*oriDpi;
  dpi2  = targetSize.width()  / (float)oriSize.width()*oriDpi;
  dpi   = dpi<dpi2?dpi:dpi2;
  previewImage = pPage->renderToImage(dpi, dpi);
}

void PDFPreviewWidget::previewUpdate(Poppler::Page* pp) {
  pPage = pp;
  regenImage();
  update();
}

void PDFPreviewWidget::wheelEvent(QWheelEvent* event) {
  if(pPage != NULL) {
    if(event->delta() > 0)
      currentPixmapSize += QSize(30, 30);
    else if(event->delta() < 0)
      currentPixmapSize -= QSize(30, 30);

    if(currentPixmapSize.width() < 150 || currentPixmapSize.height() < 150) {
      currentPixmapSize = QSize(150, 150);
    }

    regenImage();
    update();
  }
}

void PDFPreviewWidget::paintEvent(QPaintEvent* event) {
  if(pPage != NULL) {

    QPainter painter(this);
    QPixmap pixmap = QPixmap::fromImage(previewImage);
    pixmap = pixmap.scaled(currentPixmapSize, Qt::KeepAspectRatio);
    currentPixmapSize = pixmap.size();
    painter.drawPixmap(QRect((size().width() - pixmap.width()) / 2,
                      (size().height() - pixmap.height()) / 2,
                      pixmap.width(), pixmap.height()), pixmap);
  }
}


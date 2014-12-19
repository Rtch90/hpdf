#include <QtWidgets>
#include "pdf_preview_widget.h"

PDFPreviewWidget::PDFPreviewWidget(QWidget* parent) : QFrame(parent) {
  setCursor(Qt::OpenHandCursor);
  setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
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

void PDFPreviewWidget::regenPixmap(void) {
  pixmap = QPixmap::fromImage(previewImage);
  pixmap = pixmap.scaled(currentPixmapSize, Qt::KeepAspectRatioByExpanding);
  currentPixmapSize = pixmap.size();
}

void PDFPreviewWidget::repositionPixmap(void) {
  currentPixmapPos = QPoint((size().width() - pixmap.width()) / 2,
                     (size().height() - pixmap.height()) / 2);
}

void PDFPreviewWidget::previewUpdate(Poppler::Page* pp) {
  pPage = pp;
  regenImage();
  regenPixmap();
  repositionPixmap();
  update();
}

void PDFPreviewWidget::resizeEvent(QResizeEvent* event) {
  if(pPage != NULL) {
    repositionPixmap();
    update();
  }
}

void PDFPreviewWidget::wheelEvent(QWheelEvent* event) {
  if(pPage != NULL) {
    qDebug() << currentPixmapSize;
    if(event->delta() > 0)
      currentPixmapSize += QSize(30, 30);
    else if(event->delta() < 0)
      currentPixmapSize -= QSize(30, 30);

    if(currentPixmapSize.width() < 150 || currentPixmapSize.height() < 150) {
      currentPixmapSize = QSize(150, 150);
    }

    regenImage();
    regenPixmap();
    repositionPixmap();
    update();
  }
}

void PDFPreviewWidget::mousePressEvent(QMouseEvent* event) {
  if(pPage != NULL) {
    setCursor(Qt::ClosedHandCursor);

    if(event->button() == Qt::LeftButton) {
      dragStartPos = event->pos();
      lastPixmapPos = currentPixmapPos;
    }
  }
}

void PDFPreviewWidget::mouseMoveEvent(QMouseEvent* event) {
  if(pPage != NULL) {
    if(!(event->buttons() & Qt::LeftButton)) {
      return;
    }

    QPoint currentPos = event->pos();
    QPoint vector     = currentPos - dragStartPos;
    QPoint newPos     = lastPixmapPos + vector;

    if(pixmap.width() > size().width()) {
      if(newPos.x() <= 0 && newPos.x() >= size().width() - pixmap.width())
        currentPixmapPos.setX(newPos.x());
    }

    if(pixmap.height() > size().height()) {
      if(newPos.y() <= 0 && newPos.y() >= size().height() - pixmap.height())
        currentPixmapPos.setY(newPos.y());
    }

    update();
  }
}

void PDFPreviewWidget::mouseReleaseEvent(QMouseEvent* event) {
  setCursor(Qt::OpenHandCursor);
}

void PDFPreviewWidget::paintEvent(QPaintEvent* event) {
  if(pPage != NULL) {

    QPainter painter(this);
    painter.drawPixmap(QRect(currentPixmapPos.x(), currentPixmapPos.y(),
          pixmap.width(), pixmap.height()), pixmap);

    QFrame::paintEvent(event);
  }
}


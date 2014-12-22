#include <QtWidgets>
#include "PDFPreviewWidget.h"

PreviewGen pgen;

PDFPreviewWidget::PDFPreviewWidget(QWidget *parent) :
  QFrame(parent)
{

  setCursor(Qt::OpenHandCursor);
  setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
  pPage = NULL;
  currentPixmapSize = QSize(0,0);
  connect(&pgen, SIGNAL(updatePreview(QImage)),
          this, SLOT(updateImage(QImage)));
}

void PDFPreviewWidget::updateImage(QImage qimg) {
  previewImage = qimg;
  regenPixmap();
  repositionPixmap();
  update();
}

void PDFPreviewWidget::regenImage() {
  double dpi;
  double dpi2;
  QSize targetSize;

  if (currentPixmapSize == QSize(0,0))
    currentPixmapSize = this->size();

  targetSize = currentPixmapSize;

  QSizeF oriSize = pPage->pageSizeF();
  double oriDpi = 72;
  dpi = targetSize.height()/(float)oriSize.height()*oriDpi;
  dpi2= targetSize.width()/(float)oriSize.width()*oriDpi;
  dpi = dpi<dpi2?dpi:dpi2;

  //previewImage = pPage->renderToImage(dpi,dpi, -1, -1, -1, -1, rotation);
  pgen.render(pPage,dpi,rotation);
  pgen.start();
}

void PDFPreviewWidget::regenPixmap() {
  pixmap = QPixmap::fromImage(previewImage);
  pixmap = pixmap.scaled(currentPixmapSize, Qt::KeepAspectRatioByExpanding);
  currentPixmapSize = pixmap.size();
}

void PDFPreviewWidget::repositionPixmap() {
  currentPixmapPos = QPoint((size().width() - pixmap.width()) / 2, (size().height() - pixmap.height()) / 2);
}

void PDFPreviewWidget::previewUpdate(Poppler::Page* pp, Poppler::Page::Rotation rotation) {
  pPage = pp;
  this->rotation = rotation;
  currentPixmapSize = this->size();
  regenImage();
  //regenPixmap();
  //repositionPixmap();
  //update();
}

void PDFPreviewWidget::checkPreviewUpdate(Poppler::Page* pp, Poppler::Page::Rotation rotation) {
  //if (pPage != NULL && pPage == pp) {
  previewUpdate(pp, rotation);
  //}
}

void PDFPreviewWidget::checkPagePreviewExisted(Poppler::Page* pp) {
  (void) pp;
  //if (pPage != NULL && pPage == pp) {
  pPage = NULL;
  rotation = Poppler::Page::Rotate0;

  QPainter painter(this);
  painter.drawPixmap(QRect(0, 0, 0, 0), pixmap);
  update();
  //}
}

void PDFPreviewWidget::resizeEvent(QResizeEvent *event) {
  (void) event;
  if (pPage!=NULL) {
    repositionPixmap();
    update();
  }
}

void PDFPreviewWidget::wheelEvent(QWheelEvent *event) {
  if (pPage!=NULL) {
    if (event->delta() > 0)
      currentPixmapSize += QSize(30, 30);
    else if (event->delta() < 0)
      currentPixmapSize -= QSize(30, 30);

    if (currentPixmapSize.width() < 150 || currentPixmapSize.height() < 150) {
      currentPixmapSize = QSize(150, 150);
    }

    regenImage();
    regenPixmap();
    repositionPixmap();
    update();
  }
}

void PDFPreviewWidget::mousePressEvent(QMouseEvent *event)
{
  if (pPage!=NULL) {
    setCursor(Qt::ClosedHandCursor);

    if (event->button() == Qt::LeftButton) {
      dragStartPos = event->pos();
      lastPixmapPos = currentPixmapPos;
    }
  }
}

void PDFPreviewWidget::mouseMoveEvent(QMouseEvent *event) {
  if (pPage!=NULL) {
    if (!(event->buttons() & Qt::LeftButton)) {
      return;
    }

    QPoint currentPos = event->pos();
    QPoint vector = currentPos - dragStartPos;
    QPoint newPos = lastPixmapPos + vector;

    if (pixmap.width() > size().width()) {
      if (newPos.x() <= 0 && newPos.x() >= size().width() - pixmap.width())
        currentPixmapPos.setX(newPos.x());
    }

    if (pixmap.height() > size().height()) {
      if (newPos.y() <= 0 && newPos.y() >= size().height() - pixmap.height())
        currentPixmapPos.setY(newPos.y());
    }

    update();
  }
}

void PDFPreviewWidget::mouseReleaseEvent(QMouseEvent *event) {
  (void) event;
  setCursor(Qt::OpenHandCursor);
}

void PDFPreviewWidget::paintEvent(QPaintEvent *event) {
  (void) event;
  if (pPage!=NULL) {
    QPainter painter(this);
    painter.drawPixmap(QRect(currentPixmapPos.x(), currentPixmapPos.y(), pixmap.width(), pixmap.height()), pixmap);
  }
  QFrame::paintEvent(event);
}

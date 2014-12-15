#include <QtGlobal>
#include <QtWidgets>
#include "pdf_file.h"

PDFFile::PDFFile(const QString& fileName) {
  /* Load PDF file. */
  document = Poppler::Document::load(fileName);
  if(!document || document->isLocked()) {
    delete document;
  }
}

QImage* PDFFile::getPageImage(int pageNumber) {
  if(document == 0)
    return NULL;

  Poppler::Page* pdfPage = document->page(pageNumber);

  if(pdfPage == 0)
    return NULL;

  QImage* image = new QImage();
  *image = pdfPage->renderToImage();

  delete pdfPage;

  return image;
}


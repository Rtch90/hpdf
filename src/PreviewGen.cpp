#include <QtGlobal>
#include <QtWidgets>
#include "PreviewGen.h"
PreviewGen::PreviewGen()
{

}
void PreviewGen::run(){
  mutex.lock();
  Poppler::Page *pp =pPage;
  double instantDpi =dpi;
  double instantRotation = rotation;
  mutex.unlock();

  QImage pageImage = pp->renderToImage(instantDpi, instantDpi,-1,-1,-1,-1,rotation);
  emit updatePreview(pageImage);
}
void PreviewGen::render(Poppler::Page* pp,double aDpi,Poppler::Page::Rotation r){
  mutex.lock();
  pPage = pp;
  dpi = aDpi;
  rotation = r;
  mutex.unlock();
}


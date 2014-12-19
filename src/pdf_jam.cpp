#include <QtGlobal>
#include <QtWidgets>
#include <unistd.h>
#include <stdlib.h>
#include "pdf_jam.h"

PDFJam::PDFJam(void) {
  
}

/* Ensure the folder exists. */
void PDFJam::makeFolder(QString path) {
  QString tmp = "mkdir -p %1 && rm %2*";
  int value = system(tmp.arg(path).arg(path).toStdString().c_str());
  /*if(value != 0)
    qDebug() << "ERROR: Failed to make " << path;*/
}

void PDFJam::pushCommand(QString cmd) {
  mutex.lock();
  cmdQueue.push_back(cmd);
  mutex.unlock();
  if(!isRunning()) start();
}

/* Rotate PDF page clockwise. */
bool PDFJam::rotatePage(int fileIndex, int rotatedPageIndex, int degree) {
  if((degree != 90) && (degree != 180) && (degree != 270)) {
    return false;
  }

  QString rtTmp = "pdf%1 /tmp/hpdf/%2/%3.pdf --outfile /tmp/hpdf/%4/%5.pdf ";
  QString cmd = rtTmp.arg(QString::number(degree)).arg(QString::number(fileIndex))
                .arg(QString::number(rotatedPageIndex)).arg(QString::number(fileIndex))
                .arg(QString::number(rotatedPageIndex));
  pushCommand(cmd);
}

/* Remove page in a pdf file. */
bool PDFJam::removePage(int fileIndex, int numPages, int deletedPageIndex) {
  if((deletedPageIndex < 0) || (deletedPageIndex >= numPages)) {
    return false;
  }
  /*QString tmp = "file1.pdf '-' file2.pdf '1,2' file3.pdf '2-2' --outfile output.pdf"*/
  QString rmTmp = "rm /tmp/hpdf/%1/%2.pdf ";
  QString cmd   = rmTmp.arg(QString::number(fileIndex)).arg(QString::number(deletedPageIndex));
  QString tmp   = "mv /tmp/hpdf/%1/%2.pdf /tmp/hpdf/%3/%4.pdf ";

  for(int i = deletedPageIndex+1; i < numPages; i++) {
    cmd += "&& " + tmp.arg(QString::number(fileIndex)).arg(QString::number(i))
                          .arg(QString::number(fileIndex)).arg(QString::number(i-1));
  }

  pushCommand(cmd);
}

void PDFJam::cutPage(int fileIndex, int numPages, int pageIndex) {
  if((pageIndex < 0) || (pageIndex >= numPages)) {
    return;
  }

  copyPage(fileIndex, numPages, pageIndex);
  removePage(fileIndex, numPages, pageIndex);
}

void PDFJam::copyPage(int fileIndex, int numPages, int pageIndex) {
  QString cpTmp = "cp /tmp/hpdf/%1%2.pdf /tmp/hpdf/clipboard.pdf ";
  QString cmd   = cpTmp.arg(QString::number(fileIndex)).arg(QString::number(pageIndex));
  pushCommand(cmd);
}

void PDFJam::pastePage(int fileIndex, int numPages, int pageIndex) {
  /* TODO: Check if the clipboard file exists. */
  QString cmd   = "";
  QString mvTmp = "mv /tmp/hpdf/%1/%2.pdf /tmp/hpdf/%3/%4.pdf ";
  for(int i = numPages-1; i >= pageIndex; i--) {
    cmd += mvTmp.arg(QString::number(fileIndex)).arg(QString::number(i))
                    .arg(QString::number(fileIndex)).arg(QString::number(i+1));
    if(i > pageIndex) cmd += " && ";
  }

  QString pasteTmp = "cp /tmp/hpdf/clipboard.pdf /tmp/hpdf/%1/%2.pdf ";
  cmd += " && " + pasteTmp.arg(QString::number(fileIndex)).arg(QString::number(pageIndex));
  pushCommand(cmd);
}

void PDFJam::movePage(int fromFileIndex, int fromFileNumPage, int fromPageIndex,
      int toFileIndex, int toFileNumPage, int toPageIndex) {
  /* TODO: Back up clipboard. */
  /* If this page is moving with files update to file index. */
  if(toFileIndex == fromFileIndex) {
    toFileNumPage--;
    if(toPageIndex > fromPageIndex)
      toPageIndex--;
    toPageIndex--;
  }

  cutPage(fromFileIndex, fromFileNumPage, fromPageIndex);
  pastePage(toFileIndex, toFileNumPage, toPageIndex);
}

void PDFJam::savePageAsImage(Poppler::Page pp, QString dst, double dpi = 72) {

}


/* Export file number 'fileIndex' to destination. */
/* Supported n-up, orientation, offset options. */
void PDFJam::exportFile(int fileIndex, int numPages, QString dst,
      QSize nup = QSize(1,1), bool isLandscape = false, bool hasTwoSideOffset = false,
      int leftOffset = 0, int rightOffset = 0) {

  QString cmd = "pdfjam ";
  QString tmp = "/tmp/hpdf/%1/%2.pdf '-' ";

  for(int i = 0; i < numPages; i++) {
    cmd += tmp.arg(QString::number(fileIndex)).arg(QString::number(i));
  }

  QString orientation = isLandscape?" --landscape ": " --no-landscape ";
  cmd = orientation;
  if((nup.width() == 1) || (nup.height() != 1)) {
    QString nupTmp = " --nup '%1x%1' --frame true ";
    cmd += nupTmp.arg(QString::number(nup.width())).arg(QString::number(nup.height()));
  }

  QString outTmp = " --outfile %1 ";
  cmd += outTmp.arg(dst);
  pushCommand(cmd);
  /* Offset comes next. */
}

void PDFJam::loadFile(QString fileName, int fileIndex, Poppler::Document* pd) {
  int numPages = pd->numPages();
  QString path = "/tmp/hpdf/%1/";
  path = path.arg(QString::number(fileIndex));
  makeFolder(path);

  QString tmp = "pdfjam %1 %2 --outfile %3%4.pdf %5";
  QString cmd = "";
  for(int i = 0; i <= numPages; i++) {
    QString orientation = " --no-landscape ";
    QSizeF pageSize = pd->page(i)->pageSizeF();
    if(pageSize.width() > pageSize.height()) {
      orientation = " --landscape ";
    }
    cmd += tmp.arg(fileName).arg(QString::number(i+1)).arg(path)
                   .arg(QString::number(i)).arg(orientation) + " ; ";
  }
  pushCommand(cmd);

  /* Test the backend functions. */
  rotatePage(0, 5, 270);
  movePage(0, numPages, 5, 0, numPages, 10);
  /*removePage(0, numPages, 5);*/
  exportFile(0, numPages, "/home/allanis/conco.pdf", QSize(2, 2), true, true, 1, 0);
  /* End test. */
}

QString PDFJam::nextCommand(void) {
  QString cmd;
  mutex.lock();
  cmd = cmdQueue.first();
  cmdQueue.pop_front();
  mutex.unlock();
  return cmd;
}

bool PDFJam::isQueueEmpty(void) {
  return (cmdQueue.size()==0);
}

void PDFJam::run(void) {
  while(!isQueueEmpty()) {
    QString cmd = nextCommand();
    int value = system(cmd.toStdString().c_str());
    if(value != 0)
      qDebug() << "ERROR: Failed to execute " << cmd;
    else
      qDebug() << "SUCCESS: executed " << cmd;
  }
}


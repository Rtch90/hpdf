#include <QtGlobal>
#include <QtWidgets>
#include <unistd.h>
#include <stdlib.h>
#include "pdf_jam.h"

PDFJam::PDFJam(void) {
  
}

/* Ensure the folder exists. */
void PDFJam::makeFolder(QString path) {
  QString tmp = "mkdir -p %1";
  int value = system(tmp.arg(path).toStdString().c_str());
  if(value != 0)
    qDebug() << "ERROR: Failed to make " << path;
}

void PDFJam::pushCommand(QString cmd) {
  mutex.lock();
  cmdQueue.push_back(cmd);
  mutex.unlock();
  if(!isRunning()) start();
}

void PDFJam::loadFile(QString fileName, int fileNum, int numPages) {
  QString path = "/tmp/hpdf/%1/";
  path = path.arg(QString::number(fileNum));
  makeFolder(path);

  QString tmp = "pdfjam %1 %2 -- outfile %3%4.pdf";
  QString cmd = "";
  for(int i = 1; i <= numPages; i++) {
    cmd += tmp.arg(fileName).arg(QString::number(i)).arg(path).arg(QString::number(i)) + ";";
  }
  pushCommand(cmd);
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

  /*system("echo 'tata' > /tmp/test.txt");*/
}


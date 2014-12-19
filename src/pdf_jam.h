#pragma once
#include <QThread>
#include <QMutex>
#include <QImage>
#include <poppler-qt5.h>

class PDFPageWidget;
class QImage;

class PDFJam : public QThread {
  Q_OBJECT
public:
  PDFJam(void);

  void pushCommand(QString);
  void loadFile(QString fileName, int, Poppler::Document*);
  void exportFile(int, int, QString, QSize, bool, bool, int, int);
  void savePageAsImage(Poppler::Page pp, QString dst, double);
  void movePage(int, int, int, int, int, int);
  void pastePage(int, int, int);
  void copyPage(int, int, int);
  void cutPage(int, int, int);
  
  bool removePage(int, int, int);
  bool rotatePage(int, int, int);
  void makeFolder(QString);

protected:
  void run(void);

private:
  QVector<QString> cmdQueue;
  QString nextCommand(void);
  bool isQueueEmpty(void);
  QMutex mutex;
};


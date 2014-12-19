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
  void loadFile(QString fileName, int, int);
  void exportFile(int, int, QString);
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


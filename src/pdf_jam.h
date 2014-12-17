#pragma once
#include <QThread>
#include <QMutex>
#include <poppler-qt5.h>
#include <QImage>

class PDFPageWidget;
class QImage;

class PDFJam : public QThread {
  Q_OBJECT
public:
  PDFJam(void);

  void pushCommand(QString);
  void loadFile(QString fileName, int, int);
  void makeFolder(QString);

protected:
  void run(void);

private:
  QVector<QString> cmdQueue;
  QString nextCommand();
  bool isQueueEmpty();
  QMutex mutex;
};


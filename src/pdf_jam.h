#pragma once

class PDFJam { /*:public QThread {*/
public:
  PDFJam(void);

protected:
  void run(void);

private:
  volatile bool stopped;

};


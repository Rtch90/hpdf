#pragma once

class PDFJam {/*: public QThread {*/
  /*Q_OBJECT*/
public:
  PDFJam(void);

protected:
  void run(void);

private:
  volatile bool stopped;
};


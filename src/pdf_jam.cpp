#include <QtGlobal>
#include <QtWidgets>
#include <unistd.h>
#include <stdlib.h>
#include "pdf_jam.h"

PDFJam::PDFJam(void) {
  
}

void PDFJam::run(void) {
#if 0
  if((pid = fork()) < 0) {
    
  }
  else if(pid > 0) {
    /* Parent. */
  } else {
    /* Child. */
    system("echo 'tata' > /tmp/test.txt");
  }
#endif
}


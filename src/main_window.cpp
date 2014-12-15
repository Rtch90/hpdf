#include "main_window.h"
#include "ui_main_window.h"

MainWindow::Mainwindow(Qwidget* parent) :
    QMainWindow(parent),
    ui(new UI::MainWindow {
   
  ui->setupUI(this);
}

MainWindow::~MainWindow(void) {
  delete ui;
}


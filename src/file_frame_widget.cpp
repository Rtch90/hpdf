#include <QDebug>
#include <QSizePolicy>
#include "file_frame_widget.h"

#define COLLAPSE_BUTTON_WIDTH   60
#define COLLAPSE_BUTTON_HEIGHT  40

FileFrameWidget::FileFrameWidget(QWidget* parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  topLayout   = new QGridLayout();

  scrollArea  = new QScrollArea();
  mainChild   = new FileWidget();

  widgetName  = new QLabel();
  widgetName->setText(tr("File 1"));
  topLayout->addWidget(widgetName, 0, 1);

  collapseButton = new QPushButton(tr("X"));
  collapseButton->setMinimumSize(QSize(COLLAPSE_BUTTON_WIDTH, COLLAPSE_BUTTON_HEIGHT));
  collapseButton->setMaximumSize(QSize(COLLAPSE_BUTTON_WIDTH, COLLAPSE_BUTTON_HEIGHT));
  connect(collapseButton, SIGNAL(released()), this, SLOT(collapsedButtonClick()));
  topLayout->addWidget(collapseButton, 0, 0);

  topLayout->addWidget(scrollArea, 1, 0, 1, 5);
  scrollArea->setWidget(mainChild);

  setLayout(topLayout);

  setCollapsed(false);
  adjustSize();
}

QSize FileFrameWidget::sizeHint() const {
  if(collapsed == true)
    return QSize(mainChild->width(), collapseButton->height());
  else
    return QSize(mainChild->width(), collapseButton->height() + mainChild->height() + 50);
}

void FileFrameWidget::setCollapsed(bool state) {
  if(state == true) {
    collapsed = true;
    scrollArea->hide();
  } else {
    collapsed = false;
    scrollArea->show();
  }

  adjustSize();
}

void FileFrameWidget::collapsedButtonClick(void) {
  if(collapsed == true)
    setCollapsed(false);
  else
    setCollapsed(true);
}


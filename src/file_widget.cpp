#include <QPixmap>
#include <QLabel>
#include <QSize>
#include <QDrag>
#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>

#include "file_widget.h"

#define CHILD_AREA_WIDTH  150
#define CHILD_AREA_HEIGHT 180

FileWidget::FileWidget(QWidget* parent) {
  setAcceptDrops(true);
  mainLayout = new QHBoxLayout();

  for(int i = 0; i < 4; i++) {
    addChild(tr("Child number #") + QString::number(i));
  }

  setLayout(mainLayout);
}

int FileWidget::getChildCount() const {
  return child.size();
}

QSize FileWidget::sizeHint() const {
  return QSize(CHILD_AREA_WIDTH*getChildCount(), CHILD_AREA_HEIGHT + 50);
}

void FileWidget::addChild(QString name) {
  QLabel* newchild;
  newchild = new QLabel();
  newchild->setText(name);
  newchild->setStyleSheet("QLabel { background-color : red; color : blue; }");

  child.push_back(newchild);

  mainLayout->addWidget(newchild);
}

void FileWidget::dragEnterEvent(QDragEnterEvent* event) {
  if(event->mimeData()->hasFormat("text/plain"))
    event->acceptProposedAction();
}

void FileWidget::dropEvent(QDropEvent* event) {
  int from  = event->mimeData()->text().toInt();
  int to    = findChildPositionInLayout(child[findClickEventChild(event->pos())]);

  qDebug() << QString::number(from) + " " + QString::number(to);

  mainLayout->removeWidget(child[from]);
  mainLayout->insertWidget(to, child[from]);
  
  event->acceptProposedAction();
}

void FileWidget::mousePressEvent(QMouseEvent* event) {
  if(event->button() == Qt::LeftButton) {
    int draggedChild = (findClickEventChild(event->pos()));

    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;

    mimeData->setText(QString::number(draggedChild));
    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap(":/img/copy.png"));

    Qt::DropAction dropAction = drag->exec();
  }
}

int FileWidget::findClickEventChild(QPoint pos) {
  for(int i = 0; i < getChildCount(); i++)
    if(child[i]->geometry().contains(pos))
      return i;
  
  return getChildCount()-1;
}

int FileWidget::findChildPositionInLayout(QLabel* child) {
  for(int i = 0; i < getChildCount(); i++)
    if(mainLayout->itemAt(i)->widget() == child)
      return i;

  return getChildCount()-1;
}


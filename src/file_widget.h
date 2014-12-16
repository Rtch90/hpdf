#include <QWidget>
#include <QHBoxLayout>
#include <QImage>
#include <QString>
#include <QPoint>
#include <vector>
#include "pdf_page_widget.h"

class FileWidget : public QWidget {
  Q_OBJECT
public:
  FileWidget(QWidget* parent = 0);
  QSize sizeHint() const;
protected:
  void dragEnterEvent(QDragEnterEvent* event);
  void dropEvent(QDropEvent* event);
  void mousePressEvent(QMouseEvent* event);
private:
  std::vector<PDFPageWidget*> child;

  QHBoxLayout* mainLayout;

  void addChild(QString name);
  int  findClickEventChild(QPoint pos);
  int  findChildPositionInLayout(PDFPageWidget* child);
  int  getChildCount() const;
};


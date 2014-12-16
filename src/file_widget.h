#include <QWidget>
#include <QHBoxLayout>
#include <QImage>
#include <QString>
#include <QLabel>
#include <QPoint>
#include <vector>

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
  std::vector<QLabel*> child;
  
  QHBoxLayout* mainLayout;

  void addChild(QString name);
  int  findClickEventChild(QPoint pos);
  int  findChildPositionInLayout(QLabel* child);
  int  getChildCount() const;
};


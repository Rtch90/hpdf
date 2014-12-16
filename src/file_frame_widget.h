#include <QWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

#include "file_widget.h"

class FileFrameWidget : public QWidget {
  Q_OBJECT
  Q_PROPERTY(bool collapsed READ isCollapsed WRITE setCollapsed)

public:
  FileFrameWidget(QWidget* parent = 0);
  QSize sizeHint(void) const;
  
  bool isCollapsed() { return collapsed; }
  void setCollapsed(bool collapsed);

protected:
  
private slots:
  void collapsedButtonClick(void);
private:
  QGridLayout* topLayout;

  QLabel*       widgetName;
  QPushButton*  collapseButton;
  QScrollArea*  scrollArea;
  FileWidget*   mainChild;

  bool          collapsed;
};


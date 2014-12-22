#pragma once
#include <QDialog>
#include "PDFFileWidget.h"
#include "PDFJam.h"

class QGridLayout;
class QGroupBox;
class QListWidget;
class QLineEdit;
class QRadioButton;

class PDFExportDialog : public QDialog {
  Q_OBJECT

public:
  explicit PDFExportDialog(QWidget* parent = 0);

private:
  QVector<PDFFileWidget*>     fileWidgets;
  QVector<QString>            fileNames;
  QVector<int>                fileIndices;
  QVector<QVector<QVariant> > options;

private:
  PDFJam pdfJam;
  QGroupBox *fileListBox, *landscapeBox, *nupBox, *offsetBox;

  QPushButton*  btnSave, *btnSaveAll, *btnCancel;
  QGridLayout*  mainLayout;

  QListWidget*  fileList;
  QLineEdit*    txtRow, *txtCol;
  QRadioButton* chkPortrait, *chkLandscape;
  QLineEdit*    txtLeftOffset;

public:
  void setFilesToExport(QVector<PDFFileWidget*> fileWidgets, QVector<QString> fileNames, QVector<int> fileIndices);

private slots:
  void fileListChanged(int);
  void btnSaveClicked(void);
  void btnSaveAllClicked(void);
  void btnCancelClicked(void);
  void txtRowChanged(const QString&);
  void txtColChanged(const QString&);
  void txtLeftChanged(const QString&);
  void chkLandscapeChanged(bool);
  void nupBoxToggled(bool);
  void offsetBoxToggled(bool);
};

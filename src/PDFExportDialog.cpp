#include <QtWidgets>
#include <QtGlobal>
#include "PDFExportDialog.h"

PDFExportDialog::PDFExportDialog(QWidget *parent) :
  QDialog(parent) {
  mainLayout = new QGridLayout();
  mainLayout->setSpacing(15);

  // File list
  fileListBox = new QGroupBox("Selected files");
  QVBoxLayout *fileListLayout = new QVBoxLayout();
  fileList = new QListWidget();
  connect(fileList, SIGNAL(currentRowChanged(int)), this, SLOT(fileListChanged(int)));
  fileListLayout->addWidget(fileList);
  fileListLayout->setContentsMargins(0, 20, 6, 6);
  fileListBox->setLayout(fileListLayout);
  mainLayout->addWidget(fileListBox, 0, 0, 4, 2);

  // is landscape
  landscapeBox = new QGroupBox("Output orientation");
  QVBoxLayout *landscapeLayout = new QVBoxLayout();
  chkPortrait = new QRadioButton("Portrait");
  chkPortrait->setChecked(true);
  chkLandscape = new QRadioButton("Landscape");
  connect(chkLandscape, SIGNAL(toggled(bool)), this, SLOT(chkLandscapeChanged(bool)));
  landscapeLayout->addWidget(chkPortrait);
  landscapeLayout->addWidget(chkLandscape);
  landscapeBox->setLayout(landscapeLayout);

  // n-up
  nupBox = new QGroupBox("N-up option");
  nupBox->setCheckable(true);
  nupBox->setChecked(false);
  connect(nupBox, SIGNAL(toggled(bool)), this, SLOT(nupBoxToggled(bool)));
  QGridLayout *nupLayout = new QGridLayout();
  QLabel *lblRow = new QLabel("Col:");
  QLabel *lblCol = new QLabel("Row:");
  txtRow = new QLineEdit("1");
  connect(txtRow, SIGNAL(textEdited(const QString &)), this, SLOT(txtRowChanged(const QString &)));
  txtCol = new QLineEdit("1");
  connect(txtCol, SIGNAL(textEdited(const QString &)), this, SLOT(txtColChanged(const QString &)));
  nupLayout->addWidget(lblRow, 0, 0, 1, 1);
  nupLayout->addWidget(txtRow, 0, 1, 1, 2);
  nupLayout->addWidget(lblCol, 1, 0, 1, 1);
  nupLayout->addWidget(txtCol, 1, 1, 1, 2);
  nupBox->setLayout(nupLayout);

  // offsets
  offsetBox = new QGroupBox("Two-sided offset");
  offsetBox->setCheckable(true);
  offsetBox->setChecked(false);
  connect(offsetBox, SIGNAL(toggled(bool)), this, SLOT(offsetBoxToggled(bool)));
  QGridLayout *offsetLayout = new QGridLayout();
  QLabel *lblLeft = new QLabel("Left Offset:");
  txtLeftOffset = new QLineEdit("0");
  connect(txtLeftOffset, SIGNAL(textEdited(const QString &)), this, SLOT(txtLeftChanged(const QString &)));
  offsetLayout->addWidget(lblLeft, 0, 0, 1, 1);
  offsetLayout->addWidget(txtLeftOffset, 0, 1, 1, 2);
  offsetBox->setLayout(offsetLayout);

  QVBoxLayout *midLayout = new QVBoxLayout();
  midLayout->addWidget(landscapeBox);midLayout->addWidget(nupBox);midLayout->addWidget(offsetBox);
  midLayout->addStretch(1);
  midLayout->setSpacing(10);
  QFrame *midFrame = new QFrame();
  midFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
  midFrame->setLayout(midLayout);
  mainLayout->addWidget(midFrame, 0, 2, 4, 2);

  // buttons
  QVBoxLayout *rightLayout = new QVBoxLayout();
  btnSave = new QPushButton("&Export...");
  connect(btnSave, SIGNAL(clicked()), this, SLOT(btnSaveClicked()));
  rightLayout->addWidget(btnSave);
  btnSaveAll = new QPushButton("Export &all files using\nthese settings...");
  connect(btnSaveAll, SIGNAL(clicked()), this, SLOT(btnSaveAllClicked()));
  rightLayout->addWidget(btnSaveAll);
  btnCancel = new QPushButton("&Done");
  connect(btnCancel, SIGNAL(clicked()), this, SLOT(btnCancelClicked()));
  rightLayout->addWidget(btnCancel);
  rightLayout->addStretch(1);
  mainLayout->addLayout(rightLayout, 0, 4, 2, 1);

  setLayout(mainLayout);
  setGeometry(400, 400, 800, 450);
  setWindowTitle("Export to file");
}

void PDFExportDialog::setFilesToExport(QVector<PDFFileWidget*> fileWidgets,
                                       QVector<QString> fileNames,
                                       QVector<int> fileIndices) {
  this->fileWidgets = fileWidgets;
  this->fileNames = fileNames;
  this->fileIndices = fileIndices;

  QStringList modelList;

  foreach (const QString str, fileNames) {
    modelList << str;

    QVector<QVariant> option; // Landscape, row, col, left, right, twosided
    bool ok;
    option.append(QVariant(chkLandscape->isChecked()));
    option.append(QVariant(nupBox->isChecked()));
    option.append(QVariant(txtRow->text().toInt(&ok)));
    option.append(QVariant(txtCol->text().toInt(&ok)));
    option.append(QVariant(offsetBox->isChecked()));
    option.append(QVariant(txtLeftOffset->text().toInt(&ok)));

    options.append(option);
  }

  fileList->addItems(modelList);
}

void PDFExportDialog::btnSaveClicked(void) {
  int selectedIndex = fileList->currentRow();
  QVector<QVariant> option = options.at(selectedIndex);

  QString fileName = QFileDialog::getSaveFileName(this,
                                                  tr("Save PDF file"), ".",
                                                  tr("PDF file (*.pdf)"));
  if (!fileName.isEmpty()) {
    QSize nup;
    //if (option.at(0).toBool())
    //nup = QSize(option.at(3).toInt(), option.at(2).toInt());
    //else
    nup = QSize(option.at(2).toInt(), option.at(3).toInt());

    pdfJam.exportFile(fileIndices.at(selectedIndex), fileWidgets.at(selectedIndex)->getChildCount(), fileName,
                      option.at(1).toBool(), nup, // nup
                      option.at(0).toBool(), // landscape
                      option.at(4).toBool(), option.at(5).toInt()); //offset

    QMessageBox::information(this, tr("PDFFactory"), tr("Exported succesfully to\n%1.").arg(fileName));
  }
}

void PDFExportDialog::btnSaveAllClicked(void) {
  bool ok;
  for (int i = 0; i < fileNames.size(); i++) {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save PDF file"), ".",
                                                    tr("PDF file (*.pdf)"));

    QSize nup;
    //if (chkLandscape->isChecked())
    //nup = QSize(txtCol->text().toInt(&ok), txtRow->text().toInt(&ok));
    //else
    nup = QSize(txtRow->text().toInt(&ok), txtCol->text().toInt(&ok));

    if (!fileName.isEmpty()) {
      pdfJam.exportFile(fileIndices.at(i), fileWidgets.at(i)->getChildCount(), fileName,
                        nupBox->isChecked(), nup,
                        chkLandscape->isChecked(),
                        offsetBox->isChecked(), txtLeftOffset->text().toInt(&ok));
      QMessageBox::information(this, tr("PDFFactory"), tr("Exported succesfully to\n%1.").arg(fileName));
    }
  }
}

void PDFExportDialog::btnCancelClicked(void) {
  accept();
}

void PDFExportDialog::fileListChanged(int row) {
  QVector<QVariant> option = options.at(row);
  chkLandscape->setChecked(option.at(0).toBool());
  nupBox->setChecked(option.at(1).toBool());
  txtRow->setText(QString::number(option.at(2).toInt()));
  txtCol->setText(QString::number(option.at(3).toInt()));
  offsetBox->setChecked(option.at(4).toBool());
  txtLeftOffset->setText(QString::number(option.at(5).toInt()));
}

void PDFExportDialog::txtRowChanged(const QString &txt) {
  bool ok;
  QVector<QVariant> option = options.at(fileList->currentRow());
  option.remove(2);
  option.insert(2, QVariant(txt.toInt(&ok)));
  options.remove(fileList->currentRow());
  options.insert(fileList->currentRow(), option);
}

void PDFExportDialog::txtColChanged(const QString &txt) {
  bool ok;
  QVector<QVariant> option = options.at(fileList->currentRow());
  option.remove(3);
  option.insert(3, QVariant(txt.toInt(&ok)));
  options.remove(fileList->currentRow());
  options.insert(fileList->currentRow(), option);
}

void PDFExportDialog::txtLeftChanged(const QString &txt) {
  bool ok;
  QVector<QVariant> option = options.at(fileList->currentRow());
  option.remove(5);
  option.insert(5, QVariant(txt.toInt(&ok)));
  options.remove(fileList->currentRow());
  options.insert(fileList->currentRow(), option);
}

void PDFExportDialog::chkLandscapeChanged(bool state) {
  (void) state;
  QVector<QVariant> option = options.at(fileList->currentRow());
  option.remove(0);
  option.insert(0, QVariant(chkLandscape->isChecked()));
  options.remove(fileList->currentRow());
  options.insert(fileList->currentRow(), option);
}

void PDFExportDialog::nupBoxToggled(bool checked) {
  (void) checked;
  QVector<QVariant> option = options.at(fileList->currentRow());
  option.remove(1);
  option.insert(1, QVariant(nupBox->isChecked()));
  options.remove(fileList->currentRow());
  options.insert(fileList->currentRow(), option);
}

void PDFExportDialog::offsetBoxToggled(bool checked) {
  (void) checked;
  QVector<QVariant> option = options.at(fileList->currentRow());
  option.remove(4);
  option.insert(4, QVariant(offsetBox->isChecked()));
  options.remove(fileList->currentRow());
  options.insert(fileList->currentRow(), option);
}

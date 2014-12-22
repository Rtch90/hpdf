QT += widgets
TEMPLATE = app
TARGET = hpdf
DEPENDPATH += .
INCLUDEPATH += /usr/include/poppler/qt5
LIBS += -L/usr/lib -lpoppler-qt5

# Input
HEADERS += ../src/PDFExportDialog.h \
           ../src/HPDF.h \
           ../src/PDFFileWidget.h \
           ../src/PDFJam.h \
           ../src/PDFPageWidget.h \
           ../src/PDFPreviewWidget.h \
           ../src/PDFTableWidget.h \
           ../src/PreviewGen.h \
           ../src/ThumbGen.h

SOURCES += ../src/main.cpp \
           ../src/PDFExportDialog.cpp \
           ../src/HPDF.cpp \
           ../src/PDFFileWidget.cpp \
           ../src/PDFJam.cpp \
           ../src/PDFPageWidget.cpp \
           ../src/PDFPreviewWidget.cpp \
           ../src/PDFTableWidget.cpp \
           ../src/PreviewGen.cpp \
           ../src/ThumbGen.cpp

RESOURCES += hpdf.qrc

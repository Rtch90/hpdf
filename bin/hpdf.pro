QT += widgets

TEMPLATE = app
TARGET   = hpdf
DEPENDPATH    += .
INCLUDEPATH   += /usr/include/poppler/qt5
LIBS          += -L/usr/lib -lpoppler-qt5

HEADERS += \
  ../src/pdf_factory.h \
  ../src/pdf_file.h \
  ../src/pdf_page_widget.h

SOURCES += \
  ../src/main.cpp \
  ../src/pdf_factory.cpp \
  ../src/pdf_file.cpp \
  ../src/pdf_page_widget.cpp

RESOURCES += hpdf.qrc

CONFIG += console


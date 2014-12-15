QT += widgets

TEMPLATE = app
TARGET   = hpdf
DEPENDPATH  += .
INCLUDEPATH += .

HEADERS += \
  ../src/pdf_factory.h

SOURCES += \
  ../src/main.cpp \
  ../src/pdf_factory.cpp

RESOURCES += hpdf.qrc

CONFIG += console


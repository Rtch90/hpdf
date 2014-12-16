QT += widgets

TEMPLATE = app
TARGET   = hpdf
DEPENDPATH    += .
INCLUDEPATH   += /usr/include/poppler/qt5
LIBS          += -L/usr/lib -lpoppler-qt5

HEADERS += \
  ../src/file_widget.h \
  ../src/pdf_factory.h \
  ../src/pdf_page_widget.h \
  ../src/table_view.h \
  ../src/file_frame_widget.h

SOURCES += \
  ../src/main.cpp \
  ../src/file_widget.cpp \
  ../src/pdf_factory.cpp \
  ../src/pdf_page_widget.cpp \
  ../src/table_view.cpp \
  ../src/file_frame_widget.cpp

RESOURCES += hpdf.qrc

CONFIG += console


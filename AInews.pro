QT       += core gui widgets network xml
CONFIG   += c++14

TARGET = AInews
TEMPLATE = app

DEFINES  += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += \
	-Wno-missing-field-initializers

QMAKE_LFLAGS += ./data/*.o

SOURCES += main.cpp\
        mainwindow.cpp \
    dialoga.cc

HEADERS  += mainwindow.h \
    dialoga.h

FORMS    += mainwindow.ui \
    dialoga.ui

RESOURCES += \
    resources.qrc

LIBS += -ldl
LIBS += -L./ -L/home/jens/AInews -lmyhtml
LIBS += -L./ -L/home/jens/AInews -lssl -lcrypto

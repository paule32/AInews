QT       += core gui widgets network xml help
CONFIG   += c++14

TARGET = AInews
TEMPLATE = app

DEFINES  += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += \
	-Wno-missing-field-initializers

QMAKE_LFLAGS += ./data/*.o

SOURCES += main.cpp\
        mainwindow.cpp \
    dialoga.cc \
    dialoga.cc \
    main.cpp \
    mainwindow.cpp \
    help/helpbrowser.cc

HEADERS  += mainwindow.h \
    dialoga.h \
    dialoga.h \
    mainwindow.h \
    help/helpbrowser.h

FORMS    += mainwindow.ui \
    dialoga.ui

RESOURCES += \
    resources.qrc

LIBS += -ldl
LIBS += -L./ -L/home/jens/AInews -lmyhtml
LIBS += -L./ -L/home/jens/AInews -lssl -lcrypto

DISTFILES += \
    help/dBaseHelp.qch \
    help/dBaseHelp.qhc \
    help/dBaseHelp.qhp \
    help/append.html \
    help/index.html \
    help/printline.html \
    help/test.html \
    help/dBaseHelp.qhcp

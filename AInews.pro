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
    helpbrowser.cc \
    helppopup.cc \
    dockpagetopicsummary.cc \
    topicintervallcombobox.cc

HEADERS  += mainwindow.h \
    dialoga.h \
    helpbrowser.h \
    headers.h \
    helppopup.h \
    dockpagetopicsummary.h \
    topicintervallcombobox.h

FORMS    += mainwindow.ui \
    dialoga.ui \
    helppopup.ui

RESOURCES += \
    resources.qrc

LIBS += -ldl
LIBS += -L./ -L/home/jens/AInews -lmyhtml
LIBS += -L./ -L/home/jens/AInews -lssl -lcrypto

DISTFILES += \
    help/dBaseHelp.qch \
    help/dBaseHelp.qhc \
    help/append.html \
    help/index.html \
    help/printline.html \
    help/test.html \
    help/dBaseHelp.qhcp \
    help/help.qhp \
    help/help.qhcp

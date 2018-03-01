QT       += core gui widgets network xml help
CONFIG   += c++14

TARGET = AInews
TEMPLATE = app

DEFINES  += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += \
	-Wno-missing-field-initializers

QMAKE_LFLAGS += ./ssldata/*.o

INCLUDEPATH += ./ ./include

SOURCES += main.cpp\
    mainwindow.cpp \
    dialoga.cc \
    helpbrowser.cc \
    helppopup.cc \
    dockpagetopicsummary.cc \
    topicintervallcombobox.cc \
    spendendialog.cc \
    workerthread.cc \
    spendenlabel.cc
    

HEADERS  += mainwindow.h \
    dialoga.h \
    helpbrowser.h \
    headers.h \
    helppopup.h \
    dockpagetopicsummary.h \
    topicintervallcombobox.h \
    spendendialog.h \
    workerthread.h \
    spendenlabel.h \
    include/quazip/crypt.h \
    include/quazip/ioapi.h \
    include/quazip/JlCompress.h \
    include/quazip/quaadler32.h \
    include/quazip/quacrc32.h \
    include/quazip/quagzipfile.h \
    include/quazip/quaziodevice.h \
    include/quazip/quazip.h \
    include/quazip/quazip_global.h \
    include/quazip/quazipdir.h \
    include/quazip/quazipfile.h \
    include/quazip/quazipfileinfo.h \
    include/quazip/quazipnewinfo.h \
    include/quazip/unzip.h \
    include/quazip/zip.h

FORMS    += mainwindow.ui \
    dialoga.ui \
    helppopup.ui \
    spendendialog.ui

RESOURCES += \
    resources.qrc

LIBS += -ldl
LIBS += -L./ -L/home/jens/AInews -lmyhtml
LIBS += -L./ -L/home/jens/AInews -lssl -lcrypto
LIBS += -L./ -L/home/jens/AInews -lquazip

DISTFILES += \
    help/dBaseHelp.qch \
    help/dBaseHelp.qhc \
    help/append.html \
    help/index.html \
    help/printline.html \
    help/test.html \
    help/help.qhp \
    help/help.qhcp \
    ../build-AInews-Desktop_Qt_5_10_0_GCC_64bit-Debug/settings.ini \
    spenden.txt \
    info.html

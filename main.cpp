#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

#include <iostream>

#include <QObject>
#include <QApplication>
#include <QMainWindow>
#include <QSslSocket>
#include <QString>

#include "mainwindow.h"

MainWindow *mainwin = nullptr;
using namespace std;

int main(int argc, char *argv[])
{
    // ---------------------------------
    // IMPORTANT: set plugin path's ...
    // ---------------------------------  
    setenv("QT_QPA_PLATFORM_PLUGIN_PATH",
    "./plugins", 1);
    
    // ------------------------------------
    // after plugin's loaded, start app ...
    // ------------------------------------
    QApplication a(argc, argv);
    std::cout << QSslSocket::sslLibraryBuildVersionString().toStdString() << std::endl;
    
    a.addLibraryPath(QString("/usr/local/ssl/lib"));
    
    mainwin = new MainWindow;
    mainwin->show();

    return a.exec();
}

#include <iostream>

#include <QApplication>
#include <QSslSocket>
#include <QDebug>

#include "mainwindow.h"
MainWindow *mainwin = nullptr;

using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    std::cout << QSslSocket::sslLibraryBuildVersionString().toStdString() << std::endl;
    
    a.addLibraryPath(QString("/usr/local/ssl/lib"));
    
    mainwin = new MainWindow;
    mainwin->show();

    return a.exec();
}

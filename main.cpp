#include <iostream>

#include "mainwindow.h"
#include <QApplication>
#include <QSslSocket>
#include <QDebug>

using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    std::cout << QSslSocket::sslLibraryBuildVersionString().toStdString() << std::endl;
    
    a.addLibraryPath(QString("/usr/local/ssl/lib"));
    
    MainWindow w;
    w.show();

    return a.exec();
}

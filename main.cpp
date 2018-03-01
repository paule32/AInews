#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

#include <iostream>

#include <QObject>
#include <QApplication>
#include <QMainWindow>
#include <QSslSocket>
#include <QString>
#include <QDesktopWidget>

#include "mainwindow.h"
#include "spendendialog.h"

MainWindow *mainwin = nullptr;
using namespace std;

int main(int argc, char *argv[])
{
    // ---------------------------------
    // IMPORTANT: set plugin path's ...
    // ---------------------------------  
    setenv("QT_QPA_PLATFORM_PLUGIN_PATH",
    "./plugins", 1);
    Q_INIT_RESOURCE(resources);
    
    // ------------------------------------
    // after plugin's loaded, start app ...
    // ------------------------------------
    QApplication app(argc, argv);
    appDirPath = QApplication::applicationDirPath();
    std::cout << QSslSocket::sslLibraryBuildVersionString().toStdString() << std::endl;
    
    app.addLibraryPath(QString("/usr/local/ssl/lib"));
    
    // --------------------
    // donation ? - thanks!
    // --------------------
    SpendenDialog *spende = new SpendenDialog;
    spende->exec();
    if (spende->exitCode >= 1) {
        Q_CLEANUP_RESOURCE(resources);
        QCoreApplication::quit();
        return 0;
    }
    
    // and go on ...
    //
    mainwin = new MainWindow;
    mainwin->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            mainwin->size(),
            app.desktop()->availableGeometry()
        ));
    mainwin->show();

    int res = app.exec();
    Q_CLEANUP_RESOURCE(resources);
    return res;
}

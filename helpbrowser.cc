#ifndef HELPBROWSER_H
# include "headers.h"
#define HELPBROWSER_H
#endif

HelpBrowser::HelpBrowser(
    QHelpEngine * helpEngine,
    QWidget     * parent )
    : QTextBrowser(parent)
    , helpEngine(helpEngine)
{
    QString hdir = "/help";
    QString path = QApplication::applicationDirPath() + hdir;
    
    QDir arch(path);
    bool found = true;
    
    if (!arch.exists())       { found = false;
    if (!arch.mkdir ("help")) { found = false;
    if (!arch.mkpath("help")) { found = false;
    
    } else found = true;
    } else found = true;
    } else found = true;
    
    if (!found) {
        QMessageBox::warning(this,
        QString(tr("Warnung")),
        QString(
        tr("Hilfe konnte nicht abgeruffen werden.\n") +
        tr("Aktion abgebrochen.")));
        return;
    }
    
    QString hlpFile =
    QApplication::applicationDirPath() +
    QString("/help/help.qhc");   
    QFileInfo info(hlpFile);

    if (!info.exists(hlpFile)) {
        QMessageBox::warning(this,
        QString(tr("Application Warning")),
        QString(tr("Helpfile not found.")));
        
        return;
    }
}

QVariant HelpBrowser::loadResource(int type, const QUrl &name)
{
    if (name.scheme() == "qthelp")
    return QVariant(helpEngine->fileData(name)) ; else
    return QTextBrowser::loadResource(type,name);
}

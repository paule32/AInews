#include "headers.h"
#include "helpbrowser.h"

HelpBrowser::HelpBrowser(
    QWidget * parent )
    : QTextBrowser(parent)
{
    QString hdir = "/data/help";
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
    
    QString hfs = path + "/help.qch";
    QString cfs = path + "/help.qhc";
    
    qDebug() << "helpfile:" << hfs;
    qDebug() << "collectf:" << cfs;
    
    QFileInfo info(hfs);

    if (!info.exists(hfs)) {
        QMessageBox::warning(this,
        QString(tr("Application Warning")),
        QString(tr("Helpfile not found.")));
        
        return;
    }
    
    setAcceptRichText(false);
    setMinimumWidth(300);
    
    helpEngine = new QHelpEngine (hfs);
    helpEngine->setupData();
    helpEngine->setCollectionFile(cfs);
    
    setSource(QUrl("qthelp://documents/doc/index.html"));
    //setSource(helpEngine->findFile(QUrl("/doc/index.html").toString()));
}

QVariant HelpBrowser::loadResource(int type, const QUrl &name)
{
    if (name.scheme() == "qthelp")
    return QVariant(helpEngine->fileData(name)) ; else
    return QTextBrowser::loadResource(type,name);
}

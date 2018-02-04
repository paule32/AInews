#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QString>
#include <QLatin1String>
#include <QtCore/QLibraryInfo>
#include <QtWidgets/QApplication>
#include <QtHelp/QHelpEngineCore>

#include "../mainwindow.h"
#include "helpbrowser.h"

HelpBrowser::HelpBrowser(QWidget *parent)
    : QTextBrowser(parent)
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
    
    QString collectionFile = path + "/help.qhc";

    m_helpEngine = new QHelpEngineCore(collectionFile, this);
    if (!m_helpEngine->setupData()) {
        delete m_helpEngine;
        m_helpEngine = nullptr;
        return;
    }
}

void HelpBrowser::showHelpForKeyword(const QString &id)
{
    if (m_helpEngine) {
        QMap<QString, QUrl> links = m_helpEngine->linksForIdentifier(id);
        if (links.count())
            setSource(links.constBegin().value());
    }
}

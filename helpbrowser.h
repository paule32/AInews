#ifndef HELPBROWSER_H

#include <QObject>
#include <QString>
#include <QWidget>
#include <QUrl>
#include <QTextBrowser>
#include <QHelpEngine>

class HelpBrowser : public QTextBrowser
{
public:
    HelpBrowser(QHelpEngine *helpEngine, QWidget *parent = 0);
    QVariant loadResource(int type, const QUrl &name);

private:
    QHelpEngine * helpEngine;
};

#define HELPBROWSER_H
#endif // HELPBROWSER_H

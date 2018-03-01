#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <QObject>
#include <QString>
#include <QWidget>
#include <QUrl>
#include <QTextBrowser>
#include <QHelpEngine>

class HelpBrowser : public QTextBrowser
{
public:
    HelpBrowser(QWidget *parent = 0);
    QVariant loadResource(int type, const QUrl &name);

    QHelpEngine * helpEngine;
};

#endif // HELPBROWSER_H

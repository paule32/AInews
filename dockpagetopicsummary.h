#ifndef DOCKPAGETOPICSUMMARY_H
#define DOCKPAGETOPICSUMMARY_H

#include <QObject>
#include <QWidget>
#include <QDockWidget>
#include <QMouseEvent>
#include <QShowEvent>

class DockPageTopicSummary : public QDockWidget
{
public:
    explicit DockPageTopicSummary(QWidget *parent = 0);
    
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event);
};

#endif // DOCKPAGETOPICSUMMARY_H

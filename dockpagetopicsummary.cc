#include "dockpagetopicsummary.h"
#include "helppopup.h"

#include <QMouseEvent>
#include <QPalette>
#include <QColor>
#include <QDebug>

DockPageTopicSummary::DockPageTopicSummary(QWidget *parent)
    : QDockWidget(parent)
{
}

void DockPageTopicSummary::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    if (event->button() == Qt::RightButton) {     
        QPoint p = QCursor::pos();
        
        HelpPopUp *hlpPopUp = new
        HelpPopUp ;
        
        hlpPopUp->setWindowModality(Qt::ApplicationModal);
        hlpPopUp->move(p.x()+5,p.y()+5);
        hlpPopUp->show();
    }
}

void DockPageTopicSummary::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
}

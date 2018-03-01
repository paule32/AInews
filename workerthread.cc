#include <QCoreApplication>
#include <QTimer>
#include <QMessageBox>

#include "mainwindow.h"
#include "workerthread.h"


WorkerThread::WorkerThread(QObject *parent) : QObject(parent)
{
    idleTime = 0;
    
    closeIdleAppTimer = new QTimer; idleTime = 0;
    closeIdleAppTimer->setInterval(3000); connect(
    
    closeIdleAppTimer, SIGNAL(timeout()), this, SLOT(idleTimerUpdate()));
    closeIdleAppTimer->setSingleShot(true);
    
    closeIdleAppTimer->start();
    
    int result;
    if (idleTime < 10) {
        if (result == QMessageBox::Abort) {
            
        }   else
        if (result == QMessageBox::Close) {
            QCoreApplication::quit();
        }
        if (result == QMessageBox::Apply) {
        }
    }
}

WorkerThread::~WorkerThread()
{
    delete closeIdleAppTimer;
}

void WorkerThread::process()
{
    qDebug() << "Hello World " << idleTime;
    if (idleTime >= 20) {
        closeIdleAppTimer->stop();
        emit finished();
    }
}
    
void WorkerThread::idleTimerUpdate()
{
    idleTime++;
}

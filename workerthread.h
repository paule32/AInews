#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QObject>
#include <QMessageBox>

class WorkerThread : public QObject
{
    Q_OBJECT
public:
    explicit WorkerThread(QObject *parent = nullptr);
    ~WorkerThread();
signals:
    void finished();
    void error(QString err);
public slots:
    void process();
    void idleTimerUpdate();
private:
    QMessageBox * msgBox;
    QTimer * closeIdleAppTimer;
    int idleTime;
};

#endif // WORKERTHREAD_H

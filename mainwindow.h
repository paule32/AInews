#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShowEvent>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFileInfo>
#include <QtXml/QtXml>

namespace Ui {
class MainWindow;
}

enum topic_type {
    TOPIC_DEFAULT = 0,
    TOPIC_AI
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void retrieveElements(QDomElement,QString,QString);
    void loadTechCrunch(topic_type);

protected:
    virtual void showEvent(QShowEvent *event);

signals:
    void windowBecomesFocus();

private slots:
    void httpReadyRead();
    void httpDownloadFinished();
    void httpDownloadProgress(qint64, qint64);

    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_MainWindow_windowBecomesFocus();
    void on_pushButton_2_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
    QUrl url;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;
    QDomDocument * domDocument;
    QString fileName;
    QFile *file;
    bool httpRequestAborted;
    qint64 fileSize;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <myhtml/api.h>
#include <myhtml/tree.h>

#include <QObject>
#include <QString>
#include <QMainWindow>
#include <QStringList>
#include <QByteArray>
#include <QProcess>
#include <QUrl>
#include <QHelpEngineCore>
#include <QHelpEngine>
#include <QTreeWidgetItem>
#include <QListWidgetItem>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QDate>
#include <QFile>

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
    void loadTechCrunch(topic_type);
    
    void checkAndLoadData(int, QString str1="", QString str2="");
    void createHelpWindow();
    void setLinkItems(int row);
    
    void openIndexHelp();
    
    void loadLwItem();
    void saveLwItem();

    QString settingFileName;
    QString archiveDirName;
    QString helpDirName;
    
    QHelpEngine * helpEngine;
    
protected:
    virtual void showEvent(QShowEvent *event);

signals:
    void windowBecomesFocus();

private slots:
    void timer1_update();
    
    void httpReadyRead();
    void httpDownloadFinished();
    void httpDownloadProgress(qint64, qint64);

    void on_refreshData_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_MainWindow_windowBecomesFocus();
    void on_pushButton_2_clicked();
    void on_sitesBox_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_listTopicsBox_itemDoubleClicked(QListWidgetItem *item);
    void on_translateError(QProcess::ProcessError);
    void on_pushButton_6_clicked();
    void on_addNewLinkName_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_11_clicked();
    void on_actionAbout_Qt_triggered();
    
private:
    myhtml_t* myhtml;
    myhtml_tree_t* tree;

    QStringList topics;
    QByteArray html_data;
    QProcess * translate_process;

    QUrl url;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;
    
    QString fileName;
    QDate datum;
    QFile *file;
    bool httpRequestAborted;
    qint64 fileSize;

public:
    Ui::MainWindow *ui;
};

extern MainWindow *mainwin;
#define MAINWINDOW_H
#endif // MAINWINDOW_H

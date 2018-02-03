#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <myhtml/api.h>
#include <myhtml/tree.h>

#include <QMainWindow>
#include <QShowEvent>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFileInfo>

namespace Ui {
class MainWindow;
class Dialog;
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
    void setLinkItems(int row);
    
    void loadLwItem();
    void saveLwItem();

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
    void on_listTopics_itemDoubleClicked(QListWidgetItem *item);
    void on_translateError(QProcess::ProcessError);
    void on_pushButton_6_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_11_clicked();
    
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
    QString settingFileName;
    QString archiveDirName;
    QString fileName;
    QFile *file;
    bool httpRequestAborted;
    qint64 fileSize;

private:
    Ui::MainWindow *ui;
    Ui::Dialog     *uiA;
};

#endif // MAINWINDOW_H

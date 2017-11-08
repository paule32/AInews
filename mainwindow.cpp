#include <QMessageBox>
#include <QTextDocument>
#include <QNetworkReply>
#include "mainwindow.h"
#include "ui_mainwindow.h"

static const char techcrunch[] = "techcrunch.com";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    emit on_MainWindow_windowBecomesFocus();
}

// Daten auffrischen ...
void MainWindow::on_pushButton_4_clicked()
{

}

// Daten-Speicherort ändern
void MainWindow::on_pushButton_5_clicked()
{

}

// rückwärts blättern
void MainWindow::on_pushButton_clicked()
{

}

// vorwärts blättern
void MainWindow::on_pushButton_3_clicked()
{

}

void MainWindow::on_MainWindow_windowBecomesFocus()
{
    ui->plainTextEdit->document()->setHtml(QString(
    "<h1>AInews 1.0 - 2017 J. Kallup</h1>"
    "<h3>Stand: 11. November 2017</h3>"
    ));
}

void MainWindow::on_pushButton_2_clicked()
{
    loadTechCrunch(TOPIC_AI);
}

void MainWindow::retrieveElements(QDomElement root, QString tag, QString attr)
{
    QDomNodeList nodes = root.elementsByTagName(tag);

    for (int i=0;i<nodes.count();i++)
    {
        QDomNode elm = nodes.at(i);
        if (elm.isElement())
        {
            QDomElement e = elm.toElement();
            qDebug() << e.attribute(attr);
        }
    }
}

void MainWindow::httpDownloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesRead);
}

void MainWindow::httpReadyRead()
{

}

void MainWindow::httpDownloadFinished()
{
    QFileInfo fileInfo(url.path());
    fileName = fileInfo.fileName(); if (fileName.isEmpty())
    fileName = "index.html";

    if (QFile::exists(fileName)) {
        if (QMessageBox::question(this, tr("HTTP"),
                tr("Es existiert bereits eine Datei unter den Namen: %1 im "
                "aktuellen Verzeichnis. Überschreiben?").arg(fileName),
                QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
                == QMessageBox::No)
                return;
        QFile::remove(fileName);
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("HTTP"),
                      tr("Kann die Datei %1 nicht unter speichern: %2")
                      .arg(fileName).arg(file->errorString()));
        file = 0;
        return;
    }

    QByteArray bar;
    bar.clear();

    int rsize = reply->size();

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(rsize);

    for (int i=0;i<rsize;i++) {
        ui->progressBar->setValue(i*2);
        bar.append(reply->read(1));
    }

    file->write(bar);
    file->flush();
    file->close();

    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue  (100);


    QTextDocument * doc = new QTextDocument(bar.data());
    ui->plainTextEdit->document()->setHtml(doc->toPlainText());
}

void MainWindow::loadTechCrunch(topic_type topic)
{
    if (topic == TOPIC_AI)
    {
        manager = new QNetworkAccessManager(this);
        url = QUrl("https://techcrunch.com/artificial-intelligence-2/");

        request.setRawHeader("User-Agent", "Mozilla/5.0 (Mobile; rv:26.0)");
        request.setUrl(url);

        reply = manager->get(request);

        connect(reply,SIGNAL(downloadProgress(qint64,qint64)),
                this, SLOT(httpDownloadProgress(qint64,qint64)));

        connect(reply,SIGNAL(readyRead()),
                this,SLOT(httpReadyRead()));

        connect(reply,SIGNAL(finished()),
                this,SLOT(httpDownloadFinished()));
    }
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if (item->text(column).contains(techcrunch)) {
        loadTechCrunch(TOPIC_AI);
    }
    ui->pushButton->setEnabled(true);
}

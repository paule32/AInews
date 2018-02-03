#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <myhtml/api.h>
#include <myhtml/tree.h>

#include <QMessageBox>
#include <QStringList>
#include <QListWidgetItem>
#include <QTextDocument>
#include <QSettings>
#include <QNetworkReply>
#include <QFileDialog>
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>

#include "mainwindow.h"
#include "dialoga.h"

#include "ui_mainwindow.h"
#include "ui_dialoga.h"

static const char techcrunch[] = "techcrunch.com";

struct articleStruct {
    QString topic;
    int file;
};

QList<articleStruct> articles;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString setfile =
    QApplication::applicationDirPath() + "/settings.ini";
    settingFileName = setfile;
    loadLwItem();

    myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_PARSE_MODE_SINGLE, 1, 0);

    tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);
}

MainWindow::~MainWindow()
{
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);

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
//    articles .clear();
}

// Daten-Speicherort ändern
void MainWindow::on_pushButton_5_clicked()
{
    auto fd = new QFileDialog(this);
    QString fdn = fd->getExistingDirectory(this);
    ui->lineEdit->setText(fdn);
    delete fd;
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
    ui->artikelTextBox->document()->setHtml(QString(
    "<h1>AInews 1.0 - 2017 J. Kallup</h1>"
    "<h3>Stand: 11. November 2017</h3>"
    ));
}

void MainWindow::on_pushButton_2_clicked()
{
    loadTechCrunch(TOPIC_AI);
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

    int tsize = 0;
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

    html_data.clear();

    int rsize = reply->size();
    qDebug() << rsize;

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(rsize);

    for (int i=0;i<rsize;i++) {
        ui->progressBar->setValue(i*2);
        html_data.append(reply->read(1));
    }

    file->write(html_data);
    file->flush();
    file->close();

    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue  (100);

    ui->listTopics->clear();

    const char *data = html_data.data();

    // parse html
    myhtml_parse(tree, MyENCODING_UTF_8,data,strlen(data));

    mycore_string_raw_t str = {0};
    myhtml_collection_t* col = myhtml_get_nodes_by_attribute_value_whitespace_separated(
                tree, NULL, tree->document, true,
                "class", 5,
                "post-title", 10, NULL);

    for(uint i = 0; i < col->length; i++) {
        myhtml_tree_node_t *element = col->list[i];

        if(element->tag_id != MyHTML_TAG_H2
        || element->child  == NULL)
        continue;

        myhtml_serialization_tree_buffer(element->child, &str);

        if(str.data) {
            myhtml_parse(tree, MyENCODING_UTF_8,str.data,strlen(str.data));

            mycore_string_raw_t str2 = {0};
            myhtml_collection_t *col2 = myhtml_get_nodes_by_tag_id(tree, NULL, MyHTML_TAG_A, NULL);

            myhtml_tree_node_t *element = col2->list[i];

            if(element->tag_id != MyHTML_TAG_A
            || element->child  == NULL)
            continue;

            myhtml_serialization_tree_buffer(element->child, &str2);

            if (str2.data) {
                QListWidgetItem * item = new QListWidgetItem;
                item->setText(QString(str2.data));
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                item->setCheckState(Qt::Unchecked);
                ui->listTopics->addItem(item);
                
                {
                    struct articleStruct ar;
                    ar.topic = item->text();
                    ar.file  = ++tsize;
                    articles.append(ar);
                }
                
                str2.data = (char*)mycore_free(str2.data);
            }
        }
    }
  
    QString buffer;
    QSettings *settings = nullptr;
    settings = new QSettings(settingFileName,QSettings::IniFormat);
           
    for (int index = 0; index < tsize-1; index++) {
        buffer.clear();
        buffer.append(QString("%1").
        arg(articles.at(index).topic));
                
        settings->setValue(QString("ai/techcruch-%1").arg(index),buffer);
    }   settings->sync();
    
    delete settings;
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

void MainWindow::on_listTopics_itemDoubleClicked(QListWidgetItem *item)
{
    const char *data = html_data.data();

    if (ui->listTopics->currentItem() == item)
    qDebug() << "Ok: " << ui->listTopics->currentRow(); else
    qDebug() << "False!!!";

    int index = ui->listTopics->currentRow();
    if (index >  2) index = index - 1; else
    if (index == 2) index = index - 1;

    // the following code is ok, and independed of code above
    ui->titelLabel->setText(
    QString("<b>Titel:</b> %1")
    .arg(item->text()));

    // parse html
    myhtml_parse(tree, MyENCODING_UTF_8,data,strlen(data));
    myhtml_collection_t* col = myhtml_get_nodes_by_attribute_value_whitespace_separated(
                tree, NULL, tree->document, true,
                "class", 5,
                "excerpt", 7, NULL);

    if ((col != nullptr) && (index > -1)) //&& index < col->length)
    {
        mycore_string_raw_t str = {0};
        myhtml_tree_node_t *element = col->list[index];
        myhtml_serialization_tree_buffer(element,&str);

        if (str.data) {
            qDebug() << "-----\n" << str.data;
            QTextDocument * doc = new QTextDocument(str.data);
            
            /*
            if (ui->checkBox->isChecked()) {
                QByteArray byteArray = str.data;
                byteArray = qCompress(
                byteArray , 5);
            }*/
            
            ui->artikelTextBox->document()->setHtml(doc->toPlainText());
            str.data = (char*)mycore_free(str.data);
        }
    }
}

// translate
void MainWindow::on_pushButton_6_clicked()
{
    QStringList args;
    args << "-b"
         << QString("\"%1\"").arg(
            ui->artikelTextBox->document()->toPlainText());

    QString transfile = QApplication::applicationDirPath() + "/trans";
    
    translate_process = new QProcess(window());
    translate_process->setProgram(transfile);
    translate_process->setArguments(args);
    translate_process->start();

    connect(translate_process, SIGNAL(errorOccurred(QProcess::ProcessError)),
            this, SLOT(on_translateError(QProcess::ProcessError)));

    QString old_text = ui->artikelTextBox->document()->toPlainText();

    ui->artikelTextBox->document()->setHtml(QString(
    "<h1>Daten werden aufbearbeitet, Bitte etwas Geduld ...</h1>" ));

    if (!translate_process->waitForStarted()) {
        QMessageBox::warning(
        window(), "Warnung !!!",
        "TranslateProcess Error.");    return;
    }

    if (!translate_process->waitForFinished()) {
        QMessageBox::warning(
        window(), "Warnung !!!",
        "TranslateProcess End-Error.");  return;
    }

    QByteArray bar =
    translate_process->readAllStandardOutput();

    ui->artikelTextBox  ->document()->setPlainText(old_text);
    ui->translateTextBox->document()->setPlainText(bar);
}

void MainWindow::on_translateError(QProcess::ProcessError err)
{
    QMessageBox::warning(
        window(), "Warnung !!!",
        QString("Fehler: %1 aufgetretten.")
        .arg(err) );
    return;
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    Q_UNUSED(item);
}

void MainWindow::setLinkItems(int row)
{
    ui->listWidget->setCurrentRow(row);
    ui->listWidget2->clear();
    
    //QSettings *settings =
    //settings = new QSettings(settingFileName,QSettings::IniFormat);
    //for (int index = 0; index < ui->listWidget2->count(); index++) {
}

void MainWindow::loadLwItem()
{
    QSettings *settings = new
    QSettings(settingFileName,QSettings::IniFormat);

    articles.clear();    
    int size = settings->beginReadArray("ai/techcruch/AI");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        articleStruct tops;
        tops.topic = settings->value("topic").toString();
        tops.file  = settings->value("file" ).toInt();
        articles.append(tops);
    }
    settings->endArray();
    delete settings;
}

void MainWindow::saveLwItem()
{
    QSettings *settings = nullptr;
    settings = new QSettings(settingFileName,QSettings::IniFormat);
    
    int lcnt = ui->listWidget->count();
    int rcnt = 0;
    
    if (lcnt < 1) return;
    for (int i = 0; i < lcnt; ++i) {
        rcnt = ui->listWidget2->count();
        if (rcnt < 1) {
            if (i < lcnt)
            continue; else
            break;
        }
        for (int j = 0; i < rcnt; ++j) {
        
        }
    }
    
    settings->beginWriteArray("ai/techcruch/AI");
    for (int i = 0; i < articles.count(); i++) {
        settings->setArrayIndex(i);
        settings->setValue("topic",articles.at(i).topic);
        settings->setValue("file" ,articles.at(i).file );
    }   settings->sync();
        settings->endArray();
    
    delete settings;
}

// add new link-name ...
void MainWindow::on_pushButton_7_clicked()
{
    auto *da = new DialogA(this);
    da->exec();
    
    if (da->ui->lineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this,"Warnung","keine Eingabe.\nAktion abgewiesen!");
        return;
    }
    
    if (da->ui->lineEdit_2->text().trimmed().isEmpty()) {
        QMessageBox::warning(this,"Warnung","keine Gruppe angegeben.");
        return;
    }

    QString str1 = da->ui->lineEdit  ->text().trimmed();
    QString str2 = da->ui->lineEdit_2->text().trimmed();

    if (str1.contains("/")
    ||  str2.contains("/")) {
        QMessageBox::warning(this,
        QString(tr("Warnung")),
        QString(tr("ungültige Zeichen in Bezeichner:\n%1").arg(
        QString("/"))));
        delete da;
        return;
    }
    
    QString str3, str4;
    str3  = str1;
    str3  = str3.append("/" );
    str3  = str3.append(str2);

    bool found = false;
    for (int i = 0; i < ui->listWidget->count(); i++) {
        str4 = ui->listWidget->item(i)->text().trimmed();
        if (str4 == str3) {
            QMessageBox::warning(this,"Warnung","Eintrag bereits vorhanden.");
            found = true;
            break;
        }
    }

    if (found) return;
    
    QListWidgetItem *item = new
    QListWidgetItem(ui->listWidget);
    item->setText(str3);
    ui->listWidget->addItem(item);
    
    QFont font;
    font.setBold(true);
    
    QTreeWidget *tree = ui->treeWidget;
    QTreeWidgetItem *topw = new QTreeWidgetItem();
    topw->setText(0,str2);
    topw->setFont(0,font);
    tree->addTopLevelItem(topw);
    
    archiveDirName =
    QApplication::applicationDirPath() + "/arch";
    
    QDir arch("arch");
    found = true;
    
    if (!arch.exists())               { found = false;
    if (!arch.mkdir("arch"))          { found = false;
    if (!arch.mkpath(archiveDirName)) { found = false;
    
    } else found = true;
    } else found = true;
    } else found = true;
    
    qDebug() << archiveDirName;
    
    if (!found) {
        QMessageBox::critical(this,
        QString(tr("Warnung")),
        QString(tr("das Archiv konnte nicht erstellt werden")));
        return;
    }
    
    QDate datum(QDate::currentDate());
    bool val = datum.isValid();
    
    if (!val) {
        QMessageBox::warning(this,
        QString(tr("Warnung")),
        QString(tr("Falsches Datums-Format.")));
        return;
    }

    QString dstr = datum.toString("dd.MM.yyyy");
    QStringList fileList = arch.entryList(QDir::AllEntries);
    
    if (fileList.indexOf(dstr) < 0) {
        QMessageBox::StandardButtons buttons =
        QMessageBox::Apply |
        QMessageBox::Abort |
        QMessageBox::Close ;
        
        QMessageBox::information(this,
        QString(tr("Hinweis")), QString(
        tr("Es wurden keine gültigen Daten gefunden.\n") +
        tr("Vielleicht wurde das Verzeichnis gelöscht,\n") +
        tr("oder fehlende Benutzerrechte liegen vor.\n\n") +
        tr("Es kann auch sein, dass Sie das Programm erst\n") +
        tr("einrichten müssen.\n\n") +
        tr("Im letzteren Fall, klicken Sie bitte auf 'Anwenden'")),
        buttons);
    }
    
    //QFile nwFile(arch.filePath());
    //saveLwItem();
}

void MainWindow::on_pushButton_8_clicked()
{
    delete ui->listWidget->currentItem();
    saveLwItem();
}

// save button
void MainWindow::on_pushButton_11_clicked()
{
    saveLwItem();
}

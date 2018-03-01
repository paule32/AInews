#include "headers.h"

#include <QHelpContentItem>
#include <QHelpContentModel>
#include <QHelpContentWidget>
#include <QHelpIndexModel>
#include <QHelpIndexWidget>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialoga.h"
#include "ui_dialoga.h"

#include "helpbrowser.h"

QString appDirPath;
std::function<void(int,QString,QString)> check_setting_func;

class ArtikelDatum
{
public:
    ArtikelDatum() { }
    QString     topic;
    QDateTime   datTime;
};
class Artikel : public ArtikelDatum
{
public:
    Artikel() { }
    QStringList text;
};
QVector<Artikel *> ArtikelListe;

struct articleStruct {
    QString topic;
    int file;
};

QList<articleStruct> articles;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    settingFileName = QString("%1/%2")
    .arg(appDirPath)
    .arg(QString("settings.ini"));

    settings = nullptr;
    settings = new
        QSettings(settingFileName,
        QSettings::IniFormat);
    
    
    loadLwItem();

    ui->comboBoxRange->setEditable(true);
    ui->comboBoxRange->lineEdit()->setReadOnly(true);
    ui->comboBoxRange->lineEdit()->setAlignment(Qt::AlignCenter);
    
    for (int i = 0 ; i < ui->comboBoxRange->count() ; ++i)
    ui->comboBoxRange->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    ui->comboBoxRange->setEditable(false);

    myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_PARSE_MODE_SINGLE, 1, 0);

    tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);

    // help panel
    createHelpWindow();
    
    // set precollected data ...
    ui->sitesBox->clear();
    setSitesBoxData();

    check_setting_func = [this](int a, QString s1, QString s2) {
        return checkAndLoadData(a,s1,s2);
    };
    check_setting_func(1,
    QString(""),
    QString(""));
}

MainWindow::~MainWindow()
{
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);

    delete settings;
    delete ui;
}

void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    emit on_MainWindow_windowBecomesFocus();
}

// Daten auffrischen ...
void MainWindow::on_refreshData_clicked()
{
    QString filePathTmp = 
    appDirPath + "/data/arch/";
    
    QString fileName = filePathTmp + "index.gz";
    QString filePath = QDir(fileName).filePath(fileName);
    
    QDir testDir = QFileInfo(filePath).dir();  
    if (!testDir.exists()) {
        if (!testDir.mkpath(testDir.path())) {
            qWarning("could not make path: %s",
            testDir.path().toUtf8().constData());
            return;
        }
        if (fileName.endsWith('/')) {
            qWarning("could not make path: %s",
            fileName.toUtf8().constData());
            return;
        }
        qWarning("data storage error");
        return;
    }   else {
        QFile testFile(filePath);
        if (!testFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning("could not create: %s",
            fileName.toUtf8().constData());
            return;
        }
        
        QTextStream testStream(&testFile);
        testStream
            << "This is a test file named: "
            << fileName
            << endl;
    }

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
    loadTech();
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
    fileName = fileInfo.fileName();
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

    html_data.clear();

    qint64 rsize = reply->size();
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

    ui->listTopicsBox->clear();
    ArtikelListe.clear();

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
                ui->listTopicsBox->addItem(item);


                class   Artikel  * artikel = new Artikel;
                artikel->topic   = item->text();
                artikel->datTime = QDateTime::currentDateTime();
                artikel->text.append("Schnuller");
                ArtikelListe.append(artikel);

                str2.data = (char*)mycore_free(str2.data);
            }
        }
    }

    delete manager;
}

void MainWindow::loadTech()
{
    QString name;

    QList<QString> lister;
    QList<QUrl>    urlser;
    
    int size = settings->beginReadArray("AI");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        name = settings->value("name").toString();
        lister.append(name);
    }
    settings->endArray();
    
    for (int j = 0; j < lister.count()-1; ++j) {
        size = settings->beginReadArray(lister.at(j));
        for (int i = 0; i < size; ++i) {
            settings->setArrayIndex(i);
            name = settings->value("link").toString();
            qDebug() << ":: " << name;
            urlser.append(name);
        }
        settings->endArray();
    }
    
    manager = new QNetworkAccessManager(this);
    
    for (int i = 0; i < urlser.count(); ++i)
    {
        QString tmp = urlser.at(i).toString();
        if (!tmp.endsWith('/')) tmp = tmp + "/";
        
        url = QUrl(tmp);
       
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

// rules - rulesTextEdit
void MainWindow::on_pushButton_7_pressed()
{
    QMessageBox::StandardButtons buttons =
    QMessageBox::Yes |
    QMessageBox::No  | QMessageBox::Cancel;
    
    int result = QMessageBox::warning(0,
    QString(tr("Warnung")),
    QString(tr("Sollen alle Einträge gelöscht werden?.")),
    buttons);
    
    switch (result) {
    case QMessageBox::Cancel:
    case QMessageBox::No:
         return;
         break;
    case QMessageBox::Yes:
         ui->rulesTextEdit->clear();
         break;
    }
}
void MainWindow::on_listTopicsBox_itemDoubleClicked(QListWidgetItem *item)
{
    const char *data = html_data.data();
    int row = ui->listTopicsBox->currentRow();
    if (ui->listTopicsBox->currentItem() == item) {
        qDebug() << ArtikelListe.at(row)->topic;
        qDebug() << "Ok: " << row; 
    } else {
        qDebug() << "False!!!";
    }

    int index = ui->listTopicsBox->currentRow();
    if (index >= 2) index = index - 1;

    // the following code is ok, and independed of code above
    ui->topicTitle->setText(
    QString("<b>Titel:</b> %1")
    .arg(item->text()));
    
    ui->dockWidget_2->setFixedWidth(
    ui->dockWidget_4->width());

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
    // starting with inpatiant person's:)
    QString file = 
    appDirPath + "/data/pics/wait.gif";
    
    QUrl uri(QString("file://%1").arg(file));
    QImage image = QImageReader(file).read();
    
    QTextDocument *textdoc = ui->translateTextBox->document();
    textdoc->addResource(QTextDocument::ImageResource,
    uri, QVariant(image));
    
    QTextCursor cursor = ui->translateTextBox->textCursor();
    QTextImageFormat imageFormat;
    
    imageFormat.setWidth (image.width() );
    imageFormat.setHeight(image.height());
    imageFormat.setName( uri.toString() );
    
    cursor.insertImage(imageFormat);
    
    
    // translate ...
    QStringList args;
    args << "-b"
         << QString("\"%1\"").arg(
            ui->artikelTextBox->document()->toPlainText());

    QString transfile = appDirPath + "/trans";
    
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

void MainWindow::on_sitesBox_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    Q_UNUSED(item);
    
    if (item->text(0) == "AI")
    return;
    
    loadTech();
    ui->pushButton->setEnabled(true);
}

void MainWindow::setLinkItems(int row)
{
    ui->listWidget->setCurrentRow(row);
    ui->listWidget2->clear();
}

void MainWindow::loadLwItem()
{
    struct tmp {
        QString name;
        QString top;
    };
    tmp tmpstr;
    QList<tmp> strpop;
   
    ui->listWidget->clear();
    int size = settings->beginReadArray("sections");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        tmpstr.name = settings->value("name").toString();
        tmpstr.top  = settings->value("top" ).toString();
        
        QString top = QString("%1/%2")
        .arg(tmpstr.name)
        .arg(tmpstr.top );
        
        ui->listWidget->addItem(top);
        strpop.append(tmpstr);
    }
    
    settings->endArray();
}

void MainWindow::saveLwItem()
{
    QString cat;
    
    settings->beginWriteArray("sections");
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        settings->setArrayIndex(i);
        cat = ui->listWidget->item(i)->text();
        settings->setValue("name",cat);
    }
    
    settings->sync();
    settings->endArray();
}

// add new link-name ...
void MainWindow::on_addNewLinkName_clicked()
{
    ItemDialogA p;
    p.exec();
}

QString MainWindow::checkAndCreateDataFolder(QString dir)
{
    bool found = false;

    QString tmp =
    QString("%1/data/%2/")
    .arg(appDirPath)
    .arg(QString(dir));
    
    QDir arch(dir);
    found = true;
    
    if (!arch.exists())    { found = false;
    if (!arch.mkdir (tmp)) { found = false;
    if (!arch.mkpath(tmp)) { found = false;
    
    } else found = true;
    } else found = true;
    } else found = true;
    
    qDebug() << tmp;
    
    if (!found) {
        QMessageBox::critical(this,
        QString(tr("Warnung")),
        QString(tr("das Verzeichnis konnte nicht erstellt werden")));
        return QString("");
    }   return tmp;
}

void MainWindow::checkAndLoadData(int mode, QString str1, QString str2)
{
    bool found = false;
    
    if (mode == 0) {
        QString str3, str4;
        str3  = str1;
        str3  = str3.append("/" );
        str3  = str3.append(str2);

        for (int i = 0; i < ui->listWidget->count(); i++) {
            str4 = ui->listWidget->item(i)->text().trimmed();
            if (str4 == str3) {
                QMessageBox::warning(0,"Warnung","Eintrag bereits vorhanden.");
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
        
        QTreeWidget *tree = ui->sitesBox;
        QTreeWidgetItem *topw = new QTreeWidgetItem();
        QTreeWidgetItem *chlw = new QTreeWidgetItem();
        
        topw->setText(0,str2);
        topw->setFont(0,font);
        
        chlw->setText(0,str1);
        topw->addChild(chlw);
        
        tree->addTopLevelItem(topw);
        
        return;
    }
    
    QString tmp = checkAndCreateDataFolder("arch");
    QDir arch(tmp);
    
    datum = QDate::currentDate();
    bool val = datum.isValid();
    
    if (!val) {
        QMessageBox::warning(this,
        QString(tr("Warnung")),
        QString(tr("Falsches Datums-Format.")));
        return;
    }

    QString dstr = datum.toString("dd.MM.yyyy");
    QStringList fileList = arch.entryList(QDir::AllEntries);
    
    if (fileList.indexOf(dstr) < 0)
    {
        QMessageBox::StandardButtons buttons =
        QMessageBox::Apply |
        QMessageBox::Abort |
        QMessageBox::Close ;
        
        QMessageBox msgBox;
        msgBox.setText((tr("Hinweis")));
        msgBox.setInformativeText(
            tr("Es wurden keine gültigen Daten gefunden.\n") +
            tr("Vielleicht wurde das Verzeichnis gelöscht,\n") +
            tr("oder fehlende Benutzerrechte liegen vor.\n\n") +
            tr("Es kann auch sein, dass Sie das Programm erst\n") +
            tr("einrichten müssen.\n\n") +
            tr("Im letzteren Fall, klicken Sie bitte auf 'Anwenden'"));
        msgBox.setStandardButtons(buttons);
        msgBox.setDefaultButton(QMessageBox::Abort);
        
        int result = msgBox.exec();
        
        QTimer *timer1 = new QTimer;
        timer1->setInterval(1000);
        connect(timer1, SIGNAL(timeout()), this, SLOT(timer1_update()));
        timer1->start();
        
        #if 0
        // ------------------------
        // no user activity?
        // then close ...
        // ------------------------
        QThread *thread = new QThread;
        WorkerThread *worker = new WorkerThread();
        worker->moveToThread(thread);
        
        connect(worker, SIGNAL (error(QString)), this  , SLOT (errorString(QString)));
        connect(thread, SIGNAL (started())     , worker, SLOT (process()));
        connect(thread, SIGNAL (finished())    , worker, SLOT (deleteLater()));
        
        thread->start();
        #endif
        
        ui->topicPageWidget->setTabEnabled(0,true);
        ui->topicPageWidget->setTabText(0,dstr);
    }
    
    //QFile nwFile(arch.filePath());
    //saveLwItem();
}

void MainWindow::errorString(QString err)
{
    qDebug() << "Error:" << err;
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

void MainWindow::timer1_update()
{
    ui->vonDatum->setDate(datum);
    ui->vonTime ->setTime(QTime::currentTime());
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this,"Über Qt");
}

void MainWindow::createHelpWindow()
{
    QTabWidget* tWidget = new QTabWidget;
    tWidget->setMaximumWidth(320);
 
    HelpBrowser *tViewer = new HelpBrowser(this);
    QObject::connect(tViewer->helpEngine->contentWidget(),
            SIGNAL(linkActivated(QUrl)), tViewer,
            SLOT(setSource(QUrl)));
    QObject::connect(tViewer->helpEngine->indexWidget(),
            SIGNAL(linkActivated(QUrl, QString)), tViewer,
            SLOT(setSource(QUrl)));

    tWidget->addTab(tViewer->helpEngine->contentWidget(), "Contents");
    tWidget->addTab(tViewer->helpEngine->indexWidget(), "Index");

    QSplitter *m_panel = new
    QSplitter(Qt::Horizontal);

    m_panel->insertWidget(0, tViewer);
    m_panel->insertWidget(1, tWidget);

    ui->HelpBoxLayout->addWidget(m_panel);
}

void MainWindow::openIndexHelp()
{
}

void MainWindow::setSettingFile()
{
/*
    settingFileName = QString("%1/%2/").
    arg(appDirPath).
    arg(QString("settings.ini"));
    
    settings= new QSettings(settingFileName,QSettings::IniFormat);
*/
}

// rechte box
void MainWindow::on_listWidget2_itemClicked(QListWidgetItem *item)
{
    QString str = item->text();
    
    /*
    QFont font;
    font.setBold(true);
    
    QTreeWidget *tree = ui->sitesBox;
    QTreeWidgetItem *topw = new QTreeWidgetItem();
    topw->setText(0,str2);
    topw->setFont(0,font);
    tree->addTopLevelItem(topw);*/
}

void MainWindow::setSitesBoxData()
{
    QString s1,s2;
    setSettingFile();
    
    QFont font;
    font.setBold(true);

    QTreeWidget     *tree = ui->sitesBox;
    QTreeWidgetItem *artw;
    QTreeWidgetItem *topw;

    QList<QString> cont;  // to check for duplicates
    tree->clear();
    
    int size = settings->beginReadArray("sections");
    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);
        s1 = settings->value("name").toString();
        s2 = settings->value("top" ).toString();
        
        qDebug() << "lal: " << s1 << s2;
        if (cont.count() < 1) {
            cont.append(s2);
            artw = new QTreeWidgetItem();
            artw->setText(0,s2);
            artw->setFont(0,font);
            tree->addTopLevelItem(artw);
        }
        
        if (!cont.contains(s2)) {
            cont.append(s2);
            artw = new QTreeWidgetItem();
            artw->setText(0,s2);
            artw->setFont(0,font);
            tree->addTopLevelItem(artw);
        }
        else {
            topw = new QTreeWidgetItem();
            topw->setText(0,s1);
            artw->addChild(topw);
        }
    }
    
    settings->endArray();
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString str = item->text();
    qDebug() << str;
    
    if (!str.contains(QString("/"))) {
        QString s;
        int  pos ;
        if ((pos = str.indexOf("/")) < 0) {
            QMessageBox::warning(this,"Warnung",
            QString("keine Untergruppe angegeben."));
            return;
        }
    }
    
    setSettingFile();

// TODO
    QString s1, s2;
    qDebug() << str;
    
    if (str.trimmed().length() < 1) {
        QMessageBox::warning(this,
        QString("Warnung"),
        QString("ungültiger Eintrag."));
        return;
    }
    
    if (str.contains("/")) {
        QList<QString> list = str.split("/");
        s1 = list.at(0);
        s2 = list.at(1);
        
        QTreeWidget *tree = ui->sitesBox;
        qDebug() << "AT: " << s1 << s2;
        
        QList<QTreeWidgetItem*> clist =
        tree->findItems(s2,
        Qt::MatchContains |
        Qt::MatchRecursive, 0);
        
        if (clist.length() > 0) {
            QMessageBox::warning(this,
            QString(tr("Warnung")),
            QString("Eintrag bereits vorhanden."));
            
            return;
        }

        int sz = settings->beginReadArray("sections");
        settings->endArray();
        settings->beginWriteArray("sections");
        for (int i = 0; i < sz; ++i) {
            settings->setArrayIndex(i);
            settings->setValue("name", s1);
            settings->setValue("top" , s2);
        }
        settings->endArray();


        
        QFont font;
        font.setBold(true);
        font.setPointSize(10);
        
        QTreeWidgetItem *topw = new QTreeWidgetItem();
        QTreeWidgetItem *artw = new QTreeWidgetItem();
        
        artw->setText(0,s1);
        topw->setText(0,s2);
        topw->setFont(0,font);
        
        topw->addChild(artw);
        tree->addTopLevelItem(topw);
    }
    
    QString top;
    QList<QString> tops; 

/*
QStringList   list2;
list2.append("abc");
list2.append("125");

QList<QString> list;
list.append(QString("https://techcrunch.com/artificial-intelligence-2"));
list.append(QString("welt" ));
settings->beginWriteArray(str);
for (int i = 0; i < list.size(); ++i) {
    settings->setArrayIndex(i);
    settings->setValue("link", list.at(i));
    settings->setValue("rule", list2);
}
settings->endArray();
*/

    int size = settings->beginReadArray(str);
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        top = settings->value("link").toString();
        tops.append(top);
    }
    
    ui->listWidget2->clear();
    ui->listWidget2->addItems(tops);
    
    tops.clear();
    settings->endArray();
}

void MainWindow::on_comboBoxRange_currentIndexChanged(int index)
{
    QDate datum = QDate::currentDate();
    
    ui->topicPageWidget->clear();
    switch (index) {
    // 1 day
    case 0: {
        QWidget *page = new QWidget;
        ui->topicPageWidget->addTab(page,datum.toString("dd.MM.yyyy"));
    }
    break;
    
    // 1 week
    case 1: {
        std::array<QWidget*, 7> page;
        for (int i=0;i<7;i++) {
            page[i] = new QWidget;
            QString str = datum.toString("dd.MM.yyyy");
            ui->topicPageWidget->addTab(page[i],str);
            datum = datum.addDays(1);
        }
    }
    break;
    }
}

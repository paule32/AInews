#include "headers.h"

#include "spendendialog.h"
#include "ui_spendendialog.h"

#include <quazip/quazipfile.h>

#include <QDesktopServices>
#include <QDebug>

extern QString appDirPath;

SpendenDialog::SpendenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpendenDialog)
{   ui->setupUi(this);
    ui->textBrowser->setHtml(getZipContent("info.html"));
    
    exitCode = 0;
}

SpendenDialog::~SpendenDialog() { delete ui; }

void SpendenDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    qDebug() << "button: " << button->text();
    if (button->text() == QString("&Cancel")) {
        exitCode = 1;
        close();
    }
}

void SpendenDialog::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl(
    "https://www.paypal.com/cgi-bin/webscr/?"
    "hosted_button_id=FBESEMEXP8HSL&"
    "cmd=_s-xclick"
    ));
}

QString SpendenDialog::getZipContent(QString hfile)
{
    QByteArray inhalt;
    
    QuaZip zip(":/info.zip");
    zip.open(QuaZip::mdUnzip);
    zip.setCurrentFile(hfile);
        
    QuaZipFile file(&zip);
    file.open(QIODevice::ReadOnly); inhalt =
    file.readAll();
    file.close();
    
    return QString(inhalt.data());
}

void SpendenDialog::on_pushButton_2_clicked()
{
    static int flag = 0;
    
    if (!flag) {
        ui->pushButton_2->setText("GNU - Manifest");
        ui->textBrowser_2->setHtml(getZipContent("gpl.html"));
        flag = 1;
    }   else
    if (flag == 1) {
        ui->pushButton_2 ->setText("Manifest - Ausblenden");
        ui->textBrowser_2->setHtml(getZipContent("manifest.html"));
        flag = 2;
    }   else
    if (flag == 2) {
        ui->pushButton_2 ->setText("Free Software");
        ui->textBrowser_2->setHtml(getZipContent("freesoftware.html"));
        flag = 3;
    }   else {
        ui->pushButton_2 ->setText("GPL - Bitte lesen !");
        ui->textBrowser_2->setHtml("<b>Hello</b>");
        ui->textBrowser_2->update();
        flag = 0;
    }
}

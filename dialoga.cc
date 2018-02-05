#include "./headers.h"

extern std::function<void(int,QString,QString)> check_setting_func;

ItemDialogA::ItemDialogA(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemDialogA) {
    ui->setupUi(this);

}

ItemDialogA::~ItemDialogA()
{
    delete ui;
}

void ItemDialogA::on_buttonBox_clicked(QAbstractButton *button)
{
    Q_UNUSED(button);
    if (ui->lineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(0,
        QString(tr("Warnung")),
        QString(tr("keine Eingabe.\nAktion abgewiesen!")));
        return;
    }
    
    if (ui->lineEdit_2->text().trimmed().isEmpty()) {
        QMessageBox::warning(0,
        QString(tr("Warnung")),
        QString(tr("keine Gruppe angegeben.")));
        return;
    }

    QString str1 = ui->lineEdit  ->text().trimmed();
    QString str2 = ui->lineEdit_2->text().trimmed();

    if (str1.contains("/")
    ||  str2.contains("/")) {
        QMessageBox::warning(0,
        QString(tr("Warnung")),
        QString(tr("ungültige Zeichen in Bezeichner:\n%1").arg(
        QString("/"))));
        return;
    }
    
    check_setting_func(0,str1,str2);
}

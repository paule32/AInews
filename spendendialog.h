#ifndef SPENDENDIALOG_H
#define SPENDENDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QAbstractButton>

namespace Ui {
class SpendenDialog;
}

class SpendenDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SpendenDialog(QWidget *parent = 0);
    ~SpendenDialog();
    
    QString getZipContent(QString hfile);
    int exitCode;
        
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    
private:
    Ui::SpendenDialog *ui;
};

#endif // SPENDENDIALOG_H

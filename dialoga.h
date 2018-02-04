#ifndef DIALOGA_H
#define DIALOGA_H

#include <QDialog>
#include <QAbstractButton>
#include <QWidget>

#include "./ui_dialoga.h"

class ItemDialogA : public QDialog
{
    Q_OBJECT
    
public:
    explicit ItemDialogA(QWidget *parent = 0);
    ~ItemDialogA();

    Ui::ItemDialogA *ui;
    
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
};

#endif // DIALOGA_H

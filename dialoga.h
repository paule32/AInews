#ifndef ITEMDIALOGA_H
#define ITEMDIALOGA_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QAbstractButton>

#include "ui_dialoga.h"

namespace Ui {
class ItemDialogA ;
}

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

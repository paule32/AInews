#ifndef ITEMDIALOGA_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QAbstractButton>

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

#define ITEMDIALOGA_H
#endif // DIALOGA_H

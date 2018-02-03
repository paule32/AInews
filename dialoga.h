#ifndef DIALOGA_H
#define DIALOGA_H

#include <QDialog>

namespace Ui {
class DialogA;
}

class DialogA : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogA(QWidget *parent = 0);
    ~DialogA();

    Ui::DialogA *ui;
};

#endif // DIALOGA_H

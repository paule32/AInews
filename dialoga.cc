#include "dialoga.h"
#include "ui_dialoga.h"

DialogA::DialogA(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogA)
{
    ui->setupUi(this);
}

DialogA::~DialogA()
{
    delete ui;
}

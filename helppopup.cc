#include "helppopup.h"
#include "ui_helppopup.h"

HelpPopUp::HelpPopUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpPopUp)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

HelpPopUp::~HelpPopUp()
{
    delete ui;
}

void HelpPopUp::keyPressEvent(QKeyEvent *event)
{
    event->accept();
    if (event->key() == Qt::Key_Escape) {
        close();
    }
}

// close popup
void HelpPopUp::on_pushButton_3_pressed()
{
    close();
}

// help
void HelpPopUp::on_pushButton_pressed()
{
    
}

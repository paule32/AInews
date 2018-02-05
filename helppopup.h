#ifndef HELPPOPUP_H
#define HELPPOPUP_H

#include <QObject>
#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class HelpPopUp;
}

class HelpPopUp : public QWidget
{
    Q_OBJECT
    
public:
    explicit HelpPopUp(QWidget *parent = 0);
    ~HelpPopUp();

protected:
    void keyPressEvent(QKeyEvent *event);
    
private slots:
    void on_pushButton_3_pressed();
    
    void on_pushButton_pressed();
    
private:
    Ui::HelpPopUp *ui;
};

#endif // HELPPOPUP_H

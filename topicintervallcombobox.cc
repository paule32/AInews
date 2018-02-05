#include "topicintervallcombobox.h"
#include <QDebug>

TopicIntervallComboBox::TopicIntervallComboBox(QWidget *parent)
    : QComboBox(parent)
{
    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged),
    [=](int index){
        qDebug() << itemText(index);
    });
}

#ifndef MYQLISTWIDGETITEMMOVIMENTO_H
#define MYQLISTWIDGETITEMMOVIMENTO_H

#include <QObject>
#include <QWidget>
#include<QListWidgetItem>
#include "movimento.h"

class myQlistWidgetItemMovimento : public QListWidgetItem
{
private:
    QWidget* parent;
    movimento* oggetto;//puntatore polimorfo al mio oggetto
public:
    myQlistWidgetItemMovimento();
    myQlistWidgetItemMovimento(QWidget*, movimento* = nullptr);
    void update();
    movimento *getOggetto() const;
    void setOggetto(movimento *value);
};

#endif // MYQLISTWIDGETITEMMOVIMENTO_H

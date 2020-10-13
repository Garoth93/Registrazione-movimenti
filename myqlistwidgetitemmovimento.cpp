#include "myqlistwidgetitemmovimento.h"

movimento *myQlistWidgetItemMovimento::getOggetto() const
{
    return oggetto;
}

void myQlistWidgetItemMovimento::setOggetto(movimento *value)
{
    oggetto = value;
}

myQlistWidgetItemMovimento::myQlistWidgetItemMovimento()
{

}

myQlistWidgetItemMovimento::myQlistWidgetItemMovimento(QWidget * p, movimento * mo):
    parent(p),
    oggetto(mo)
{
    update();
}

void myQlistWidgetItemMovimento::update()
{
    setText(oggetto->getInfo());
    if(oggetto->tipoMovimento()=="Entrata")
        setIcon(QIcon(":/entrata.png"));
    else
        setIcon(QIcon(":/uscita.png"));
}

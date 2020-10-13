#include "modifica.h"
#include "ui_modifica.h"

modifica::modifica(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifica)
{
    ui->setupUi(this);

    impostaValidator();

    connect(ui->b_buttonIns, SIGNAL(clicked()), this, SLOT(pressioneModifica()));
    connect(ui->b_buttonElimina, SIGNAL(clicked()), this, SLOT(pressioneElimina()));
    connect(ui->calendarWidget,SIGNAL(clicked(const QDate &)),this,SLOT(pressioneCalendario(const QDate &)));
}

modifica::~modifica()
{
    delete ui;
}

void modifica::popolaModifica()
{
    ui->calendarWidget->setSelectedDate(obj->data());
    ui->l_lineDesc->setText(obj->descrizione());
    ui->l_lineValore->setText(QString::number(obj->valore()));
    ui->l_labelData->setText(obj->data().toString("dd/MM/yyyy"));
    if(obj->tipoMovimento()=="Uscita")
        ui->radioButton->setChecked(true);
    else
        ui->radioButton_2->setChecked(true);
    QString toSe = obj->categoria();
    int x = ui->c_comboCategoria->findText(toSe);
    ui->c_comboCategoria->setCurrentIndex(x);
}

movimento *modifica::getObj() const
{
    return obj;
}

void modifica::setObj(movimento *value)
{
    obj = value;
}

void modifica::setListaPrincipale(QList<movimento *> *listaPrincipale)
{
    m_listaPrincipale = listaPrincipale;
}

void modifica::impostaValidator()
{
    ui->l_lineValore->setValidator( new QIntValidator(0,9999999,this));
}

void modifica::pressioneModifica()
{
    obj->setData(ui->calendarWidget->selectedDate());
    QString movimentoToIns;
    if(ui->radioButton->isChecked()) movimentoToIns="Uscita";
    else movimentoToIns="Entrata";
    obj->setTipoMovimento(movimentoToIns);
    obj->setDescrizione(ui->l_lineDesc->text());
    obj->setCategoria(ui->c_comboCategoria->currentText());
    obj->setValore(ui->l_lineValore->text().toDouble());
    this->close();
}

void modifica::pressioneCalendario(const QDate &)
{
    ui->l_labelData->setText(ui->calendarWidget->selectedDate().toString("dd/MM/yyyy"));//inserisco la stringa ma in formato dd/MM//yyyy
}

void modifica::pressioneElimina()
{
    int toDel;
    QList<movimento*>::iterator it = m_listaPrincipale->begin();
    for(int i=0;it!=m_listaPrincipale->end();++it , i++)
    {
        if(obj==(*it))
        {
            toDel=i;
        }
    }
    m_listaPrincipale->removeAt(toDel);
    delete obj;
    this->close();
}

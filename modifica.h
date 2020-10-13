#ifndef MODIFICA_H
#define MODIFICA_H

#include <QDialog>

#include "jsonoperation.h"
#include "movimento.h"

namespace Ui {
class modifica;
}

class modifica : public QDialog
{
    Q_OBJECT

public:
    explicit modifica(QWidget *parent = nullptr);
    ~modifica();
    void popolaModifica();

    movimento *getObj() const;
    void setObj(movimento *value);
    void setListaPrincipale(QList<movimento *> *listaPrincipale);
    void impostaValidator();

private:
    Ui::modifica *ui;
    movimento * obj;
    QList<movimento*>* m_listaPrincipale;
public slots:
    void pressioneModifica();
    void pressioneCalendario(const QDate &);
    void pressioneElimina();
};

#endif // MODIFICA_H

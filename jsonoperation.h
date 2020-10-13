#ifndef JSONOPERATION_H
#define JSONOPERATION_H

#include "movimento.h"

#include <QList>
#include <QString>
#include <QDate>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValueRef>

class jsonOperation
{
private:
    QList<movimento*>* l_listaMovimenti;
    QFile * fileJson;
    QString pathFile;
public:
    jsonOperation(QList<movimento*>* listaPrincipale,QString pathFile);
    void init();
    void loadDataFromJson();
    bool InsertDataInJson(QString categoria, QDate data, QString descrizione, QString tipoMovimento, double valore);
    bool insertAllDataInJson();
    QList<movimento*>* getListaSelect(QDate dataCerca, QString dym);
    int getNextId();

    double getSaldoTot();
    double getSaldoMensile(QDate mese);
    double getSaldoGiorno(QDate giorno);
    double getSaldoSettimana(QDate dataTo);

    double getUscitaTot();
    double getUscitaMensile(QDate mese);
    double getUscitaGiorno(QDate giorno);
    double getUscitaSettimana(QDate dataTo);

    double getEntrataTot();
    double getEntrataMensiole(QDate mese);
    double getEntrataGiorno(QDate giorno);
    double getEntrataSettimana(QDate dataTo);

    double getEntrataTotCategoria(QString cate);
    double getEntrataMeseCategoria(QString cate, QDate mese);
    double getEntrataSettimanaCategoria(QString cate, QDate settimana);
    double getEntrataGiornoCategoria(QString cate, QDate giorno);

    double getUscitaTotCategoria(QString cate);
    double getUscitaMeseCategoria(QString cate, QDate mese);
    double getUscitaSettimanaCategoria(QString cate, QDate settimana);
    double getUscitaGiornoCategoria(QString cate, QDate giorno);

    void setFileJson(QString toSet);
};
#endif // JSONOPERATION_H

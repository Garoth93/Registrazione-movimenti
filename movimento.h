#ifndef MOVIMENTO_H
#define MOVIMENTO_H

#include <QList>
#include <QString>
#include <QDate>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValueRef>

class movimento
{
private:
    int m_id;
    QString m_categoria;
    QDate m_data;
    QString m_descrizione;
    QString m_tipoMovimento;
    double m_valore;

public:
    movimento(int id, QString categoria, QDate data, QString descrizione, QString tipoMovimento, double valore);
    QString categoria() const;
    void setCategoria(const QString &categoria);
    QDate data() const;
    void setData(const QDate &data);
    QString descrizione() const;
    void setDescrizione(const QString &descrizione);
    QString tipoMovimento() const;
    void setTipoMovimento(const QString &tipoMovimento);
    double valore() const;
    void setValore(double valore);
    QString getInfo();
    int getId() const;
    void setId(int id);
};

#endif // MOVIMENTO_H

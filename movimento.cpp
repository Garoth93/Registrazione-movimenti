#include "movimento.h"


QString movimento::categoria() const
{
    return m_categoria;
}

void movimento::setCategoria(const QString &categoria)
{
    m_categoria = categoria;
}

QDate movimento::data() const
{
    return m_data;
}

void movimento::setData(const QDate &data)
{
    m_data = data;
}

QString movimento::descrizione() const
{
    return m_descrizione;
}

void movimento::setDescrizione(const QString &descrizione)
{
    m_descrizione = descrizione;
}

QString movimento::tipoMovimento() const
{
    return m_tipoMovimento;
}

void movimento::setTipoMovimento(const QString &tipoMovimento)
{
    m_tipoMovimento = tipoMovimento;
}

double movimento::valore() const
{
    return m_valore;
}

void movimento::setValore(double valore)
{
    m_valore = valore;
}

QString movimento::getInfo()
{
    QString toRet;
    toRet=QString("Data: %1%2%3%4Euro: %5%6Categoria: %7%8Descrizione: %9")
            .arg(m_data.toString())
            .arg("\n")
            .arg(m_tipoMovimento)
            .arg("\n")
            .arg(m_valore)
            .arg("\n")
            .arg(m_categoria)
            .arg("\n")
            .arg(m_descrizione);
    return toRet;
}

int movimento::getId() const
{
    return m_id;
}

void movimento::setId(int id)
{
    m_id = id;
}

movimento::movimento(int id, QString categoria, QDate data, QString descrizione, QString tipoMovimento, double valore):
    m_id(id),
    m_categoria(categoria),
    m_data(data),
    m_descrizione(descrizione),
    m_tipoMovimento(tipoMovimento),
    m_valore(valore)
{
}

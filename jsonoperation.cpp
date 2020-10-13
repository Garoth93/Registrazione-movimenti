#include "jsonoperation.h"

void jsonOperation::setFileJson(QString toSet)
{
    //QString toSet = value.fileName();
    delete fileJson;
    fileJson=new QFile();
    fileJson->setFileName(toSet);
    //fileJson.setFileName(toSet);
}

jsonOperation::jsonOperation(QList<movimento*>* listaPrincipale,QString pathFileTo):
    l_listaMovimenti(listaPrincipale),
    pathFile(pathFileTo)
{

}

void jsonOperation::init()
{
    fileJson = new QFile();
    fileJson->setFileName(pathFile);
}

void jsonOperation::loadDataFromJson()
{
    if( !fileJson->open( QIODevice::ReadWrite ) ) //read json content
    {
        //open file error ...
    }
    QJsonDocument jsonOrg = QJsonDocument::fromJson( fileJson->readAll() );
    fileJson->close();
    QJsonArray jsonArray = jsonOrg.array();
    for (int i=0; i < jsonArray.size(); i++)
    {
        QJsonValue temp = jsonArray.at(i).toObject().value("movimento");
        QString catToIns  = temp.toObject().value("Categoria").toString();
        QString dataTo    = temp.toObject().value("Data").toString();
        QDate datatoIns   = QDate::fromString(dataTo,"dd/MM/yyyy");
        QString descToIns = temp.toObject().value("Descrizione").toString();
        QString moviToIns = temp.toObject().value("Tipo movimento").toString();
        double valToIns   = temp.toObject().value("Valore").toDouble();
        l_listaMovimenti->push_back(new movimento(getNextId(),catToIns,datatoIns,descToIns,moviToIns,valToIns));
    }
}

bool jsonOperation::InsertDataInJson(QString categoria, QDate data, QString descrizione, QString tipoMovimento, double valore)
{
    if( !fileJson->open( QIODevice::ReadOnly ) ) //read json content
    {
        //open file error ...
        return false;
    }
    QJsonDocument jsonOrg = QJsonDocument::fromJson( fileJson->readAll() );
    QByteArray jsonFileData = fileJson->readAll();
    fileJson->close();
    //local variable, do not use m_ prefix.
    QJsonObject projectDetails = { {"Data", data.toString( "dd/MM/yyyy" )},
                                   {"Tipo movimento", tipoMovimento},
                                   {"Valore", valore},
                                   {"Categoria", categoria},
                                   {"Descrizione", descrizione}
                                 };
    QJsonObject notificationObj =  {{ "movimento", projectDetails }};
    QJsonArray arrLog = jsonOrg.array();
    arrLog.push_back( notificationObj );

    QJsonDocument doc( arrLog );

    if( !fileJson->open( QIODevice::WriteOnly ) ) //write json content to file.
    {
        //cannot open for write ...
        return false;
    }

    fileJson->write(doc.toJson());
    fileJson->close();
    return true;
}

bool jsonOperation::insertAllDataInJson()
{
    if( !fileJson->open( QIODevice::WriteOnly ) ) //read json content
    {
        //open file error ...
        return false;
    }
    fileJson->seek(0);
    fileJson->close();
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        InsertDataInJson((*it)->categoria(),(*it)->data(),(*it)->descrizione(),(*it)->tipoMovimento(),(*it)->valore());
    }
    return true;
}

QList<movimento *> *jsonOperation::getListaSelect(QDate dataCerca, QString dym)
{
    QList<movimento*>* toRet=new QList<movimento*>;

    if(dym=="mese")
    {
        QList<movimento*>::iterator it = l_listaMovimenti->begin();
        for(;it!=l_listaMovimenti->end();++it)
        {
            if((*it)->data().toString("MM/yyyy")==dataCerca.toString("MM/yyyy"))
            {
                toRet->push_back(*it);
            }
        }
    }
    if(dym=="settimana")
    {
        QList<movimento*>::iterator it = l_listaMovimenti->begin();
        for(;it!=l_listaMovimenti->end();++it)
        {
            if((*it)->data().weekNumber()==dataCerca.weekNumber())
            {
                toRet->push_back(*it);
            }
        }
        return toRet;
    }
    if(dym=="giorno")
    {
        QList<movimento*>::iterator it = l_listaMovimenti->begin();
        for(;it!=l_listaMovimenti->end();++it)
        {
            if((*it)->data()==dataCerca)
            {
                toRet->push_back(*it);
            }
        }
    }
    return toRet;
}

int jsonOperation::getNextId()
{
    int max=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->getId() >= max) max=(*it)->getId();
    }
    return ++max;
}

double jsonOperation::getSaldoTot()
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->tipoMovimento()=="Uscita")
            toRet=toRet-(*it)->valore();
        else
            toRet=toRet+(*it)->valore();
    }
    return toRet;
}

double jsonOperation::getSaldoMensile(QDate mese)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data().toString("MM/yyyy")==mese.toString("MM/yyyy"))
        {
            if((*it)->tipoMovimento()=="Uscita")
                toRet=toRet-(*it)->valore();
            else
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

double jsonOperation::getSaldoGiorno(QDate giorno)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data()==giorno)
        {
            if((*it)->tipoMovimento()=="Uscita")
                toRet=toRet-(*it)->valore();
            else
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

double jsonOperation::getSaldoSettimana(QDate dataTo)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data().weekNumber()==dataTo.weekNumber())
        {
            if((*it)->tipoMovimento()=="Uscita")
                toRet=toRet-(*it)->valore();
            else
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

double jsonOperation::getUscitaTot()
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->tipoMovimento()=="Uscita")
            toRet=toRet+(*it)->valore();
    }
    return toRet;
}

double jsonOperation::getUscitaMensile(QDate mese)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data().toString("MM/yyyy")==mese.toString("MM/yyyy"))
        {
            if((*it)->tipoMovimento()=="Uscita")
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

double jsonOperation::getUscitaGiorno(QDate giorno)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data()==giorno)
        {
            if((*it)->tipoMovimento()=="Uscita")
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

double jsonOperation::getUscitaSettimana(QDate dataTo)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data().weekNumber()==dataTo.weekNumber())
        {
            if((*it)->tipoMovimento()=="Uscita")
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

double jsonOperation::getEntrataTot()
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->tipoMovimento()=="Entrata")
            toRet=toRet+(*it)->valore();
    }
    return toRet;
}

double jsonOperation::getEntrataMensiole(QDate mese)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data().toString("MM/yyyy")==mese.toString("MM/yyyy"))
        {
            if((*it)->tipoMovimento()=="Entrata")
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

double jsonOperation::getEntrataGiorno(QDate giorno)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data()==giorno)
        {
            if((*it)->tipoMovimento()=="Entrata")
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

double jsonOperation::getEntrataSettimana(QDate dataTo)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data().weekNumber()==dataTo.weekNumber())
        {
            if((*it)->tipoMovimento()=="Entrata")
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

double jsonOperation::getEntrataTotCategoria(QString cate)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->tipoMovimento()=="Entrata" && (*it)->categoria()==cate)
            toRet=toRet+(*it)->valore();
    }
    return toRet;
}

double jsonOperation::getEntrataMeseCategoria(QString cate, QDate mese)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data().toString("MM/yyyy")==mese.toString("MM/yyyy") && (*it)->categoria()==cate)
        {
            if((*it)->tipoMovimento()=="Entrata")
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

double jsonOperation::getEntrataSettimanaCategoria(QString cate, QDate settimana)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data().weekNumber()==settimana.weekNumber())
        {
            if((*it)->tipoMovimento()=="Entrata" && (*it)->categoria()==cate)
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

double jsonOperation::getEntrataGiornoCategoria(QString cate, QDate giorno)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data()==giorno)
        {
            if((*it)->tipoMovimento()=="Entrata" && (*it)->categoria()==cate)
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

double jsonOperation::getUscitaTotCategoria(QString cate)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->tipoMovimento()=="Uscita" && (*it)->categoria()==cate)
            toRet=toRet+(*it)->valore();
    }
    return toRet;
}

double jsonOperation::getUscitaMeseCategoria(QString cate, QDate mese)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data().toString("MM/yyyy")==mese.toString("MM/yyyy") && (*it)->categoria()==cate)
        {
            if((*it)->tipoMovimento()=="Uscita")
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

double jsonOperation::getUscitaSettimanaCategoria(QString cate, QDate settimana)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data().weekNumber()==settimana.weekNumber())
        {
            if((*it)->tipoMovimento()=="Uscita" && (*it)->categoria()==cate)
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

double jsonOperation::getUscitaGiornoCategoria(QString cate, QDate giorno)
{
    double toRet=0;
    QList<movimento*>::iterator it = l_listaMovimenti->begin();
    for(;it!=l_listaMovimenti->end();++it)
    {
        if((*it)->data()==giorno)
        {
            if((*it)->tipoMovimento()=="Uscita" && (*it)->categoria()==cate)
                toRet=toRet+(*it)->valore();
        }
    }
    return toRet;
}

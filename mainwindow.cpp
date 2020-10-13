#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValueRef>
#include <QPdfWriter>
#include <QPainter>
#include <QDir>
#include <QFileDialog>
#include "myqlistwidgetitemmovimento.h"
#include <QMenuBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    init();

    ui->setupUi(this);

    impostaValidator();

    //inserimento
    ui->radioButton->setChecked(true);
    ui->l_labelData->setText(ui->calendarWidget->selectedDate().toString("dd/MM/yyyy"));
    connect(ui->b_buttonIns, SIGNAL(clicked()), this, SLOT(pressioneInserimento()));
    connect(ui->calendarWidget,SIGNAL(clicked(const QDate &)),this,SLOT(pressioneCalendario(const QDate &)));
    connect(ui->b_buttonPulisci, SIGNAL(clicked()), this, SLOT(pressionePulisci()));

    //visualizza
    ui->r_radioMese->setChecked(true);
    connect(ui->calendarWidget_vis,SIGNAL(clicked(const QDate &)),this,SLOT(pressioneCalendarioVis(const QDate &)));
    popolaViewVisualizza();
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(doppioClickItemListaVisualizza(QListWidgetItem*)));
    connect(ui->b_bottonePdf, SIGNAL(clicked()), this, SLOT(writePdf()));
    connect(ui->r_radioMese,SIGNAL(clicked()),this,SLOT(popolaViewVisualizza()));
    connect(ui->r_radioGiorno,SIGNAL(clicked()),this,SLOT(popolaViewVisualizza()));
    connect(ui->r_radioSettimana,SIGNAL(clicked()),this,SLOT(popolaViewVisualizza()));

    //stat
    ui->r_radioTotStat->setChecked(true);
    connect(ui->r_radioTotStat,SIGNAL(clicked()),this,SLOT(popolaViewStatistiche()));
    connect(ui->r_radioMeseStat,SIGNAL(clicked()),this,SLOT(popolaViewStatistiche()));
    connect(ui->r_radioSettimanaStat,SIGNAL(clicked()),this,SLOT(popolaViewStatistiche()));
    connect(ui->r_radioGiornoStat,SIGNAL(clicked()),this,SLOT(popolaViewStatistiche()));
    connect(ui->calendarWidget_2,SIGNAL(clicked(const QDate &)),this,SLOT(pressioneCalendarioStat(const QDate &)));
    //currentTextChanged(const QString &text)
    connect(ui->c_comboCategoria_2,SIGNAL(currentTextChanged(const QString &)),this,SLOT(cambioComboCategoriaStatistiche(const QString &)));
}

void MainWindow::init()
{
    QDir m_mainPath = QDir::home();
    QString newPath=m_mainPath.homePath();
    newPath += "/m_movimentazione/jsonInUso";
    m_mainPath.setPath(newPath);
    if(!m_mainPath.exists())
    {
        m_mainPath.mkpath(".");
        QFile fileNew(m_mainPath.canonicalPath() + "/dataMoney.json");
        if ( fileNew.open(QIODevice::ReadWrite) )
        {
            fileNew.close();
        }
    }
    m_modelJson=new jsonOperation(&m_listaPrincipale,m_mainPath.canonicalPath() + "/dataMoney.json");
    m_modelJson->init();
    m_modelJson->loadDataFromJson();
    setStyle();

    //barra menu
    QAction *cambiaPath = new QAction("Cambia file",this);
    QMenuBar * barraMenu = new QMenuBar(this);
    QMenu *menu = new QMenu("Opzioni");
    menu->addAction(cambiaPath);
    barraMenu->addAction(menu->menuAction());
    connect(cambiaPath, SIGNAL(triggered()), this, SLOT(setNewJsonFile()));
}

void MainWindow::setStyle()
{

    //per tutti
    setStyleSheet(
        "QPushButton {"
            "border: 2px solid gray; border-radius: 10px; padding: 0 8px; background: rgb(208, 214, 211);"
        "}"
        "QPushButton:hover {"
            "border: 2px solid gray; border-radius: 10px; padding: 0 8px; background: rgb(194, 207, 201);"
        "}"
        "QPushButton:pressed { "
            "border: 2px solid gray; border-radius: 10px; padding: 0 8px; background: rgb(179, 186, 183);"
        "}"
        "QLineEdit { "
            "border: none; padding: 0 8px;"
        "}"
        "QLineEdit:focus { "
            "border: 2px solid rgb(66,165,245); padding: 0 8px;"
        "}"
        "QLineEdit:hover {"
            "background: rgb(194, 207, 201); border: none;"
        "}"
        "QListWidget {"
            "border: none; border-radius: 5px; background: rgb(208, 214, 211)"
        "}"
        "QListWidget::item {"
           "border: 1px solid gray;  border-radius: 5px; background: transparent; padding: 3px"
        "}"
        "QListWidget::item:selected {"
           "border: 1px solid gray;  border-radius: 5px; background: rgb(179, 186, 183); padding: 3px"
        "}"
        "QListWidget::item:hover {"
           "border: 1px solid gray;  border-radius: 5px; background: rgb(194, 207, 201); padding: 3px"
        "}");
}

void MainWindow::impostaValidator()
{
    ui->l_lineValore->setValidator( new QIntValidator(0,9999999,this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pulisciCampi()
{
    ui->l_lineValore->clear();
    ui->l_lineDesc->clear();
}

void MainWindow::pressioneInserimento()
{
    QDate dataToIns = ui->calendarWidget->selectedDate();
    QString movimentoToIns;
    if(ui->radioButton->isChecked()) movimentoToIns="Uscita";
    else movimentoToIns="Entrata";
    QString descToins = ui->l_lineDesc->text();
    QString categoriaMovToIns = ui->c_comboCategoria->currentText();
    double prezzoToIns = ui->l_lineValore->text().toDouble();
    movimento * mToIns = new movimento(m_modelJson->getNextId(),categoriaMovToIns,dataToIns,descToins,movimentoToIns,prezzoToIns);
    if(mToIns) m_listaPrincipale.push_back(mToIns);
    m_modelJson->insertAllDataInJson();
    calcolaTotali();
    popolaViewVisualizza();
    popolaViewStatistiche();
    pulisciCampi();
}

void MainWindow::pressioneCalendario(const QDate &)
{

    ui->l_labelData->setText(ui->calendarWidget->selectedDate().toString("dd/MM/yyyy"));//inserisco la stringa ma in formato dd/MM//yyyy
}

void MainWindow::pressioneCalendarioVis(const QDate &)
{
    popolaViewVisualizza();
}

void MainWindow::pulisciListaView()
{
    ui->listWidget->clear();
}

void MainWindow::calcolaTotali()
{
    ui->l_labelEntrataTot->setText(QString::number(m_modelJson->getSaldoTot()));
    ui->l_labelUscitaTot->setText(QString::number(m_modelJson->getUscitaTot()));
    ui->l_labelEntrataMensile->setText(QString::number(m_modelJson->getEntrataMensiole(ui->calendarWidget_vis->selectedDate())));
    ui->l_labelUscitaMensile->setText(QString::number(m_modelJson->getUscitaMensile(ui->calendarWidget_vis->selectedDate())));
    ui->l_labelEntrataSettimana->setText(QString::number(m_modelJson->getEntrataSettimana(ui->calendarWidget_vis->selectedDate())));
    ui->l_labelUscitaSettimana->setText(QString::number(m_modelJson->getUscitaSettimana(ui->calendarWidget_vis->selectedDate())));
    ui->l_labelEntrataGiorno->setText(QString::number(m_modelJson->getEntrataGiorno(ui->calendarWidget_vis->selectedDate())));
    ui->l_labelUscitaGiorno->setText(QString::number(m_modelJson->getUscitaGiorno(ui->calendarWidget_vis->selectedDate())));
}

void MainWindow::popolaViewVisualizza()
{
    pulisciListaView();
    QString getRadio;
    if(ui->r_radioMese->isChecked()) getRadio="mese";
    if(ui->r_radioGiorno->isChecked()) getRadio="giorno";
    if(ui->r_radioSettimana->isChecked()) getRadio="settimana";
    QList<movimento*>* toIns=m_modelJson->getListaSelect(ui->calendarWidget_vis->selectedDate(),getRadio);
    QList<movimento*>::iterator it = toIns->begin();
    for(;it!=toIns->end();++it)
    {
        ui->listWidget->addItem(new myQlistWidgetItemMovimento(this,(*it)));
    }
    calcolaTotali();
}

void MainWindow::doppioClickItemListaVisualizza(QListWidgetItem*)
{
    movimento * tmp;
    tmp = (dynamic_cast<myQlistWidgetItemMovimento*>(ui->listWidget->currentItem()))->getOggetto();
    modifica * dialogMod = new modifica(this);
    dialogMod->setObj(tmp);
    dialogMod->setListaPrincipale(&m_listaPrincipale);
    dialogMod->popolaModifica();
    dialogMod->exec();
    popolaViewVisualizza();
    popolaViewStatistiche();
    m_modelJson->insertAllDataInJson();
}

void MainWindow::pressionePulisci()
{
    pulisciCampi();
}

void MainWindow::writePdf()
{
//    const QString fileName("/Users/alessio/Desktop/movimenti.pdf");
//    //const QImage image("/home/patrick/truck.jpg");

//    QPdfWriter pdfWriter(fileName);
//    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
//    pdfWriter.setTitle("Movimenti");
//    QPainter painter(&pdfWriter);
    QString getRadio;
    if(ui->r_radioMese->isChecked()) getRadio="mese";
    if(ui->r_radioGiorno->isChecked()) getRadio="giorno";
    if(ui->r_radioSettimana->isChecked()) getRadio="settimana";
    QList<movimento*>* toIns=m_modelJson->getListaSelect(ui->calendarWidget_vis->selectedDate(),getRadio);
    QList<movimento*>::iterator it = toIns->begin();
    QString toPdf;
    for(;it!=toIns->end();++it)
    {
        toPdf.append((*it)->getInfo());
        toPdf.append(QString("%1%2%3").arg("\n").arg("\n").arg("\n"));
    }
//    painter.drawText(0,0,toPdf);


    //QFile caFile("output.txt");

    QString newPath;
    newPath = QFileDialog::getExistingDirectory(this,
                                                tr("Choose Or Create Directory"),
                                                "",
                                                QFileDialog::DontResolveSymlinks);



    QFile caFile(newPath + "/movimenti.txt");
    caFile.open(QIODevice::WriteOnly | QIODevice::Text);

    if(!caFile.isOpen()){
        qDebug() << "- Error, unable to open" << "outputFilename" << "for output";
    }
    QTextStream outStream(&caFile);
    outStream << toPdf;
    caFile.close();
}

void MainWindow::pressioneCalendarioStat(const QDate &)
{
    popolaViewStatistiche();
}

void MainWindow::popolaViewStatistiche()
{
    QString cate = ui->c_comboCategoria_2->currentText();
    double spesa =0;
    double entrata =0;
    QDate toS = ui->calendarWidget_2->selectedDate();
    if(ui->r_radioTotStat->isChecked())
    {
        entrata=m_modelJson->getEntrataTotCategoria(cate);
        spesa=m_modelJson->getUscitaTotCategoria(cate);
    }
    else if(ui->r_radioMeseStat->isChecked())
    {
        entrata=m_modelJson->getEntrataMeseCategoria(cate,toS);
        spesa=m_modelJson->getUscitaMeseCategoria(cate,toS);
    }
    else if(ui->r_radioSettimanaStat->isChecked())
    {
        entrata=m_modelJson->getEntrataSettimanaCategoria(cate,toS);
        spesa=m_modelJson->getUscitaSettimanaCategoria(cate,toS);
    }
    else if(ui->r_radioGiornoStat->isChecked())
    {
        entrata=m_modelJson->getEntrataGiornoCategoria(cate,toS);
        spesa=m_modelJson->getUscitaGiornoCategoria(cate,toS);
    }
    ui->l_labelUscitaTotStat->setText(QString::number(spesa));
    ui->l_labelEntrataTotStat->setText(QString::number(entrata));
    ui->groupBox_4->setTitle(QString("Spese ed entrate per la categoria: %1").arg(cate));


}

void MainWindow::cambioComboCategoriaStatistiche(const QString &text)
{
    popolaViewStatistiche();
}

void MainWindow::setNewJsonFile()
{
    QString newPath;
    newPath = QFileDialog::getOpenFileName(this,
                                          tr("Open Directory"),
                                          newPath,"*.json");

    if(!newPath.isEmpty())
        {
        m_modelJson->setFileJson(newPath);
        m_listaPrincipale.clear();
        m_modelJson->loadDataFromJson();
    }

}




#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "jsonoperation.h"
#include "modifica.h"
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QList<movimento*> m_listaPrincipale;
    jsonOperation * m_modelJson;
public:
    MainWindow(QWidget *parent = nullptr);
    void init();
    void impostaValidator();
    ~MainWindow();
    void pulisciCampi();
    void setStyle();
private:
    Ui::MainWindow *ui;
public slots:
    void pressioneInserimento();
    void pressioneCalendario(const QDate &);
    void pressioneCalendarioVis(const QDate &);
    void pulisciListaView();
    void calcolaTotali();
    void popolaViewVisualizza();
    void doppioClickItemListaVisualizza(QListWidgetItem*);
    void pressionePulisci();
    void writePdf();
    void pressioneCalendarioStat(const QDate &);
    void popolaViewStatistiche();
    void cambioComboCategoriaStatistiche(const QString &);
    void setNewJsonFile();
};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myitem.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include "kmean.h"

#include <QGraphicsView>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QRadioButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    //sloty po zmacknuti tlacitek
    void vygenerujBody();    //slot na vygenerovani bodu
    void vygenerujCentroidy(); //slot na vygenerovani centroidu
    void vyres();             //slot na vyreseni ulohy kmeans
    void aktualizuj(bool itemreleased); //slot reagujici na signaly z myitemu
    void nastavAktualizace();   //slot reagujici na signaly z radiobuttonu
    void nactiSoubor();
private:
    QGraphicsView * graphicsview;
    QLabel * label_body;
    QLabel * label_centroidy;
    QSpinBox * spinbox_body;
    QSpinBox * spinbox_centroidy;
    QPushButton * button_vygeneruj_body;
    QPushButton * button_vygeneruj_centroidy;
    QPushButton * button_vypocti;

    QRadioButton * radiobutton_vypocti_po; // vypocti po presunuti
    QRadioButton * radiobutton_vypocti_pri;
    QRadioButton * radiobutton_nepocti_auto; // nepocitej automaticky


    void vytvor_ui();

    QGraphicsScene * scena;     //platno
    QVector<myitem *> body;     //vektor bodu
    QVector<myitem *> centroidy;//vektor centroidu
    QVector<QGraphicsLineItem *> linky; //vektor linek spojujici bod ve shluku s centroidem
    QVector<QGraphicsLineItem *> hraniceVoronehoBunek; //vektor linek tvorici hranice voroneho bunek

    unsigned int velikostBodu;     //velikost bodu na platne

    unsigned int sirkaPlatna;    //sirka platna (QGraphicsView)
    unsigned int vyskaPlatna;    //vyska platna (QGraphicsView)

    unsigned int pocetBodu;      //pocet bodu
    unsigned int pocetCentroidu; //pocet centroidu

    bool povolitAktualizaci;    //boolean, ve kterem je ulozeno zda-li byly vypocteny shluky
    int rezimAktualizace;       //rezim aktualizace obsahuje cisla 0 nebo 1 nebo 2
                                // 0 - nebude dochazet k aktulizacim
                                // 1 - bude dochazet k aktualizacim po pusteni bodu ci centroidu
                                // 2 - bude dochazet k aktualizacim pri presouvani bodu ( centroidu ne)

    void vygeneruj(bool points); //funkce na vygenerovani nahodnych pozic bodu ci centroidu (podle vstupniho parametru)

    void smazBody(bool points); //funkce na smazani bodu(points) a centroidu ze sceny
    void smazLinky();     //funkce na smazani linek a hranic ze sceny

    void voronoi();       //funkce na vykresleni hranic voroneho bunek

    void ulozBodyDoVektoru(QVector<Centroid> &centroidy, QVector<Bod> &body); //ulozi body do vektoru (predela QVector<myitem*> na QVector<Bod>)

    void resizeEvent(QResizeEvent* event);
};
#endif // MAINWINDOW_H

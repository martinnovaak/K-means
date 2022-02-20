#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myitem.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include "kmean.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    //sloty po zmacknuti tlacitek
    void generatePoints();    //slot na vygenerovani bodu
    void generateCentroids(); //slot na vygenerovani centroidu
    void solve();             //slot na vyreseni ulohy kmeans
private:
    Ui::MainWindow *ui;
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

    void vygeneruj(bool points); //funkce na vygenerovani nahodnych pozic bodu ci centroidu (podle vstupniho parametru)

    void smazBody(bool points); //funkce na smazani bodu(points) a centroidu ze sceny
    void smazLinky();     //funkce na smazani linek a hranic ze sceny

    void voronoi();         //funkce na vykresleni hranic voroneho bunek

    void ulozBodyDoVektoru(QVector<Centroid> &centroidy, QVector<Bod> &body); //ulozi body do vektoru (predela QVector<myitem*> na QVector<Bod>)
};
#endif // MAINWINDOW_H

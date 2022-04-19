#ifndef CENTROID_H
#define CENTROID_H

#include "bod.h"

//Centroid je specialni pripad bodu - ma take n-souradnic
//navic ale taky obsahuje vlastni shluk
class Centroid : public Bod
{
private:
    unsigned int pocet;                     //pocet bodu ve shluku
    QVector<unsigned int> bodyVeShluku;     //body ve shluku (vektor obsahuje pouze indexy jednotlivych bodu)
    QVector<double> novysouradnice;         //souradnice centroidu v nove iteraci (v zakladu funguje jako soucet bodu, ktere se pridavaji
                                            //funkci pridejDoShluku, po zavolani funkce novyCentroid se vydeli poctem bodu ve skluku)
    friend class MainWindow;
public:
    Centroid();
    Centroid(const double * bod, unsigned int dim);//konstruktor
    Centroid(const Centroid & c);           //kopirovaci konstruktor
    ~Centroid();                            //destruktor
    double operator[](unsigned int i) const;//pretizeny operator[] vraci i-tou souradnici

    void pridejDoShluku(unsigned int index, const Bod & bod);//funkce ktera prida bod do shluku
    void vycistiShluk();                    //funkce na vycisteni shluku
    bool novyCentroid();                    //funkce na overeni zda-li jsou nove souradnice odlisne od tech starych
};

#endif // CENTROID_H

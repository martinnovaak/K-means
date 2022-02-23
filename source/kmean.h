#ifndef KMEAN_H
#define KMEAN_H

#include "bod.h"
using namespace std;

class KMean
{
private:
    QVector<Centroid> centroidy;
    QVector<Bod> body;

    unsigned int pocetCentroidu;
    unsigned int pocetBodu;

    unsigned int dimenze;

    void priradBodyKCentroidum();
    unsigned int nalezniNejblizsiho(const Bod & bod);
    double vzdalenost(const Bod & a, const Bod & b);
    void vynuluj();
    bool vypoctiNoveCentroidy();
    void ulozPozici();
public:
    //KMean(QVector<Centroid>& centroidy, QVector<Bod>& body, unsigned int pocetcentroidu, unsigned int pocetbodu, unsigned int dimenze);
    KMean(QVector<Centroid>& centroidy, QVector<Bod>& body, unsigned int pocetcentroidu, unsigned int pocetbodu, unsigned int dimenze);
    void vyres();
    double operator()(unsigned int i, unsigned int j);
    const Centroid & operator[](unsigned int i);
};

#endif // KMEAN_H

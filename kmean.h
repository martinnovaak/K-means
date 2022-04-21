#ifndef KMEAN_H
#define KMEAN_H

#include "centroid.h"
using namespace std;

class KMean
{
private:
    vector<Centroid> centroidy;
    vector<Bod> body;

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
    KMean(vector<Centroid>& centroidy, vector<Bod>& body, unsigned int dimenze);
    void vyres();
    double operator()(unsigned int i, unsigned int j);
    const Centroid & operator[](unsigned int i);
};

#endif // KMEAN_H

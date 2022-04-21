#include "kmean.h"

KMean::KMean(vector<Centroid>& centroidy, vector<Bod>& body, unsigned int dimenze)
    : pocetCentroidu(centroidy.size()),pocetBodu(body.size()),dimenze(dimenze)
{
    for(unsigned int i = 0; i < pocetCentroidu; i++)
    {
        this->centroidy.push_back(Centroid(centroidy.at(i)));
    }
    for(unsigned int i = 0; i < pocetBodu; i++)
    {
        this->body.push_back(Bod(body.at(i)));
    }
}

//nalezne k bodu jeho nejblizsi centroid a vlozi ho do jeho shluku
void KMean::priradBodyKCentroidum()
{
    unsigned int index;
    for (unsigned int i = 0; i < this->pocetBodu; i++)
    {
        index = nalezniNejblizsiho(body[i]);
        this->centroidy[index].pridejDoShluku(i, this->body[i]);
    }
}

//nalezne nejblizsi centroid k aktualnimu bodu
//vrati jeho index ve vektoru
unsigned int KMean::nalezniNejblizsiho(const Bod &bod)
{
    unsigned int index = 0;
    double min = vzdalenost(bod, this->centroidy[0]);
    double pom;
    for (unsigned int i = 1; i < this->pocetCentroidu; i++)
    {
        pom = vzdalenost(bod, centroidy[i]);
        if (min > pom)
        {
            min = pom;
            index = i;
        }
    }
    return index;
}

//vypocet euklidovske vzdalenosti
double KMean::vzdalenost(const Bod &a, const Bod &b)
{
    double suma = 0.0;
    for (unsigned int i = 0; i < dimenze; i++)
    {
        suma += (a[i]-b[i])*(a[i]-b[i]);
    }
    return suma;
}

//vycisti aktualni shluk
void KMean::vynuluj()
{
    for(unsigned int i = 0; i < this->pocetCentroidu; i++)
        centroidy[i].vycistiShluk();
}

//vypocte novou iteraci, pokud jsou nove souradnice vsech centroidu stejne -> vrati false jinak vrati true
bool KMean::vypoctiNoveCentroidy()
{
    bool pokracuj = false;
    for(unsigned int i = 0; i < this->pocetCentroidu; i++)
    {
        if(!this->centroidy[i].novyCentroid())
            pokracuj = true;
    }
    return pokracuj;
}

void KMean::vyres()
{
    bool pokracuj = true;
    while(pokracuj)
    {
        vynuluj();                         // vymaz predchozi shluky
        priradBodyKCentroidum();           // a najdi nove shluky
        pokracuj = vypoctiNoveCentroidy(); // jsou souradnice stejna jako v predchozi iteraci?
    }
}

//vrati j-tou souradnici i-teho centroidu
double KMean::operator()(unsigned int i, unsigned int j)
{
    if (i < this->pocetCentroidu && j < this->dimenze)
        return this->centroidy[i][j];
    else
    {
        return -1;
    }
}

//vrati referenci na i-ty centroid
const Centroid & KMean::operator[](unsigned int i)
{
    if(i < this->pocetCentroidu)
    {
        return this->centroidy[i];
    }
    else
        throw -1;
}


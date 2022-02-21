#include "bod.h"

Bod::Bod(const double *bod, unsigned int dim)
{
    this->dim = dim;
    souradnice = new double[dim];
    for(unsigned int i = 0; i < dim; i++)
    {
        souradnice[i] = bod[i];
    }
}

Bod::Bod(const Bod &b) : dim(b.dim)
{
    souradnice = new double[dim];
    for (unsigned int i = 0; i < dim; i++)
        souradnice[i] = b[i];
}

Bod::~Bod()
{
    delete[] souradnice;
}

double Bod::operator[](unsigned int i) const
{
    if(i < dim)
        return souradnice[i];
    else
        return -1;
}

Centroid::Centroid(const double *bod, unsigned int dim) : Bod(bod, dim)
{
    this->pocet = 0;
    for(unsigned int i = 0; i < this->dim; i++)
        novysouradnice.push_back(0.0);
}

Centroid::Centroid(const Centroid &c) : Bod(c.souradnice, c.dim), pocet(c.pocet)
{
    for (unsigned int i = 0; i < this->pocet; i++)
        bodyVeShluku.push_back(c[i]);
    for(unsigned int i = 0; i < this->dim; i++)
        novysouradnice.push_back(0.0);
}

Centroid::~Centroid()
{
    vycistiShluk();
}

double Centroid::operator[](unsigned int i) const
{
    if(i < dim)
        return souradnice[i];
    else
        return -1;
}

void Centroid::pridejDoShluku(unsigned int index, const Bod &bod)
{
    bodyVeShluku.push_back(index); //prida index bodu do vektoru shluku
    for (unsigned int i = 0; i < this->dim; i++) //pricte bod do novych souradnic
        this->novysouradnice[i] += bod[i];
    this->pocet++;
}

void Centroid::vycistiShluk()
{
    for (unsigned int i = 0; i < this->pocet; i++)
        bodyVeShluku.pop_back();
    for (unsigned int i = 0; i < this->dim; i++)
        novysouradnice[i] = 0.0;
    pocet = 0;

}

bool Centroid::novyCentroid()
{
    if(pocet == 0) //pokud je shluk prazdny centroid zustava na stejnem miste -> return true
        return true;
    bool stejny = true;
    for (unsigned int  i = 0; i < this->dim; i++)
    {
        this->novysouradnice[i] /= this->pocet; //nova souradnice je ve stredu shluku
        if (this->novysouradnice[i] != this->souradnice[i])
            stejny = false;
    }
    if(stejny == false) //bude-li centroid premisten na novou pozici - zmen jeho souradnice
    {
        for (unsigned int  i = 0; i < this->dim; i++)
            this->souradnice[i] = novysouradnice[i];
    }
    return stejny;
}

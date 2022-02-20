#ifndef BOD_H
#define BOD_H

#include <vector>

class Bod
{
protected:
    double * souradnice;
    unsigned int dim;
public:
    Bod(double * bod, unsigned int dim)
    {
        this->dim = dim;
        souradnice = new double[dim];
        for(unsigned int i = 0; i < dim; i++)
        {
            souradnice[i] = bod[i];
        }
    }

    Bod(const Bod& b) : dim(b.dim)
    {
        souradnice = new double[dim];
        for (unsigned int i = 0; i < dim; i++)
            souradnice[i] = b[i];
    }

    ~Bod()
    {
        delete[] souradnice;
    }

    double operator[](unsigned int i) const
    {
        if(i < dim)
            return souradnice[i];
        else
            return -1;
    }
};

class Centroid : public Bod
{
private:
    unsigned int pocet;
    std::vector<unsigned int> bodyVeShluku;
    std::vector<double> novysouradnice;

    friend class MainWindow;
public:
    Centroid(double * bod, unsigned int dim) : Bod(bod, dim)
    {
        this->pocet = 0;
        for(unsigned int i = 0; i < this->dim; i++)
            novysouradnice.push_back(0.0);
    }

    Centroid(const Centroid & c) : Bod(c.souradnice, c.dim), pocet(c.pocet)
    {
        for (unsigned int i = 0; i < this->pocet; i++)
            bodyVeShluku.push_back(c[i]);
        for(unsigned int i = 0; i < this->dim; i++)
            novysouradnice.push_back(0.0);
    }

    ~Centroid()
    {
        vycistiShluk();
        //delete[] souradnice;
    }

    double operator[](unsigned int i) const
    {
        if(i < dim)
            return souradnice[i];
        else
            return -1;
    }

    void pridejDoShluku(unsigned int index, Bod & bod)
    {
        bodyVeShluku.push_back(index);
        for (unsigned int i = 0; i < this->dim; i++)
            this->novysouradnice[i] += bod[i];
        this->pocet++;
    }

    void vycistiShluk()
    {
        for (unsigned int i = 0; i < this->pocet; i++)
            bodyVeShluku.pop_back();
        for (unsigned int i = 0; i < this->dim; i++)
            novysouradnice[i] = 0.0;
        pocet = 0;

    }

    bool novyCentroid()
    {
        if(pocet == 0)
            return true;
        bool stejny = true;
        for (unsigned int  i = 0; i < this->dim; i++)
        {
            this->novysouradnice[i] /= this->pocet;
            if (this->novysouradnice[i] != this->souradnice[i])
                stejny = false;
            if(!stejny)
                this->souradnice[i] = novysouradnice[i];
        }
        return stejny;
    }
};

#endif // BOD_H

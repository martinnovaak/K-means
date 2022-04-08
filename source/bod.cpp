#include "bod.h"
#include <QMessageBox>

Bod::Bod()
{

}

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
    return souradnice[i];
}

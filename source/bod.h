#ifndef BOD_H
#define BOD_H

#include <QVector>

//Bod v n-te dimenzi obsahuje n souradnic
class Bod
{
protected:
    double * souradnice; // souradnice bodu
    unsigned int dim;    // dimenze
public:
    Bod();
    Bod(const double * bod, unsigned int dim);    //konstruktor
    Bod(const Bod& b);                          //kopirovaci konstruktor
    ~Bod();                                     //destruktor
    double operator[](unsigned int i) const;    //pretizeny operator[] vraci i-tou souradnici
};

#endif // BOD_H

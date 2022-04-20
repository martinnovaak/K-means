#include "dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>


Dialog::Dialog(unsigned int velikost_centroidu, unsigned int velikost_bodu) :  velikost_bod(velikost_bodu),velikost_centroid(velikost_centroidu)
{
    QVBoxLayout * layout = new QVBoxLayout();
    QHBoxLayout * radka_centroid = new QHBoxLayout();
    this->labelCentroid = new QLabel("Velikost centroidu");
    this->spinboxCentroid = new QSpinBox;
    this->spinboxCentroid->setMaximum(20);
    this->spinboxCentroid->setMinimum(5);
    this->spinboxCentroid->setValue(velikost_centroidu);
    radka_centroid->addWidget(labelCentroid);
    radka_centroid->addWidget(spinboxCentroid);
    layout->addLayout(radka_centroid);

    QHBoxLayout * radka_bod = new QHBoxLayout();
    this->labelBod = new QLabel("Velikost bodu");
    this->spinboxBod = new QSpinBox;
    this->spinboxBod->setMaximum(20);
    this->spinboxBod->setMinimum(5);
    this->spinboxBod->setValue(velikost_bodu);
    radka_bod->addWidget(labelBod);
    radka_bod->addWidget(spinboxBod);
    layout->addLayout(radka_bod);

    this->buttonReturn = new QPushButton("Ulož");
    layout->addWidget(buttonReturn);
    this->setLayout(layout);

    connect(buttonReturn, &QPushButton::clicked, this, &Dialog::stisknuto);
}

void Dialog::stisknuto()
{
    this->velikost_bod = spinboxBod->value();
    this->velikost_centroid = spinboxCentroid->value();
    accept();
}

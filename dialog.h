#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
class QLabel;
class QSpinBox;
class QPushButton;

class Dialog : public QDialog
{
    Q_OBJECT
private:
    QLabel * labelCentroid;
    QSpinBox * spinboxCentroid;
    QLabel * labelBod;
    QSpinBox * spinboxBod;
    QPushButton * buttonReturn;

    unsigned int velikost_bod;
    unsigned int velikost_centroid;
public:
    explicit Dialog(unsigned int velikost_centroidu, unsigned int velikost_bodu);
    unsigned int getBod(){return velikost_bod;};
    unsigned int getCentroid(){return velikost_centroid;};
private slots:
    void stisknuto();
};

#endif // DIALOG_H

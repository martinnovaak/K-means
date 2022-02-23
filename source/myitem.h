#ifndef MYITEM_H
#define MYITEM_H
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QVector>

class myitem : public QObject, public QGraphicsItem
{
    Q_OBJECT
private:
    double souradnice_x;                 // x-ova souradnice pro inicializaci
    double souradnice_y;                 // y-ova souradnice pro inicializaci
    double zx;                           // x-ova souradnice po premisteni uzivatelem na jinou pozici
    double zy;                           // y-ova souradnice po premisteni uzivatelem na jinou pozici
    double wh;                           // strana ctverce (item ackoli se vykresluje jen kruh tak vystupuje jako ctverec)
    Qt::GlobalColor color;               // aktualni barva kruhu
    Qt::GlobalColor pomColor;            // pomocná proměnná, ve ktere je ulozena hodnota barvy z konstruktoru
    QVector<QGraphicsLineItem *> lines;  // linky pripojene k itemu (centroid muze mit vice bod max 1)
    bool centroid;                       // boolean rikajici, jestli se jedna o centroid ci ne
public:
    myitem(double x, double y, double wh, Qt::GlobalColor color);   // konstruktor
    virtual ~myitem();
    //gettery a settery
    double getX(){return zx;};            // getter na x-ovou souradnici leveho horniho rohu itemu
    double getY(){return zy;};            // getter na y-ovou souradnici leveho horniho rohu itemu
    double getCenterX(){return zx +wh/2;};// getter na stredovou x-ovou souradnici itemu
    double getCenterY(){return zy +wh/2;};// getter na stredovou y-ovou souradnici itemu
    void setX(double x){zx = x;};         // setter na stredovou x-ovou souradnici itemu
    void setY(double y){zy = y;};         // setter na stredovou y-ovou souradnici itemu
protected:
//metody zabyvajici se vykreslovanim itemu
//overridnute metody pro vykresleni itemu
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget) override;

//metody zabyvajici posouvanim linky soucasne s bodem
public:
    void addLine(QGraphicsLineItem *line, bool centroid); // metoda na spojeni linky s itemem
    void deleteLines();
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override; //overridnuta metoda zachytavajici zmenu polohy itemu
private:
    void moveLineToCenter();    //metoda na zmenu polohy linek
// metody na interakci itemu s mysi
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;                       // zmacknuti mysi
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;                     // pusteni mysi
//signaly
signals:
    void itemReleased();    //signal vysilajici informaci ze item byl pusten mysi
    void itemChanged();     //signal vysilajici informaci ze item byl posunut
};

#endif // MYITEM_H

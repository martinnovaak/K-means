#include "myitem.h"
#include <QToolTip>
#include <QGraphicsScene>

myitem::myitem(double x, double y, double wh, Qt::GlobalColor color)
{
    souradnice_x = x;
    souradnice_y = y;
    zx = x;
    zy = y;
    this->wh = wh;
    this->color = this->pomColor = color;

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    setToolTip(QString::number(souradnice_x) + " " + QString::number(souradnice_y));
}

//ctverec predstavujici item na platne
QRectF myitem::boundingRect() const
{
    return QRectF(souradnice_x, souradnice_y, wh, wh);
}

void myitem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRectF rect = boundingRect();
    painter->setBrush(color);
    painter->setPen(color);
    painter->drawEllipse(rect);
}

//item dostane informaci jedna-li se o centroid ci ne
//prida linku do vektoru linek
void myitem::addLine(QGraphicsLineItem *line, bool centroid)
{
    this->centroid = centroid;
    this->lines.push_back(line);
}

//pri zmene pozice nastavi souradnice zx a zy na nove souradnice, nastavi novy tooltip a zavola metodu na zmenu jednoho vrcholu linky
//pro spravny chod metody ji jeste zavola v predkovi
QVariant myitem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) //zmenila-li se pozice
    {
        // ve value je ulozena hodnota noveho leveho horniho rohu itemu
        QPointF newPos = value.toPointF();
        double nx = souradnice_x + newPos.x();
        double ny = souradnice_y + newPos.y();
        bool mimo = false;
        if (nx < 10)
        {
            newPos.setX(10-souradnice_x);
            setInfo(newPos);
            mimo = true;
        }
        if(ny < 5)
        {
            newPos.setY(5-souradnice_y);
            setInfo(newPos);
            mimo = true;
        }
        if (mimo)
            return newPos;
        setInfo(newPos);
        this->scene()->update();
    }
    this->update();
    return QGraphicsItem::itemChange(change, value); //zavolej metodu predka
}


void myitem::setInfo(QPointF newPos)
{
    zx = souradnice_x + newPos.x();
    zy = souradnice_y + newPos.y();
    setToolTip(QString::number(zx) + " " + QString::number(zy)); //zmena tooltipu na novou pozici
    moveLineToCenter(); //presun linku na novy stred itemu
    emit itemChanged();//zavolej signal
}

//po stisknuti itemu se zmeni jeho barva na magentovou a jeho linky na cervenou
void myitem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    //1) zmeni barvy
    this->color = Qt::magenta;
    for(int i = 0; i < this->lines.size(); i++)
        lines[i]->setPen(QPen(Qt::red));
    //paint se zavola az pri tazeni - barva se tedy meni pouze pri tazeni itemy po scene
    QGraphicsItem::mousePressEvent(event);
}

//po uvolneni itemu se barvy na zpet
void myitem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    //1) zmeni barvy
    this->color = this->pomColor;
    for(int i = 0; i < this->lines.size(); i++)
        lines[i]->setPen(QPen(Qt::black));
    //2)nakresli znovu item
    QGraphicsItem::mouseReleaseEvent(event);
    this->update(); //po uvolneni uz se paint nevola, je treba zavolat rucne
    emit itemReleased(); //zavolej signal
}

//linka je mezi dvema body (p1,p2) p1 je vzdy centroid a p2 je vzdy bod
//metoda zmeni jeden jeho bod na novy stred
void myitem::moveLineToCenter()
{
     QPointF novyStred = QPointF(zx + wh/2, zy + wh/2);
     for (int i = 0; i < this->lines.length(); i++)
     {
         QPointF p1 = centroid ? novyStred : lines[i]->line().p1(); // jedna-li se o centroid p1 je novyStred jinak p1 je porad stejny
         QPointF p2 = centroid ? lines[i]->line().p2() : novyStred; // jedna-li se o centroid p2 je stejny bod jinak je p2 novyStred
         lines[i]->setLine(QLineF(p1, p2));                         // nastav linku na (p1,p2)
     }
}

void myitem::deleteLines()
{
    lines.clear();
}

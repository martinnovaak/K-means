#include "myitem.h"
#include <QToolTip>

#define UNUSED(expr) (void)(expr)

myitem::myitem(double x, double y, double wh, Qt::GlobalColor color)
{
    souradnice_x = x;
    souradnice_y = y;
    zx = x;
    zy = y;
    this->wh = wh;
    this->color = this->pom = color;
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    setToolTip(QString::number(souradnice_x) + " " + QString::number(souradnice_y));
}

QRectF myitem::boundingRect() const
{
    return QRectF(souradnice_x, souradnice_y, wh, wh);
}

void myitem::paint(QPainter *painter, [[maybe_unused]] const QStyleOptionGraphicsItem *option, [[maybe_unused]] QWidget *widget)
{   
    QRectF rect = boundingRect();
    painter->setBrush(color);
    painter->setPen(color);
    painter->drawEllipse(rect);
}

void myitem::addLine(QGraphicsLineItem *line, bool centroid)
{
    this->centroid = centroid;
    this->lines.push_back(line);
}


QVariant myitem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
          // value is the new position.
          QPointF newPos = value.toPointF();
          zx = souradnice_x + newPos.x();
          zy = souradnice_y + newPos.y();
          setToolTip(QString::number(zx) + " " + QString::number(zy));

          moveLineToCenter();
    }
    return QGraphicsItem::itemChange(change, value);
}

void myitem::moveLineToCenter()
{
     QPointF newCenterPos = QPointF(zx + wh/2, zy + wh/2);


     for (int i = 0; i < this->lines.length(); i++)
     {
         QPointF p1 = centroid ? newCenterPos : lines[i]->line().p1();
         QPointF p2 = centroid ? lines[i]->line().p2() : newCenterPos;
         lines[i]->setLine(QLineF(p1, p2));
     }
}

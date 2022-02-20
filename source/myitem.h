#ifndef MYITEM_H
#define MYITEM_H
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QVector>

class myitem : public QGraphicsItem
{
    double souradnice_x;
    double souradnice_y;
    double zx;
    double zy;
    double wh;
    Qt::GlobalColor color;
    Qt::GlobalColor pom;

    QVector<QGraphicsLineItem *> lines;
    bool centroid;
public:
    myitem(double x, double y, double wh, Qt::GlobalColor color);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget) override;

    double getX(){return zx;};
    double getY(){return zy;};
    double getCenterX(){return zx +wh/2;};
    double getCenterY(){return zy +wh/2;};
    void setX(double x){zx = x;};
    void setY(double y){zy = y;};

    void addLine(QGraphicsLineItem *line, bool centroid);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void moveLineToCenter();
};

#endif // MYITEM_H

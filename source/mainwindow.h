#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myitem.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include "kmean.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void posun();
private slots:
    void generatePoints();
    void generateCentroids();
    void solve();
private:
    Ui::MainWindow *ui;
    QGraphicsScene * scene;
    QVector<myitem *> points;
    QVector<myitem *> centroids;
    QVector<QGraphicsLineItem *> lines;
    QVector<QGraphicsLineItem *> voronoilines;

    unsigned int sizeOfItem;

    unsigned int width;
    unsigned int height;

    unsigned int numberOfPoints;
    unsigned int numberOfCentroids;

    void generate(bool points);
    void clearScene(bool points);

    void removeLines();

    void voronoi();

    void addPointsToVector(std::vector<Centroid> &centroidy, std::vector<Bod> &body);
};
#endif // MAINWINDOW_H

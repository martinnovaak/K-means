#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QRandomGenerator>
#include "VoronoiDiagramGenerator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),sizeOfItem(10), width(719-sizeOfItem/2), height(469-sizeOfItem/2), numberOfPoints(50), numberOfCentroids(5)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(0,0,width,height);
    generate(true);
    generate(false);

    connect(ui->solveButton, &QPushButton::clicked, this, &MainWindow::solve);
    connect(ui->pointButton, &QPushButton::clicked, this, &MainWindow::generatePoints);
    connect(ui->centroidButton, &QPushButton::clicked, this, &MainWindow::generateCentroids);
}

MainWindow::~MainWindow()
{
    delete ui;
    this->points.clear();
    this->centroids.clear();
    this->lines.clear();
    this->voronoilines.clear();
}

//vygeneruje nahodne body nebo centroidy na platno
void MainWindow::generate(bool p)
{
    double w,h;
    QRandomGenerator g;

    std::uniform_real_distribution<double> x(0,width-sizeOfItem/2);
    std::uniform_real_distribution<double> y(0,height-sizeOfItem/2);

    unsigned int number = (p == true) ? numberOfPoints : numberOfCentroids;

    for (unsigned int i = 0; i < number; i++ )
    {
        w = x(*QRandomGenerator::global());
        h = y(*QRandomGenerator::global());
        myitem * el ;
        if (p)
        {
            el = new myitem(w, h, 10, Qt::darkYellow);
            points.push_back(el);
        }
        else
        {
            el = new myitem(w, h, 10, Qt::red);
            centroids.push_back(el);
        }

        scene->addItem(el);
    }
}

//smaze body a centroidy ze sceny
void MainWindow::clearScene(bool p)
{
    if (p)
    {
        for (int i = 0; i < points.length(); i++)
        {
            scene->removeItem(points[i]);
        }
        points.clear();
    }
    else
    {
        for (int i = 0; i < centroids.length(); i++)
        {
            scene->removeItem(centroids[i]);
        }
        centroids.clear();
    }
}

//samze linky ze sceny
void MainWindow::removeLines()
{
    for(int i = 0; i < lines.length(); i++)
        this->scene->removeItem(lines[i]);
    for(int i = 0; i < voronoilines.length(); i++)
        this->scene->removeItem(voronoilines[i]);
    lines.clear();
    voronoilines.clear();
}

//vypocita linky voroneho diagramu
//pouzit fortuneho algoritmus ze stranky: https://web.archive.org/web/20131207065132/http://www.skynet.ie/~sos/mapviewer/voronoi.php
void MainWindow::voronoi()
{
    int c = this->centroids.length();
    float * x = new float[c];
    float * y = new float[c];
    for (int i = 0; i < c; i++)
    {
        x[i] = (float)centroids[i]->getX();
        y[i] = (float)centroids[i]->getY();
    }

    VoronoiDiagramGenerator vdg;
    vdg.generateVoronoi(x,y,c, 0, width,0,height,sizeOfItem/2);

    vdg.resetIterator();

    float x1,y1,x2,y2;
    while(vdg.getNext(x1,y1,x2,y2))
    {
        x1 = (x1 < 0.01) ? 0 : x1 + sizeOfItem/2;
        y1 = (y1 < 0.01) ? 0 : y1 + sizeOfItem/2;
        QGraphicsLineItem *line = scene->addLine(QLineF(x1,y1,x2+sizeOfItem/2,y2+sizeOfItem/2), QPen(Qt::blue, 2));
        voronoilines.push_back(line);
    }
    delete[] x;
    delete[] y;
}

void MainWindow::addPointsToVector(std::vector<Centroid> &centroidy, std::vector<Bod> &body)
{
    double * point;
    point = new double[2];
    for (int i = 0; i < points.length(); i++)
    {
        point[0] = points[i]->getX();
        point[1] = points[i]->getY();
        body.push_back(Bod(point, 2));
    }
    for (int i = 0; i < centroids.length(); i++)
    {
        scene->removeItem(centroids[i]);
        point[0] = centroids[i]->getX();
        point[1] = centroids[i]->getY();
        centroidy.push_back(Centroid(point, 2));
    }
    delete[] point;
}

//vygeneruje nove body (nejprve smaze ty stare a vygeneruje nove)
void MainWindow::generatePoints()
{
    clearScene(true);
    this->numberOfPoints = ui->spinBox->value();
    generate(true);
    removeLines();
}

//vygeneruje nove centroidy (nejprve smaze ty stare a vygeneruje nove)
void MainWindow::generateCentroids()
{
    clearScene(false);
    this->numberOfCentroids = ui->spinBox_2->value();
    generate(false);
    removeLines();
}

void MainWindow::solve()
{
    //odstrani stare linky mezi body a linky voroneho diagramu
    removeLines();

    //parametry pro inicializaci tridy kmean
    std::vector<Centroid> centroidy;
    std::vector<Bod> body;
    addPointsToVector(centroidy, body);
    centroids.clear();

    KMean kmin(centroidy, body, numberOfCentroids, numberOfPoints, 2);
    kmin.vyres();

    for (unsigned int i = 0; i < numberOfCentroids; i++)
    {
        //nové centroidy
        myitem * el = new myitem(kmin(i,0), kmin(i,1), sizeOfItem, Qt::red);
        centroids.push_back(el);
        //nalezne body v aktuálním shluku
        for(unsigned int j = 0; j < kmin[i].pocet; j++)
        {
            unsigned int bod = kmin[i].bodyVeShluku[j]; // index bodu ve shluku
            QGraphicsLineItem *line = scene->addLine(QLineF(el->getCenterX(),el->getCenterY(),points[bod]->getCenterX(),points[bod]->getCenterY()));
            lines.push_back(line);
            //spojení linky s krajními body (centroidem a bodem)
            this->points[bod]->addLine(line, false);
            this->centroids[i]->addLine(line, true);
        }
        //vlození centroidu na plátno
        scene->addItem(centroids[i]);
    }
    voronoi();
}

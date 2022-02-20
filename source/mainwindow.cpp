#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include "VoronoiDiagramGenerator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),velikostBodu(10), sirkaPlatna(719-velikostBodu/2), vyskaPlatna(469-velikostBodu/2), pocetBodu(50), pocetCentroidu(5)
{
    ui->setupUi(this);

    scena = new QGraphicsScene(this);
    ui->graphicsView->setScene(scena);
    scena->setSceneRect(0,0,sirkaPlatna,vyskaPlatna);
    vygeneruj(true);
    vygeneruj(false);

    //napojeni signalu z tlacitek na sloty
    connect(ui->solveButton, &QPushButton::clicked, this, &MainWindow::solve);
    connect(ui->pointButton, &QPushButton::clicked, this, &MainWindow::generatePoints);
    connect(ui->centroidButton, &QPushButton::clicked, this, &MainWindow::generateCentroids);
}

MainWindow::~MainWindow()
{
    delete ui;
    this->body.clear();
    this->centroidy.clear();
    this->linky.clear();
    this->hraniceVoronehoBunek.clear();
}

//funkce vygeneruje nahodne body nebo centroidy na platno
void MainWindow::vygeneruj(bool p) //je-li p == true generuje body jinak centroidy
{
    double w,h;
    QRandomGenerator g;

    std::uniform_real_distribution<double> x(0,sirkaPlatna-velikostBodu/2);
    std::uniform_real_distribution<double> y(0,vyskaPlatna-velikostBodu/2);

    unsigned int number = (p == true) ? pocetBodu : pocetCentroidu;

    for (unsigned int i = 0; i < number; i++ )
    {
        w = x(*QRandomGenerator::global());
        h = y(*QRandomGenerator::global());
        myitem * el ;
        if (p)
        {
            el = new myitem(w, h, 10, Qt::darkYellow);
            body.push_back(el);
        }
        else
        {
            el = new myitem(w, h, 10, Qt::red);
            centroidy.push_back(el);
        }
        scena->addItem(el);
    }
}

//smaze body a centroidy ze sceny
void MainWindow::smazBody(bool p)
{
    if (p)
    {
        for (int i = 0; i < body.length(); i++)
        {
            scena->removeItem(body[i]);
        }
        body.clear();
    }
    else
    {
        for (int i = 0; i < centroidy.length(); i++)
        {
            scena->removeItem(centroidy[i]);
        }
        centroidy.clear();
    }
}

//samze linky ze sceny
void MainWindow::smazLinky()
{
    for(int i = 0; i < linky.length(); i++)
        this->scena->removeItem(linky[i]);
    for(int i = 0; i < hraniceVoronehoBunek.length(); i++)
        this->scena->removeItem(hraniceVoronehoBunek[i]);
    linky.clear();
    hraniceVoronehoBunek.clear();
}

//vypocita linky voroneho diagramu
//pouzit fortuneho algoritmus ze stranky: https://web.archive.org/web/20131207065132/http://www.skynet.ie/~sos/mapviewer/voronoi.php
void MainWindow::voronoi()
{
    int c = this->centroidy.length();
    float * x = new float[c];
    float * y = new float[c];
    for (int i = 0; i < c; i++)
    {
        x[i] = (float)centroidy[i]->getX();
        y[i] = (float)centroidy[i]->getY();
    }

    VoronoiDiagramGenerator vdg;
    vdg.generateVoronoi(x,y,c, 0, sirkaPlatna,0,vyskaPlatna,velikostBodu/2);
    vdg.resetIterator();
    float x1,y1,x2,y2;
    while(vdg.getNext(x1,y1,x2,y2))
    {
        x1 = (x1 < 0.01) ? 0 : x1 + velikostBodu/2;
        y1 = (y1 < 0.01) ? 0 : y1 + velikostBodu/2;
        QGraphicsLineItem *line = scena->addLine(QLineF(x1,y1,x2+velikostBodu/2,y2+velikostBodu/2), QPen(Qt::blue, 2));
        hraniceVoronehoBunek.push_back(line);
    }
    delete[] x;
    delete[] y;
}

//predela body z myitem* na bod resp. centroid
void MainWindow::ulozBodyDoVektoru(QVector<Centroid> &c, QVector<Bod> &b)
{
    double * point;
    point = new double[2];
    for (int i = 0; i < body.length(); i++)
    {
        point[0] = body[i]->getX();
        point[1] = body[i]->getY();
        b.push_back(Bod(point, 2));
    }
    for (int i = 0; i < centroidy.length(); i++)
    {
        scena->removeItem(centroidy[i]);
        point[0] = centroidy[i]->getX();
        point[1] = centroidy[i]->getY();
        c.push_back(Centroid(point, 2));
    }
    delete[] point;
}

//vygeneruje nove body (nejprve smaze ty stare a vygeneruje nove)
void MainWindow::generatePoints()
{
    smazBody(true);
    this->pocetBodu = ui->spinBox->value();
    vygeneruj(true);
    smazLinky();
}

//vygeneruje nove centroidy (nejprve smaze ty stare a vygeneruje nove)
void MainWindow::generateCentroids()
{
    smazBody(false);
    this->pocetCentroidu = ui->spinBox_2->value();
    vygeneruj(false);
    smazLinky();
}

void MainWindow::solve()
{
    //odstrani stare linky mezi body a linky voroneho diagramu
    smazLinky();

    //parametry pro inicializaci tridy kmean
    QVector<Centroid> c;
    QVector<Bod> b;
    ulozBodyDoVektoru(c, b);
    centroidy.clear();

    KMean kmin(c, b, pocetCentroidu, pocetBodu, 2);
    kmin.vyres();

    for (unsigned int i = 0; i < pocetCentroidu; i++)
    {
        //nové centroidy
        myitem * el = new myitem(kmin(i,0), kmin(i,1), velikostBodu, Qt::red);
        centroidy.push_back(el);
        //nalezne body v aktuálním shluku
        for(unsigned int j = 0; j < kmin[i].pocet; j++)
        {
            unsigned int bod = kmin[i].bodyVeShluku[j]; // index bodu ve shluku
            QGraphicsLineItem *line = scena->addLine(QLineF(el->getCenterX(),el->getCenterY(),body[bod]->getCenterX(),body[bod]->getCenterY()));
            linky.push_back(line);
            //spojení linky s krajními body (centroidem a bodem)
            this->body[bod]->addLine(line, false);
            this->centroidy[i]->addLine(line, true);
        }
        //vlození centroidu na plátno
        scena->addItem(centroidy[i]);
    }
    voronoi();
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include "VoronoiDiagramGenerator.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),velikostBodu(10), sirkaPlatna(759-velikostBodu/2), vyskaPlatna(469-velikostBodu/2)
    , pocetBodu(50), pocetCentroidu(5)
{
    ui->setupUi(this);
    this->setFixedSize(804, 619);

    //zakaz aktualizovat
    this->povolitAktualizaci = false;
    this->rezimAktualizace = 0;

    //vytvor scenu
    scena = new QGraphicsScene(this);
    ui->graphicsView->setScene(scena);
    scena->setSceneRect(0,0,sirkaPlatna,vyskaPlatna);
    //vygeneruj body a centroidy
    vygeneruj(true);
    vygeneruj(false);

    //napojeni signalu z tlacitek na sloty
    connect(ui->solveButton, &QPushButton::clicked, this, &MainWindow::vyres);
    connect(ui->pointButton, &QPushButton::clicked, this, &MainWindow::vygenerujBody);
    connect(ui->centroidButton, &QPushButton::clicked, this, &MainWindow::vygenerujCentroidy);

    //napojeni signalu z radiobuttonu
    connect(ui->radioButton, &QRadioButton::clicked, this, &MainWindow::nastavAktualizace);
    connect(ui->radioButton_2, &QRadioButton::clicked, this, &MainWindow::nastavAktualizace);
    connect(ui->radioButton_3, &QRadioButton::clicked, this, &MainWindow::nastavAktualizace);

    connect(ui->akceNacist, &QAction::triggered, this, &MainWindow::nactiSoubor);
}

MainWindow::~MainWindow()
{
    delete ui;

    for(int i = 0; i < body.size(); i++)
        delete body.takeAt(i);
    for(int i = 0; i < centroidy.size(); i++)
       delete centroidy.takeAt(i);
    for(int i = 0; i < linky.size(); i++)
       delete linky.takeAt(i);
    for(int i = 0; i < hraniceVoronehoBunek.size(); i++)
        delete hraniceVoronehoBunek.takeAt(i);
    delete scena;

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
            el = new myitem(w, h, velikostBodu, Qt::darkYellow);
            body.push_back(el);
            connect(this->body[i], &myitem::itemReleased, this, [this]{aktualizuj(true);});
            connect(this->body[i], &myitem::itemChanged, this, [this]{aktualizuj(false);});
        }
        else
        {
            el = new myitem(w, h, velikostBodu, Qt::red);
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
        for(myitem * bod : qAsConst(body))
        {
            scena->removeItem(bod);
            delete bod;
        }
        body.clear();
    }
    else
    {
        for(myitem * centroid : qAsConst(centroidy))
        {
            scena->removeItem(centroid);
            delete centroid;
        }
        centroidy.clear();
    }
}

//samze linky ze sceny
void MainWindow::smazLinky()
{
    for(QGraphicsLineItem * linka : qAsConst(linky))
    {
        scena->removeItem(linka);
        delete linka;
    }
    for (QGraphicsLineItem * hranice : qAsConst(hraniceVoronehoBunek))
    {
        scena->removeItem(hranice);
        delete hranice;
    }
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
    for (myitem * bod : qAsConst(body))
    {
        point[0] = bod->getX();
        point[1] = bod->getY();
        b.push_back(Bod(point, 2));
        bod->deleteLines();
    }
    for (myitem * centroid : qAsConst(centroidy))
    {
        scena->removeItem(centroid);
        point[0] = centroid->getX();
        point[1] = centroid->getY();
        c.push_back(Centroid(point, 2));
        centroid->deleteLines();
    }
    //for (int i = 0; i < centroidy.length(); i++)
    //    delete centroidy.takeAt(i);
    for (myitem * centroid : qAsConst(centroidy)) delete centroid;

    delete[] point;
}

//vygeneruje nove body (nejprve smaze ty stare a vygeneruje nove)
void MainWindow::vygenerujBody()
{
    povolitAktualizaci = false; // zakaz aktualizace
    smazBody(true);
    this->pocetBodu = ui->spinBox->value();
    vygeneruj(true);
    smazLinky();
    this->scena->update();
}

//vygeneruje nove centroidy (nejprve smaze ty stare a vygeneruje nove)
void MainWindow::vygenerujCentroidy()
{
    povolitAktualizaci = false; // zakaz aktualizace
    smazBody(false);
    this->pocetCentroidu = ui->spinBox_2->value();
    vygeneruj(false);
    smazLinky();
    this->scena->update();
}

void MainWindow::vyres()
{
    povolitAktualizaci = true; // povol aktualizace

    //parametry pro inicializaci tridy kmean
    QVector<Centroid> c;
    QVector<Bod> b;
    ulozBodyDoVektoru(c, b);
    centroidy.clear();

    //odstrani stare linky mezi body a linky voroneho diagramu
    smazLinky();

    KMean kmin(c, b, 2);
    kmin.vyres();
    for (unsigned int i = 0; i < pocetCentroidu; i++)
    {
        //nové centroidy
        myitem * el = new myitem(kmin(i,0), kmin(i,1), velikostBodu, Qt::red);
        centroidy.push_back(el);
        //nalezne body v aktuálním shluku
        connect(el, &myitem::itemReleased, this, [this]{aktualizuj(true);});
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
    this->scena->update();
}

// slot na aktualizace je volan po emitnuti signalu z itemu
//itemreleased je true pokud byl volan ze signalu itemReleased, byl-li volan ze signalu itemChanged je false
void MainWindow::aktualizuj(bool itemreleased)
{
    if(povolitAktualizaci && rezimAktualizace ) //pokud jsou vypoctene shluky a zaroven je povoleno aktualizovat
    {
        if(rezimAktualizace == 2 || itemreleased)
            vyres();
    }
        return;
}

//slot volan po emitnuti signalu z radiobuttonu
void MainWindow::nastavAktualizace()
{
    if(ui->radioButton->isChecked())
        this->rezimAktualizace = 1;
    else if(ui->radioButton_2->isChecked())
        this->rezimAktualizace = 2;
    else
        this->rezimAktualizace = 0;
    //je-li povoleno aktualizovat -> zaktualizuj
    if(rezimAktualizace && povolitAktualizaci)
        vyres();
}

void MainWindow::nactiSoubor()
{
    QString cestaSouboru;
    QString nazevSouboru = QFileDialog::getOpenFileName(this, "Otevři soubor", cestaSouboru, "Textový soubor (*.txt)");
    QFileInfo i(nazevSouboru);
    if(nazevSouboru.isEmpty())
        return;

    QFile soubor(i.absoluteFilePath());
    if(!soubor.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", soubor.errorString());
    }

    smazBody(true);

    QTextStream in(&soubor);

    unsigned int index = 0;
    while(!in.atEnd())
    {
        QString line = in.readLine();
        line.remove(' ');
        line.remove('(');
        line.remove(')');
        QStringList fields = line.split(",");
        if(fields.size() == 2)
        {
            double x = abs(fields[0].toDouble()); //uprava tak aby se jednalo o kladne cislo
            double y = abs(fields[1].toDouble());
            while(x > sirkaPlatna) x -= sirkaPlatna; //aby bylo v platne
            while(y > vyskaPlatna) y -= vyskaPlatna;
            if(x < this->velikostBodu)  //a aby nebylo moc male
            {
                x *= sirkaPlatna;
                x /= velikostBodu;
            }
            if(y < this->velikostBodu)
            {
                y /= 10;
                y *= vyskaPlatna;
            }
            myitem * el ;
            el = new myitem(x, y , velikostBodu, Qt::darkYellow);
            body.push_back(el);
            connect(this->body[index], &myitem::itemReleased, this, [this]{aktualizuj(true);});
            connect(this->body[index], &myitem::itemChanged, this, [this]{aktualizuj(false);});
            scena->addItem(el);
            index++;
        }
        this->pocetBodu = index;
        ui->spinBox->setValue(index);
    }

    soubor.close();
}

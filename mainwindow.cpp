#include "mainwindow.h"
#include <QRandomGenerator>
#include "VoronoiDiagramGenerator.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>

#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)  : QMainWindow(parent)
    ,velikostBodu(10), pocetBodu(50), pocetCentroidu(5)
{
    setWindowTitle("K-means - Shlukování metodou nejbližších středů");
    scena = new QGraphicsScene(this);
    vytvor_ui();

    //zakaz aktualizovat
    this->povolitAktualizaci = false;
    this->rezimAktualizace = 0;

    //vytvor scenu
    //scena = new QGraphicsScene(this);
    //ui->graphicsView->setScene(scena);
    QRect platno = this->graphicsview->rect();
    sirkaPlatna = platno.width();
    vyskaPlatna = platno.height();
    scena->setSceneRect(platno.x(),platno.y(), platno.width(), platno.height());
    //vygeneruj body a centroidy
    vygeneruj(true);
    vygeneruj(false);

    //napojeni signalu z tlacitek na sloty
    connect(button_vypocti, &QPushButton::clicked, this, &MainWindow::vyres);
    connect(button_vygeneruj_body, &QPushButton::clicked, this, &MainWindow::vygenerujBody);
    connect(button_vygeneruj_centroidy, &QPushButton::clicked, this, &MainWindow::vygenerujCentroidy);

    //napojeni signalu z radiobuttonu
    connect(radiobutton_vypocti_po, &QRadioButton::clicked, this, &MainWindow::nastavAktualizace);
    connect(radiobutton_vypocti_pri, &QRadioButton::clicked, this, &MainWindow::nastavAktualizace);
    connect(radiobutton_nepocti_auto, &QRadioButton::clicked, this, &MainWindow::nastavAktualizace);

    connect(akceNacti, &QAction::triggered, this, &MainWindow::nactiSoubor);
}

MainWindow::~MainWindow()
{
    //delete ui;

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

    std::uniform_real_distribution<double> x(velikostBodu,sirkaPlatna-velikostBodu);
    std::uniform_real_distribution<double> y(velikostBodu,vyskaPlatna-velikostBodu);

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
    for (myitem * centroid : qAsConst(centroidy)) delete centroid;

    delete[] point;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    QRect platno = this->graphicsview->rect();

    this->scena->setSceneRect(platno.x()+10,platno.y()+10, platno.width()-5,platno.height()-15);

    this->sirkaPlatna = platno.width();
    this->vyskaPlatna = platno.height();

    this->scena->update();
}

//vygeneruje nove body (nejprve smaze ty stare a vygeneruje nove)
void MainWindow::vygenerujBody()
{
    povolitAktualizaci = false; // zakaz aktualizace
    smazBody(true);
    this->pocetBodu = this->spinbox_body->value();
    vygeneruj(true);
    smazLinky();
    this->scena->update();
}

//vygeneruje nove centroidy (nejprve smaze ty stare a vygeneruje nove)
void MainWindow::vygenerujCentroidy()
{
    povolitAktualizaci = false; // zakaz aktualizace
    smazBody(false);
    this->pocetCentroidu = this->spinbox_centroidy->value();
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
    if(radiobutton_vypocti_po->isChecked())
        this->rezimAktualizace = 1;
    else if(radiobutton_vypocti_pri->isChecked())
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
        this->spinbox_body->setValue(index);
    }
    soubor.close();
    this->scena->update();
}

void MainWindow::vytvor_ui()
{  
    akceNacti = new QAction("Načti soubor");
    this->menuBar()->addAction(akceNacti);

    QWidget * widget = new QWidget;
    QVBoxLayout * layout = new QVBoxLayout;
    this->graphicsview = new QGraphicsView(this->scena);
    this->graphicsview->resize(769,469);
    this->graphicsview->setMinimumHeight(333);
    this->graphicsview->setMinimumWidth(547);
    layout->addWidget(graphicsview);

    QHBoxLayout * fcni_tlacitka = new QHBoxLayout;
    QHBoxLayout * lo_body = new QHBoxLayout;
    this->label_body = new QLabel("Počet bodů");
    this->spinbox_body = new QSpinBox;
    this->spinbox_body->setMinimum(3);
    this->spinbox_body->setMaximum(200);
    this->spinbox_body->setValue(50);
    this->button_vygeneruj_body = new QPushButton("Vygeneruj Body");
    lo_body->addWidget(label_body);
    lo_body->addWidget(spinbox_body);
    lo_body->addWidget(button_vygeneruj_body);

    QHBoxLayout * lo_centroidy = new QHBoxLayout;
    this->label_centroidy = new QLabel("Počet centrojdů");
    this->spinbox_centroidy = new QSpinBox;
    this->spinbox_centroidy->setMinimum(1);
    this->spinbox_centroidy->setMaximum(100);
    this->spinbox_centroidy->setValue(5);
    this->button_vygeneruj_centroidy = new QPushButton("Vygeneruj centrojdy");

    this->button_vypocti = new QPushButton("Vypočítej");
    lo_centroidy->addWidget(label_centroidy);
    lo_centroidy->addWidget(spinbox_centroidy);
    lo_centroidy->addWidget(button_vygeneruj_centroidy);

    fcni_tlacitka->addLayout(lo_body);
    fcni_tlacitka->addLayout(lo_centroidy);
    fcni_tlacitka->addStretch();
    fcni_tlacitka->addWidget(button_vypocti);

    QHBoxLayout * radio_buttony = new QHBoxLayout;
    this->radiobutton_vypocti_po = new QRadioButton("Vypočti po přesunutí");
    this->radiobutton_vypocti_pri = new QRadioButton("Vypočti při přesunování");
    this->radiobutton_nepocti_auto = new QRadioButton("Nepočítat automaticky");
    radio_buttony->addWidget(radiobutton_vypocti_po);
    radio_buttony->addWidget(radiobutton_vypocti_pri);
    radio_buttony->addWidget(radiobutton_nepocti_auto);
    radio_buttony->addStretch();
    this->radiobutton_nepocti_auto->setChecked(true);

    layout->addLayout(fcni_tlacitka);
    layout->addLayout(radio_buttony);

    widget->setLayout(layout);
    this->setCentralWidget(widget);
}

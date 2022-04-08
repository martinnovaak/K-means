# K-Means
Tento projekt obsahuje algoritmus Shlukování metodou nejbližších středů neboli K-Means (soubory kmeans.h, kmeans.cpp, bod.h a bod.cpp). Dále je k projektu udělána grafická aplikace pro znázornění výsledků ve 2D (soubory mainwindow.h, mainwindow.cpp, mainwindow.ui, myitem.h, myitem.cpp). V projektu je dále využit algoritmus pro výpočet hranic Voroneho buněk. Tento algoritmus jsem neprogramoval, ale použil již vytvořenou třídu z [Voronoi Resources](https://web.archive.org/web/20131207065132/http://www.skynet.ie/~sos/mapviewer/voronoi.php) (soubory VoronoiDiagramGenerator.h a VoronoiDiagramGenerator.cpp). Program je napsán v programovacím jazyce C++ a pro grafické rozhraní byla využita knihovna Qt. 
## Složka program
Ve složce program se nachází spustitelný .exe program. Pro jeho spuštění je třeba mít ve stejné složce všechny .dll knihovny, které se v daném složce nachází.
## Složka source
Ve složce source se nachází veškeré zdrojové soubory k programu a testovací soubor testdata.txt.
## Třída pro výpočet k-průměrů (k-means) 
Pro samotný funkční algoritmus jsou potřeba třídy kmeans.h a bod.h s příslušnými kmeans.cpp a bod.cpp. Třída je napsána obecně pro libovolnou dimenzi.
#### Způsob použití algoritmu
Instance třídy se vytvoří následovně: `KMean instance(vektor_centroidů, vektor_bodů, dimenze);`
Poté se úloha dá vyřešit pomocí metody: `instance.vyres();`
A k výsledným centroidům se dá přistupovat pomocí přetížené metody pro indexování: `instance[index];`   kde index je číslo z množiny {0,1,...,počet_centroidů - 1}
Pomocí další přetížené metody se dá přistupovat k indexům bodů, které jsou ve shluku s centroidem `instance[index]` pomocí: `instance(index, i)`; kde i je číslo z množiny {0,1,...,`instance[index].pocet;`}

Pro vytvoření instance třídy KMean je potřeba mít vektory centroidů a bodů. Instance jak Bodu tak Centroidu se dá vytvořit pomocí pole délky n reprezentující souřadnice následovně: `Bod bod(souradnice, n);`. Vytvoření instance Centroidu je stejné.

## Grafické uživatelské rozhraní
Program se skládá z okna o velikosti 759x469 pixelů. Aplikace se skládá z plátna (QGraphicsView) o velikosti 759 x 469 pixelů. Pod plátnem se nachází spinboxy, na kterých se dá nastavit námi požadovaný počet centroidů a bodů. Body se dají dále vygenerovat po stisknutí příslušného tlačítka. Body i Centroidy si pomocí myši můžeme přesunout na námi chtěnou pozici. Body při přesunu mění barvu na magentovou po jejich puštění se jim barva vrátí na původní.
Celá úloha Shlukování metodou nejbližšícch středů se dá vypočítat a nakreslit stisknutím tlačítka `Vypočti`. Vykreslí se shluky, všechny body budou černou linkou jsou se svým centroidem. Modrými linkami jsou znázorněny hranice Voroného buněk. 

Na nejnižším řádku aplikace se nacházejí 3 radiobuttony. Na nich si může uživatel navolit zda-li chce volat výpočet metody při puštění bodu či jeho přesouvání. Příslušné linky daného bodu při jeho přesouvání mění barvu na červenou. Po jejich puštění zpět na černou.

V levém horním rohu aplikace se nachází menubar s akcí pro načtení bodů ze souboru formátu .txt. Data by měly být zapsány podobně jako v souboru testdata.txt.

## Spuštění
## Spuštění na Windows
Ve složce program se nachází spustitelný .exe soubor.
### Spuštění na linuxových distribucích založených na debianu
#### Potřebné balíčky
`sudo apt install git` <br/>
`sudo apt install g++` <br/>
`sudo apt install qt5-default` <br/>
`sudo apt install make` <br/>

#### Naklonování projektu
`git clone https://github.com/martinnovaak/K-means` <br/>
`cd K-means` <br/>
`rm -r program`  <br/>
`cd source` <br/>

#### Spuštění
`qmake Plot.pro` <br/>
`make` <br/>
`./Plot` <br/>

## Oprava chyb
### Defaultní konstruktor
Do tříd Centroid a Bod byl přidán prázdný defaultní konstruktor pro kompatibilitu se staršími verzemi Qt. Před verzí Qt 5.13 vyžadovaly třídy kontejnerů v Qt po objektech defaultní konstruktory viz [Qt dokumentace](https://doc.qt.io/archives/qt-5.12/containers.html#the-container-classes). 

### Bílé "šmouhy"
Ve verzi Qt6 vznikali při vykreslování bílé horizontální šmouhy, kde se nevykreslovaly body ani Voroneho linky. Vyřešeno aktualizací plátna po každém vykreslení.

# K-Means
Tento projekt obsahuje algoritmus Shlukování metodou nejbližších středů neboli K-Means (soubory kmeans.h, kmeans.cpp, bod.h, bod.cpp, centroid.h a centroid.cpp). Dále je k projektu udělána grafická aplikace pro znázornění výsledků ve 2D (soubory mainwindow.h, mainwindow.cpp, myitem.h, myitem.cpp, dialog.h, dialog.cpp). V projektu je dále využit algoritmus pro výpočet hranic Voroneho buněk. Tento algoritmus jsem neprogramoval, ale použil již vytvořenou třídu z [Voronoi Resources](https://web.archive.org/web/20131207065132/http://www.skynet.ie/~sos/mapviewer/voronoi.php) (soubory VoronoiDiagramGenerator.h a VoronoiDiagramGenerator.cpp). Program je napsán v programovacím jazyce C++ a pro grafické rozhraní byla využita knihovna Qt. 
## Třída pro výpočet k-průměrů (k-means) 
Pro samotný funkční algoritmus jsou potřeba třídy kmeans.h, bod.h a centroid.h s příslušnými kmeans.cpp, bod.cpp a centroid.cpp. Třída je napsána obecně pro libovolnou dimenzi.
#### Způsob použití algoritmu
Instance třídy se vytvoří následovně: `KMean instance(vektor_centroidů, vektor_bodů, dimenze);`
Poté se úloha dá vyřešit pomocí metody: `instance.vyres();`
A k výsledným centroidům se dá přistupovat pomocí přetížené metody pro indexování: `instance[index];`   kde index je číslo z množiny {0,1,...,počet_centroidů - 1}
Pomocí další přetížené metody se dá přistupovat k indexům bodů, které jsou ve shluku s centroidem `instance[index]` pomocí: `instance(index, i)`; kde i je číslo z množiny {0,1,...,`instance[index].pocet;`}

Pro vytvoření instance třídy KMean je potřeba mít vektory centroidů a bodů. Instance jak Bodu tak Centroidu se dá vytvořit pomocí pole délky n reprezentující souřadnice následovně: `Bod bod(souradnice, n);`. Vytvoření instance Centroidu je stejné.

## Grafické uživatelské rozhraní
Aplikace se skládá z plátna (QGraphicsView) o velikosti 759 x 469 pixelů. Velikost plátna se přizpůsobuje velikosti okna programu. Je ale potřeba po každým změně velikosti ("resiznutí") vygenerovat nové body a centroidy. 
Pod plátnem se nachází spinboxy pro nastavení námi požadovaný počet centroidů a bodů. Body se dají dále vygenerovat po stisknutí příslušného tlačítka. Body i Centroidy si pomocí myši můžeme přesunout na námi chtěnou pozici. Body při přesunu mění barvu na magentovou a po jejich následném puštění se jim barva vrátí na původní.
Celá úloha Shlukování metodou nejbližšícch středů se dá vypočítat a nakreslit stisknutím tlačítka `Vypočti`. Vykreslí se shluky, všechny body budou spojeny černou linkou se svým centroidem. Modrými linkami jsou znázorněny hranice Voroného buněk. 

Na nejnižším řádku aplikace se nacházejí 3 radiobuttony. Na nich si může uživatel nastavit, zda-li chce volat výpočet metody při puštění bodu či jeho přesouvání. Příslušné linky daného bodu při jeho přesouvání mění barvu na červenou. Po jejich puštění zpět na černou.

V levém horním rohu aplikace se nachází menubar s akcí pro načtení bodů ze souboru formátu .txt. Data by měly být zapsány podobně jako v souboru testdata.txt.

## Spuštění
## Spuštění na Windows
<strike> Ve složce program se nachází spustitelný .exe soubor. </strike>
#### MinGW shell:
`git clone https://github.com/martinnovaak/K-means.git` <br/>
`cd K-means` <br/>

`qmake Plot.pro` <br/>
`mingw32-make` <br/>

Více viz [Qt dokumentace](https://wiki.qt.io/Build_Standalone_Qt_Application_for_Windows#Using_MinGW_2) 

Ve složce release se nachází spustitelný .exe soubor

### Spuštění na linuxových distribucích založených na debianu
#### Potřebné balíčky
`sudo apt install git` <br/>
`sudo apt install g++` <br/>
`sudo apt install qt5-default` <br/>
`sudo apt install make` <br/>

#### Naklonování projektu
`git clone https://github.com/martinnovaak/K-means.git` <br/>
`cd K-means` <br/>

#### Spuštění
`qmake Plot.pro` <br/>
`make` <br/>
`./Plot` <br/>

## Změny oproti původní verzi
### Zrušení Mainwindow.ui
Aplikace nemá již pevně vytvořený UI. Veškeré grafické nastavení okna je děláno v Mainwindow.h a Mainwindow.cpp. 

### Resizování aplikace
Nově je umožněno měnit velikost aplikace. Po změně velikosti plátna je potřeba vygenerovat body a centroidy znovu.

### Nastavení velikosti centroidů a bodů
Ve verzích Qt5 a Qt6 se body vykreslovaly různou velikostí. Přidal jsem do programu dialog pro možnost nastavení vlastní velikosti. Pro použití nové velikosti bodů je potřeba body znovu vygenerovat.

### Úprava KMeans algoritmu
Třídy pro výpočet shluků byly upraveny tak, aby nepoužívaly třídy frameworku Qt. Dají se nyní libovolně použít samostatně bez nutnosti užití Qt knihoven.

## Oprava chyb
### Defaultní konstruktor
Do tříd Centroid a Bod byl přidán prázdný defaultní konstruktor pro kompatibilitu se staršími verzemi Qt. Před verzí Qt 5.13 vyžadovaly třídy kontejnerů v Qt po objektech defaultní konstruktory viz [Qt dokumentace](https://doc.qt.io/archives/qt-5.12/containers.html#the-container-classes). 

### Bílé "šmouhy"
Ve verzi Qt6 vznikali při vykreslování bílé horizontální šmouhy, kde se nevykreslovaly body ani Voroneho linky. Vyřešeno aktualizací plátna po každém vykreslení.

### Warningy ve třídě VoronoiDiagramGenerator
Bylo opraveno absolutní minimum pro to, aby se ve třídě nevyskytovaly žádné (v Qt6 se žádné nevyskytují) upozornění. (Ve třídě se stále nachází podle mě nevyužívané funkce.)

### Načítání dat ze souboru
Po načtení dat ze souboru již nezůstavají vykreslené staré Voroneho linky.


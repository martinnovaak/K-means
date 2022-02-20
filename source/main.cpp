#include "mainwindow.h"

#include <QApplication>
#include "kmean.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowIcon(QIcon(":/obrazek.PNG"));
    return a.exec();
}

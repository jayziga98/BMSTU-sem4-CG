#include "mainwindow.h"
#include <qfile.h>
#include "staskpool.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    STaskPool pool;
    MainWindow w(&pool);

    w.show();
    return a.exec();
}

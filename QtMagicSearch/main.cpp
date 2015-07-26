#include "mainwindow.h"
#include <urls.h>
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    int ret = a.exec();

    QDir dir(imageDir);
    if (dir.exists())
    {
        dir.setNameFilters(QStringList() << "*.*");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList())
        {
            dir.remove(dirFile);
        }
    }

    return ret;
}

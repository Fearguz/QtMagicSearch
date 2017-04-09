#include "mainwindow.h"
#include "controller.h"
#include "constants.h"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller controller;
    MainWindow w {controller};
    w.show();

    // Main loop
    int ret = a.exec();

    QDir dir(DirConstants::ImageDir);
    if (dir.exists())
    {
        dir.setNameFilters(QStringList() << "*.*");
        dir.setFilter(QDir::Files);
        for (auto&& file : dir.entryList())
        {
            dir.remove(file);
        }
    }
    return ret;
}

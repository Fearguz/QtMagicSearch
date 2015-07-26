#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "card.h"

namespace Ui
{
    class MainWindow;
}

class Controller;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        void showImage(QString &path);

        void fillList(QList<Card*> source);
        void clearList();

        void startProgressBar();
        void updateProgressBar(int percent);
        void stopProgressBar();

        void fillCardInfoTab(Card *card);

    private:
        Ui::MainWindow *ui;
        Controller *controller;

    private slots:
        void searchButtonClicked(bool);
        void listItemDoubleClicked(QModelIndex idx);
};

#endif // MAINWINDOW_H

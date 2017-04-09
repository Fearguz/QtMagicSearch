#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

#include "controller.h"
#include "card.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Controller& controller, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void listItemDoubleClicked(QModelIndex idx);
    void searchButtonClicked(bool);

    void showImage(const QString& path);
    void fillList(QList<Card*> source);
    void onProgressUpdate(int8_t value);

private:
    Controller& m_controller;
    Ui::MainWindow* m_ui;

    void clearList();

    void fillCardInfoTab(const Card& card);

    void startProgressBar() noexcept;
    void updateProgressBar(int percent) noexcept;
    void stopProgressBar() noexcept;
};

#endif // MAINWINDOW_H

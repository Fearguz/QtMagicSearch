#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller.h"
#include "urls.h"

#include <qstandarditemmodel.h>
#include <QVariant>

MainWindow::MainWindow(Controller* ctrl, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), controller(ctrl)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);

    connect(ui->searchButton, SIGNAL(clicked(bool)), SLOT(searchButtonClicked(bool)));
    connect(ui->cardList, SIGNAL(doubleClicked(QModelIndex)), SLOT(listItemDoubleClicked(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::listItemDoubleClicked(QModelIndex idx)
{
    QStandardItemModel *model = static_cast<QStandardItemModel*>(ui->cardList->model());
    if (model != NULL)
    {
        QStandardItem* item = model->itemFromIndex(idx);
        Card *card = item->data(Qt::UserRole + 1).value<Card*>();

        QString filename = QString::number(card->Id) + ".jpg";
        QString path = imageDir + filename;
        QFileInfo checkFile(path);
        if (checkFile.exists())
        {
            showImage(path);
        }
        else
        {
            QString imageUrl = highResImageUrl + "/" + filename;
            controller->download(imageUrl);
        }

        ui->imageDisplay->setToolTip(card->asString());
        fillCardInfoTab(card);
    }
}

void MainWindow::searchButtonClicked(bool)
{
    bool isComplexSearch =  (!ui->searchText->text().isEmpty() || (ui->blackCheckBox->isChecked())
                            || (ui->blueCheckBox->isChecked()) || (ui->redCheckBox->isChecked())
                            || (ui->greenCheckBox->isChecked()) || (ui->whiteCheckBox->isChecked()));

    bool isSimpleSearch = (!ui->searchName->text().isEmpty()) && !isComplexSearch;

    QString url;
    if (!isComplexSearch && !isSimpleSearch)
    {
        url = getAllCardsUrl;
    }
    else if (isComplexSearch)
    {
        url = mtgDbSearch;
        bool first = true;

        url += "/?q=";
        if (!ui->searchText->text().isEmpty())
        {
            url += "description m " + ui->searchText->text();
            first = false;
        }
        if (ui->blackCheckBox->isChecked())
        {
            if (!first)
            {
                url += " and ";
            }
            url += "color m black";
            first = false;
        }
        if (ui->blueCheckBox->isChecked())
        {
            if (!first)
            {
                url += " and ";
            }
            url += "color m blue";
            first = false;
        }
        if (ui->redCheckBox->isChecked())
        {
            if (!first)
            {
                url += " and ";
            }
            url += "color m red";
            first = false;
        }
        if (ui->greenCheckBox->isChecked())
        {
            if (!first)
            {
                url += " and ";
            }
            url += "color m green";
            first = false;
        }
        if (ui->whiteCheckBox->isChecked())
        {
            if (!first)
            {
                url += " and ";
            }
            url += "color m white";
            first = false;
        }
    }
    else if (isSimpleSearch)
    {
        url = mtgDbSearch;
        url += ui->searchName->text();
    }

    clearList();
    controller->download(url);
}

void MainWindow::showImage(QString &path)
{
    QPixmap image;
    image.load(path, "JPG");

    QGraphicsScene *scene = new QGraphicsScene;
    ui->imageDisplay->setScene(scene);
    scene->addPixmap(image.scaled(QSize(ui->imageDisplay->width() * 0.9, ui->imageDisplay->height() * 0.9)));
}

void MainWindow::fillList(QList<Card*> source)
{
    QStandardItemModel *model = new QStandardItemModel(source.size(), 1, this);

    size_t row = 0;
    foreach (Card *card, source)
    {
        QStandardItem *item = new QStandardItem(card->Name);
        item->setData(QVariant::fromValue(card), Qt::UserRole + 1);

        model->setItem(row++, item);
    }

    ui->cardList->setModel(model);
}

void MainWindow::clearList()
{
    QStandardItemModel *model = static_cast<QStandardItemModel*>(ui->cardList->model());
    if (model != NULL)
    {
        int size = model->rowCount();
        for (int idx = 0; idx < size; ++idx)
        {
            QStandardItem* item = model->item(idx);
            Card *card = item->data(Qt::UserRole + 1).value<Card*>();
            delete card;
        }

        model->clear();
        delete model;
    }
}

void MainWindow::startProgressBar()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);
}

void MainWindow::updateProgressBar(int percent)
{
    ui->progressBar->setValue(percent);
    ui->progressBar->repaint();
}

void MainWindow::stopProgressBar()
{
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
}

void MainWindow::fillCardInfoTab(Card *card)
{
    ui->infoName->setText(card->Name);

    QString colors;
    int size = card->Colors.size();
    for (int idx = 0; idx < size; ++idx)
    {
        colors += card->Colors.at(idx);
        if ((idx + 1) < size)
        {
            colors += ", ";
        }
    }
    ui->infoColors->setText(colors);

    ui->infoType->setText(card->Type);
    ui->infoSubtype->setText(card->SubType);
    ui->infoDescription->setText(card->Description);
    ui->infoPower->setText(QString::number(card->Power));
    ui->infoToughness->setText(QString::number(card->Toughness));
}

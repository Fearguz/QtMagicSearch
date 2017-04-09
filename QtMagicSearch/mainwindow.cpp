#include "ui_mainwindow.h"
#include <qstandarditemmodel.h>
#include "mainwindow.h"
#include "controller.h"
#include "constants.h"

MainWindow::MainWindow(Controller& controller, QWidget *parent) : QMainWindow(parent),
                                                                  m_controller(controller), m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_ui->progressBar->setVisible(false);
    connect(m_ui->searchButton, SIGNAL(clicked(bool)), this, SLOT(searchButtonClicked(bool)));
    connect(m_ui->cardList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(listItemDoubleClicked(QModelIndex)));

    connect(&m_controller, SIGNAL(imageDownloaded(QString)), this, SLOT(showImage(const QString&)));
    connect(&m_controller, SIGNAL(cardsDownloaded(QList<Card*>)), this, SLOT(fillList(QList<Card*>)));
    connect(&m_controller, SIGNAL(progressChanged(int8_t)), this, SLOT(onProgressUpdate(int8_t)));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::listItemDoubleClicked(QModelIndex idx)
{
    QStandardItemModel *model = static_cast<QStandardItemModel*>(m_ui->cardList->model());
    if (model != nullptr)
    {
        QStandardItem* item = model->itemFromIndex(idx);
        Card& card = *(item->data(Qt::UserRole + 1).value<Card*>());

        QString filename = QString::number(card.Id) + ".jpg";
        QString filepath = DirConstants::ImageDir + filename;
        QFileInfo file{filepath};
        if (file.exists())
        {
            showImage(filepath);
        }
        else
        {
            QString imageUrl = QString(UrlConstants::HighResImageUrl) + "/" + filename;
            m_controller.download(imageUrl);
        }

        m_ui->imageDisplay->setToolTip(card.toString());
        fillCardInfoTab(card);
    }
}

void MainWindow::searchButtonClicked(bool)
{
    bool isComplexSearch =  (!m_ui->searchText->text().isEmpty() || (m_ui->blackCheckBox->isChecked())
                            || (m_ui->blueCheckBox->isChecked()) || (m_ui->redCheckBox->isChecked())
                            || (m_ui->greenCheckBox->isChecked()) || (m_ui->whiteCheckBox->isChecked()));

    bool isSimpleSearch = (!m_ui->searchName->text().isEmpty()) && !isComplexSearch;

    QString url;
    if (!isComplexSearch && !isSimpleSearch)
    {
        url = QString(UrlConstants::GetAllCardsUrl);
    }
    else if (isComplexSearch)
    {
        url = QString(UrlConstants::MtgDbSearch);
        url += "/?q=";

        bool isFirstQuery = true;
        if (!m_ui->searchText->text().isEmpty())
        {
            url += "description m " + m_ui->searchText->text();
            isFirstQuery = false;
        }
        if (m_ui->blackCheckBox->isChecked())
        {
            if (!isFirstQuery)
            {
                url += " and ";
            }
            url += "color m black";
            isFirstQuery = false;
        }
        if (m_ui->blueCheckBox->isChecked())
        {
            if (!isFirstQuery)
            {
                url += " and ";
            }
            url += "color m blue";
            isFirstQuery = false;
        }
        if (m_ui->redCheckBox->isChecked())
        {
            if (!isFirstQuery)
            {
                url += " and ";
            }
            url += "color m red";
            isFirstQuery = false;
        }
        if (m_ui->greenCheckBox->isChecked())
        {
            if (!isFirstQuery)
            {
                url += " and ";
            }
            url += "color m green";
            isFirstQuery = false;
        }
        if (m_ui->whiteCheckBox->isChecked())
        {
            if (!isFirstQuery)
            {
                url += " and ";
            }
            url += "color m white";
            isFirstQuery = false;
        }
    }
    else if (isSimpleSearch)
    {
        url = QString(UrlConstants::MtgDbSearch);
        url += m_ui->searchName->text();
    }

    clearList();
    m_controller.download(url);
}

void MainWindow::clearList()
{
    QStandardItemModel* model = static_cast<QStandardItemModel*>(m_ui->cardList->model());
    if (model != nullptr)
    {
        int32_t size = model->rowCount();
        for (int32_t idx = 0; idx < size; ++idx)
        {
            QStandardItem* item = model->item(idx);
            Card *card = item->data(Qt::UserRole + 1).value<Card*>();
            delete card;
        }

        model->clear();
        delete model;
    }
}

void MainWindow::showImage(const QString& path)
{
    QPixmap image;
    image.load(path, "JPG");

    QGraphicsScene *scene = new QGraphicsScene;
    m_ui->imageDisplay->setScene(scene);
    scene->addPixmap(image.scaled(QSize(m_ui->imageDisplay->width() * 0.9, m_ui->imageDisplay->height() * 0.9)));
}

void MainWindow::fillList(QList<Card*> source)
{
    QStandardItemModel *model = new QStandardItemModel(source.size(), 1, this);

    size_t row = 0;
    for (auto&& card : source)
    {
        QStandardItem *item = new QStandardItem(card->Name);
        item->setData(QVariant::fromValue(card), Qt::UserRole + 1);
        model->setItem(row, item);
        ++row;
    }

    m_ui->cardList->setModel(model);
}

void MainWindow::onProgressUpdate(int8_t value)
{
    if (value == -1)
    {
        stopProgressBar();
    }
    else if (value == 0)
    {
        startProgressBar();
    }
    else if (value > 0)
    {
        updateProgressBar(value);
    }
}

void MainWindow::startProgressBar() noexcept
{
    m_ui->progressBar->setValue(0);
    m_ui->progressBar->setVisible(true);
}

void MainWindow::updateProgressBar(int percent) noexcept
{
    m_ui->progressBar->setValue(percent);
    m_ui->progressBar->repaint();
}

void MainWindow::stopProgressBar() noexcept
{
    m_ui->progressBar->setVisible(false);
    m_ui->progressBar->setValue(0);
}

void MainWindow::fillCardInfoTab(const Card& card)
{
    QString colors;
    for (auto&& color : card.Colors)
    {
        colors.append(color);
    }

    m_ui->infoName->setText(card.Name);
    m_ui->infoColors->setText(colors);
    m_ui->infoType->setText(card.Type);
    m_ui->infoSubtype->setText(card.SubType);
    m_ui->infoDescription->setText(card.Description);
    m_ui->infoPower->setText(QString::number(card.Power));
    m_ui->infoToughness->setText(QString::number(card.Toughness));
}

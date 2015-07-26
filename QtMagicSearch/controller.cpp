#include "controller.h"
#include "mainwindow.h"
#include "urls.h"

#include <QtNetwork>

Controller::Controller(MainWindow *win) : window(win) { }

void Controller::download(const QString &url)
{
    QUrl httpUrl = url;
    if (!httpUrl.isValid())
    {
        return;
    }

    httpCon.abortAll();

    if (!url.startsWith(lowResImageUrl) && !url.startsWith(highResImageUrl))
    {
        window->startProgressBar();
    }
    httpCon.download(httpUrl, this, SLOT(updateDownloadProgress(qint64,qint64)), SLOT(cardDownloadFinished(QNetworkReply*)));
}

void Controller::updateDownloadProgress(qint64 bytesRead, qint64 bytesTotal)
{
    int percent = ((double)bytesRead / bytesTotal) * 100;
    window->updateProgressBar(percent);
}

void Controller::cardDownloadFinished(QNetworkReply *reply)
{
    QString url = reply->url().toString();
    if (url.startsWith(lowResImageUrl) || url.startsWith(highResImageUrl))
    {
        QByteArray image = reply->readAll();

        QDir directory;
        if (!directory.exists(imageDir))
        {
            directory.mkdir(imageDir);
        }

        QString filename = url.split("/").back();
        QString fullFilePath = imageDir + filename;
        QFile file(fullFilePath);
        if (!file.exists())
        {
            if (!file.open(QIODevice::WriteOnly))
            {
                window->stopProgressBar();
                reply->deleteLater();
                return;
            }

            QPixmap pixmap;
            pixmap.loadFromData(image);
            pixmap.save(&file, "JPG");
        }
        file.close();
        window->showImage(fullFilePath);
    }
    else
    {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull())
        {
            window->stopProgressBar();
            reply->deleteLater();
            return;
        }

        if (doc.isObject())
        {
            QList<Card*> list;
            list.append(parseObject(doc.object()));

            window->fillList(list);
        }
        else if (doc.isArray())
        {
            QList<Card*> list = parseArray(doc.array());
            window->fillList(list);
        }
    }

    window->stopProgressBar();
    reply->deleteLater();
    httpCon.removeHandle(reply);
}

Card* Controller::parseObject(QJsonObject object)
{
    Card *card = new Card;
    card->Id = object.value("id").toInt();
    card->SetNumber = object.value("setNumber").toInt();
    card->Name = object.value("name").toString();
    card->Description = object.value("description").toString();
    card->Colors = parseArrayOfStrings(object.value("colors").toArray());
    card->ManaCost = object.value("manaCost").toString();
    card->Type = object.value("type").toString();
    card->SubType = object.value("subType").toString();
    card->Power = object.value("power").toInt();
    card->Toughness = object.value("toughness").toInt();

    return card;
}

QList<Card*> Controller::parseArray(QJsonArray array)
{
    QList<Card*> ret;
    foreach (QJsonValue value, array)
    {
        if (value.isObject())
        {
            QJsonObject object = value.toObject();
            ret.append(parseObject(object));
        }
    }

    return ret;
}

QStringList Controller::parseArrayOfStrings(QJsonArray array)
{
    QStringList ret;
    foreach (QJsonValue value, array)
    {
        ret << value.toString();
    }

    return ret;
}


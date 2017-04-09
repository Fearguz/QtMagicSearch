#include <stdexcept>
#include <QtNetwork/QNetworkReply>
#include <QPixmap>
#include "controller.h"
#include "constants.h"

void Controller::download(const QUrl& httpUrl)
{
    if (!httpUrl.isValid())
    {
        throw std::runtime_error("The url is invalid");
    }

    QString urlStr = httpUrl.toString();
    if (!urlStr.startsWith(UrlConstants::LowResImageUrl) && !urlStr.startsWith(UrlConstants::HighResImageUrl))
    {
        emit progressChanged(0);
    }

    m_connector.abortAll();
    m_connector.download(httpUrl, this, SLOT(updateDownloadProgress(qint64,qint64)), SLOT(cardDownloadFinished(QNetworkReply*)));
}

void Controller::updateDownloadProgress(qint64 bytesRead, qint64 bytesTotal)
{
    int8_t percent = static_cast<int8_t>((double)bytesRead / bytesTotal) * 100;
    emit progressChanged(percent);
}

void Controller::cardDownloadFinished(QNetworkReply *reply)
{
    QString url = reply->url().toString();
    if (url.startsWith(UrlConstants::LowResImageUrl) || url.startsWith(UrlConstants::HighResImageUrl))
    {
        QDir imageDir {DirConstants::ImageDir};
        if (!imageDir.exists())
        {
            imageDir.mkdir(DirConstants::ImageDir);
        }
        QString filename = url.split('/').back();
        QString filepath = DirConstants::ImageDir + QDir::separator() + filename;
        QFile file {filepath};
        if (!file.exists())
        {
            if (file.open(QIODevice::WriteOnly))
            {
                QByteArray imageData = reply->readAll();
                QPixmap pixmap;
                pixmap.loadFromData(imageData);
                pixmap.save(&file, "JPG");
            }
        }
        emit imageDownloaded(filepath);
    }
    else
    {
        QByteArray jsonData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if (jsonDoc.isObject())
        {
            QList<Card*> cards;
            cards.append(parseObject(jsonDoc.object()));
            emit cardsDownloaded(cards);
        }
        else if (jsonDoc.isArray())
        {
            QList<Card*> cards = parseArray(jsonDoc.array());
            emit cardsDownloaded(cards);
        }
    }

    reply->deleteLater();
    m_connector.removeHandle(reply);
    emit progressChanged(-1);
}

Card* Controller::parseObject(QJsonObject object)
{
    Card* card{new Card {
        object.value("id").toInt(),
        object.value("setNumber").toInt(),
        object.value("name").toString(),
        object.value("description").toString(),
        parseArrayOfStrings(object.value("colors").toArray()),
        object.value("manaCost").toString(),
        object.value("type").toString(),
        object.value("subType").toString(),
        static_cast<int8_t>(object.value("power").toInt()),
        static_cast<int8_t>(object.value("toughness").toInt())
    } };
    return card;
}

QList<Card*> Controller::parseArray(QJsonArray array) noexcept
{
    QList<Card*> cards;
    for (auto&& elem : array)
    {
        if (elem.isObject())
        {
            QJsonObject object = elem.toObject();
            cards << parseObject(object);
        }
    }
    return cards;
}

QStringList Controller::parseArrayOfStrings(QJsonArray array) noexcept
{
    QStringList strings;
    for (auto&& elem : array)
    {
        strings << elem.toString();
    }
    return strings;
}


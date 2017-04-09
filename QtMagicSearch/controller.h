#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include <QUrl>
#include "httpconnector.h"
#include "card.h"

class QNetworkReply; // Forward declaration

class Controller : public QObject
{
    Q_OBJECT

public:
    void download(const QUrl& httpUrl);

signals:
    void imageDownloaded(QString filepath);
    void cardsDownloaded(QList<Card*> cards);
    void progressChanged(int8_t progress);

private slots:
    void updateDownloadProgress(qint64 bytesRead, qint64 bytesTotal);
    void cardDownloadFinished(QNetworkReply *reply);

private:
    HttpConnector m_connector;

    static Card* parseObject(QJsonObject object);
    static QList<Card*> parseArray(QJsonArray array) noexcept;
    static QStringList parseArrayOfStrings(QJsonArray array) noexcept;
};

#endif // CONTROLLER_H

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "httpconnector.h"
#include "card.h"

class QNetworkReply;
class MainWindow;

class Controller : public QObject
{
    Q_OBJECT

private:
    MainWindow *window;
    HttpConnector httpCon;

public:
    Controller(MainWindow *win);

    void download(const QString& url);

private:
    Card* parseObject(QJsonObject object);
    QList<Card*> parseArray(QJsonArray array);
    QStringList parseArrayOfStrings(QJsonArray array);

private slots:
    void updateDownloadProgress(qint64 bytesRead, qint64 bytesTotal);
    void cardDownloadFinished(QNetworkReply *reply);
};

#endif // CONTROLLER_H

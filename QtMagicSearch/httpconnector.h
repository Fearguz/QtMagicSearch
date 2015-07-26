#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QtNetwork>

class HttpConnector : public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager manager;
    QList<QNetworkReply*> handles;

public:
    HttpConnector() = default;
    ~HttpConnector();

    QNetworkReply* download(const QUrl &url, QObject *user_class, const char* downloadProgressSlot, const char* user_slot);

    void abortAll();
    void removeHandle(QNetworkReply *handle);
};

#endif // DOWNLOADMANAGER_H

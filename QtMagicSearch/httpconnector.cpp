#include "httpconnector.h"

HttpConnector::~HttpConnector()
{
    abortAll();
}

QNetworkReply* HttpConnector::download(const QUrl &url, QObject* user_class, const char* downloadProgressSlot, const char* downloadFinishedSlot)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), user_class, downloadProgressSlot);
    connect(&manager, SIGNAL(finished(QNetworkReply*)), user_class, downloadFinishedSlot);
    handles.append(reply);
    return reply;
}

void HttpConnector::abortAll()
{
    foreach (QNetworkReply *reply, handles)
    {
        if (reply->isOpen())
        {
            reply->close();
            reply->abort();
            reply->deleteLater();
        }
    }
    handles.clear();
}

void HttpConnector::removeHandle(QNetworkReply *handle)
{
    if (handle->isOpen())
    {
        handle->close();
        handle->abort();
        handle->deleteLater();
    }
    handles.removeOne(handle);
}

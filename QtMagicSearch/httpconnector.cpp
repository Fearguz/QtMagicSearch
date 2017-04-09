#include "httpconnector.h"

HttpConnector::~HttpConnector()
{
    abortAll();
}

QNetworkReply* HttpConnector::download(const QUrl &url, QObject* userClass, const char* downloadProgressSlot, const char* downloadFinishedSlot)
{
    QNetworkRequest request{url};
    QNetworkReply* reply = m_manager.get(request);
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), userClass, downloadProgressSlot);
    connect(&m_manager, SIGNAL(finished(QNetworkReply*)), userClass, downloadFinishedSlot);

    m_handles.append(reply);
    return reply;
}

void HttpConnector::abortAll() noexcept
{
    for (auto&& handle : m_handles)
    {
        handle->abort();
        handle->deleteLater();
    }
    m_handles.clear();
}

void HttpConnector::removeHandle(QNetworkReply* handle) noexcept
{
    handle->deleteLater();
    m_handles.removeOne(handle);
}

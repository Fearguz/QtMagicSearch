#ifndef HTTPCONNECTOR_H
#define HTTPCONNECTOR_H

#include <QtNetwork>

class HttpConnector : public QObject
{
    Q_OBJECT

public:
    ~HttpConnector() override;

    QNetworkReply* download(const QUrl &url, QObject *user_class, const char* downloadProgressSlot, const char* user_slot);

    void abortAll() noexcept;
    void removeHandle(QNetworkReply *handle) noexcept;

private:
    QNetworkAccessManager m_manager;
    QList<QNetworkReply*> m_handles;
};

#endif // HTTPCONNECTOR_H

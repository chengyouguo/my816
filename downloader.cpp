#include "downloader.h"
#include <QFile>
#include <QDebug>

Downloader& Downloader::instance()
{
    static Downloader inst;
    return inst;
}

Downloader::Downloader(QObject *parent)
    : QObject(parent)
{
    m_nam = new QNetworkAccessManager(this);
}

void Downloader::download(const QString &url, const QString &savePath)
{
    QNetworkRequest req{QUrl(url)};
    QNetworkReply *reply = m_nam->get(req);

    QObject::connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            emit error(reply->errorString());
            reply->deleteLater();
            return;
        }

        QFile file(savePath);
        if (!file.open(QIODevice::WriteOnly)) {
            emit error("Cannot open file for write");
            reply->deleteLater();
            return;
        }

        file.write(reply->readAll());
        file.close();

        emit finished(savePath);
        reply->deleteLater();
    });
}
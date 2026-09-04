#include "downloader.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QDebug>

Downloader& Downloader::instance()
{
    static Downloader inst;
    return inst;
}

Downloader::Downloader(QObject* parent)
    : QObject(parent)
{
    m_nam = new QNetworkAccessManager(this);
}

void Downloader::download(const QString& url, const QString& savePath)
{
    QNetworkRequest req{QUrl(url)};
    req.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");

    QNetworkReply* reply = m_nam->get(req);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            emit error(reply->errorString());
            reply->deleteLater();
            return;
        }

        QByteArray data = reply->readAll();

        // 太小 = 下载失败，不写文件
        if (data.size() < 100) {
            emit error("Downloaded data too small");
            reply->deleteLater();
            return;
        }

        QFile file(savePath);
        if (!file.open(QIODevice::WriteOnly)) {
            emit error("Cannot open file for write");
            reply->deleteLater();
            return;
        }

        file.write(data);
        file.close();

        emit finished(savePath);
        reply->deleteLater();
    });
}
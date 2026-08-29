#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>

class Downloader : public QObject
{
    Q_OBJECT
public:
    static Downloader& instance();

    void download(const QString &url, const QString &savePath);

signals:
    void finished(const QString &path);
    void error(const QString &msg);

private:
    explicit Downloader(QObject *parent = nullptr);
    ~Downloader() = default;
    Q_DISABLE_COPY(Downloader)

    QNetworkAccessManager *m_nam;
};
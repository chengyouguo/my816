#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>

class Downloader : public QObject
{
    Q_OBJECT
public:
    static Downloader& instance();

    void download(const QString& url, const QString& savePath);

signals:
    void finished(const QString& savePath);
    void error(const QString& err);

private:
    explicit Downloader(QObject* parent = nullptr);
    QNetworkAccessManager* m_nam = nullptr;
};

#endif
#pragma once
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

class AudioFetcher : public QObject {
    Q_OBJECT
public:
    explicit AudioFetcher(QObject *parent = nullptr);
    void playFile(const QString &path);
private:
    QMediaPlayer *m_player;
    QAudioOutput *m_audio;
};
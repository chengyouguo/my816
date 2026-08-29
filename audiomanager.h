#pragma once
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

class AudioManager : public QObject
{
    Q_OBJECT
public:
    static AudioManager& instance();

    void play(const QString &filePath);
    void stop();

private:
    explicit AudioManager(QObject *parent = nullptr);
    ~AudioManager() = default;
    Q_DISABLE_COPY(AudioManager)

    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
};
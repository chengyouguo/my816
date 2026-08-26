#include "audiofetcher.h"
#include <QFile>

AudioFetcher::AudioFetcher(QObject *parent)
    : QObject(parent)
    , m_player(new QMediaPlayer(this))
    , m_audio(new QAudioOutput(this))
{
    m_player->setAudioOutput(m_audio);
}

void AudioFetcher::playFile(const QString &path)
{
    if (QFile::exists(path))
        m_player->setSource(QUrl::fromLocalFile(path));
}
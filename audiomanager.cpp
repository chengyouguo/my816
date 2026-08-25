#include "audiomanager.h"
#include <QUrl>
#include <QFile>
#include <QDebug>

AudioManager& AudioManager::instance()
{
    static AudioManager inst;
    return inst;
}

AudioManager::AudioManager(QObject *parent)
    : QObject(parent)
{
    m_audioOutput = new QAudioOutput(this);
    m_player = new QMediaPlayer(this);
    m_player->setAudioOutput(m_audioOutput);
}

void AudioManager::play(const QString &filePath)
{
    if (!QFile::exists(filePath)) {
        qDebug() << "Audio file not exist:" << filePath;
        return;
    }
    m_player->setSource(QUrl::fromLocalFile(filePath));
    m_player->play();
}

void AudioManager::stop()
{
    m_player->stop();
}
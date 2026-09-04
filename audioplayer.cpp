#include "audioplayer.h"
#include "downloader.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

AudioPlayer& AudioPlayer::instance()
{
    static AudioPlayer inst;
    return inst;
}

AudioPlayer::AudioPlayer(QObject* parent)
    : QObject(parent)
{
    // 默认 baseDir = exe 所在目录
    m_baseDir = QCoreApplication::applicationDirPath();

    // ✅ Qt6 必须绑定 QAudioOutput，否则"显示 playing 但没声音"
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_audioOutput->setVolume(1.0);
    m_player->setAudioOutput(m_audioOutput);

    // 加载状态 → 加载完成才真正 play()（Qt6 异步）
    connect(m_player, &QMediaPlayer::mediaStatusChanged,
            this, &AudioPlayer::onMediaStatusChanged);

    // 播放状态 → 播完复位
    connect(m_player, &QMediaPlayer::playbackStateChanged,
            this, &AudioPlayer::onPlaybackStateChanged);

    // 下载完成 -> 触发播放
    connect(&Downloader::instance(), &Downloader::finished,
            this, &AudioPlayer::onDownloadFinished);

    // 下载失败 -> 透传错误
    connect(&Downloader::instance(), &Downloader::error,
            this, [this](const QString& err) {
        qWarning() << "AudioPlayer: download error:" << err;
        QString word = m_currentWord;
        m_currentWord.clear();
        emit errorOccurred(word, err);
    });
}

AudioPlayer::~AudioPlayer()
{
    if (m_player) {
        m_player->stop();
    }
}

void AudioPlayer::setBaseDir(const QString& dir)
{
    m_baseDir = dir;
}

QString AudioPlayer::audioDir() const
{
    return m_baseDir + "/audio";
}

QString AudioPlayer::cacheFilePath(const QString& word) const
{
    return audioDir() + "/" + word.toLower() + ".mp3";
}

bool AudioPlayer::isCached(const QString& word) const
{
    return QFileInfo::exists(cacheFilePath(word));
}

bool AudioPlayer::isPlaying() const
{
    return m_player->playbackState() == QMediaPlayer::PlayingState;
}

void AudioPlayer::stop()
{
    m_player->stop();
}

void AudioPlayer::play(const QString& word)
{
    QString w = word.trimmed().toLower();
    if (w.isEmpty())
        return;

    // 确保 audio 目录存在
    QDir dir(audioDir());
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // 已缓存 -> 直接播
    if (isCached(w)) {
        prepareAndPlay(cacheFilePath(w), w);
        return;
    }
    // 正在下载 -> 标记播放需求，等下载完自动播
    if (m_downloading.contains(w)) {
        m_pendingPlay.insert(w);
        return;
    }

    // 需要下载，下载完自动播放
    m_pendingPlay.insert(w);
    doDownload(w);

}

void AudioPlayer::playFile(const QString& filePath)
{
    if (filePath.isEmpty())
        return;

    QString word = QFileInfo(filePath).baseName().toLower();

    // 文件太小 -> 视为无效
    QFileInfo fi(filePath);
    if (!fi.exists() || fi.size() < 100) {
        qWarning() << "AudioPlayer: playFile invalid file:" << filePath;
        emit errorOccurred(word, "Audio file missing or too small");
        return;
    }

    prepareAndPlay(filePath, word);
}

void AudioPlayer::prepareAndPlay(const QString& filePath, const QString& word)
{
    if (isPlaying()) {
        m_player->stop();
    }

    m_currentWord = word;
    m_pendingFile = filePath;
    // ✅ 这一行是救命的：强制让 Qt 认为"源变了"
    m_player->setSource(QUrl());
    m_player->setSource(QUrl::fromLocalFile(filePath));

    // ⚠️ Qt6 异步：不在这里 play()，等 LoadedMedia 再播
    // （立即 play() 会导致状态机混乱、UI 卡在"播放中"）
}

void AudioPlayer::doDownload(const QString& word)
{
    m_downloading.insert(word);

    // 有道词典发音接口
    QString url = QString(
        "https://dict.youdao.com/dictvoice?audio=%1&type=2"
    ).arg(word);

    QString savePath = cacheFilePath(word);

    qDebug() << "AudioPlayer: downloading" << word << "->" << savePath;
    Downloader::instance().download(url, savePath);
}

void AudioPlayer::onDownloadFinished(const QString& filePath)
{
    QString word = QFileInfo(filePath).baseName().toLower();
    m_downloading.remove(word);

    // 文件太小视为下载失败（有道返回错误页等）
    QFileInfo fi(filePath);
    if (fi.size() < 100) {
        qWarning() << "AudioPlayer: downloaded file too small, treat as failed:"
                   << filePath;
        QFile::remove(filePath); // 删掉无效文件，下次可重试
        emit errorOccurred(word, "Downloaded audio is empty");
        return;
    }

    // 有等待播放的请求 -> 播它
    if (m_pendingPlay.contains(word)) {
        m_pendingPlay.remove(word);
        prepareAndPlay(filePath, word);
    }
}

void AudioPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::LoadedMedia ||
        status == QMediaPlayer::BufferedMedia) {
        // 加载完成 -> 真正开始播放
        if (!m_currentWord.isEmpty()) {
            m_player->play();
            emit playingStarted(m_currentWord);
        }
    } else if (status == QMediaPlayer::EndOfMedia) {
        m_player->stop(); // 触发 StoppedState -> playingFinished
    } else if (status == QMediaPlayer::InvalidMedia) {
        // 文件损坏 / 无解码器（FFmpeg 缺失）
        qWarning() << "AudioPlayer: InvalidMedia:" << m_pendingFile;
        QFile::remove(m_pendingFile); // 删掉坏文件，下次可重试
        QString word = m_currentWord;
        m_currentWord.clear();
        m_pendingFile.clear();
        emit errorOccurred(word, "Invalid media (file corrupted or FFmpeg missing)");
    }
}

void AudioPlayer::onPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    // 播完且不是被新词打断 -> 复位
    if (state == QMediaPlayer::StoppedState && !m_currentWord.isEmpty()) {
        emit playingFinished();
        m_currentWord.clear();
        m_pendingFile.clear();
    }
}

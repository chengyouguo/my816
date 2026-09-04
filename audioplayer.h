#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QString>
#include <QUrl>
#include <QSet>

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    static AudioPlayer& instance();

    /// 播放单词：自动查 exe/audio/<word>.mp3，没有就下载再播
    void play(const QString& word);

    /// 播放本地音频文件（mp3/wav 等）
    void playFile(const QString& filePath);

    /// 停止播放
    void stop();

    /// 设置 exe 所在目录（通常在 main() 里调用一次）
    void setBaseDir(const QString& dir);

    /// 是否正在播放
    bool isPlaying() const;

signals:
    void playingStarted(const QString& word);
    void playingFinished();
    void errorOccurred(const QString& word, const QString& err);

private slots:
    void onDownloadFinished(const QString& filePath);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);

private:
    explicit AudioPlayer(QObject* parent = nullptr);
    ~AudioPlayer();

    QString audioDir() const;
    QString cacheFilePath(const QString& word) const;
    bool isCached(const QString& word) const;
    void doDownload(const QString& word);
    void prepareAndPlay(const QString& filePath, const QString& word);

    QString m_baseDir;
    QMediaPlayer* m_player = nullptr;
    QAudioOutput* m_audioOutput = nullptr;

    QSet<QString> m_downloading;   // 正在下载的单词（防重复请求）
    QSet<QString> m_pendingPlay;   // 下载完需要自动播放的单词
    QString       m_currentWord;   // 当前正在播放/加载的单词
    QString       m_pendingFile;   // 等待 LoadedMedia 后 play 的文件路径
};

#endif // AUDIOPLAYER_H

#ifndef WEIGHTEDWORDPOOL_H
#define WEIGHTEDWORDPOOL_H

#include <QVector>
#include <QVariantMap>
#include <QSet>
#include <QString>
#include <random>

class WeightedWordPool
{
public:
    WeightedWordPool();

    void reset(const QVector<QVariantMap> &weightedWords);
    bool isEmpty() const;
    int remaining() const;          // 界面“剩余 N 个”用这个
    int rawPoolSize() const;        // 仅供调试
    QSet<QString> uniqueWords() const;

    QVariantMap draw();

private:
    void removeWord(const QString &word);
    void rebuildUnique();

    QVector<QVariantMap> m_pool;
    QSet<QString> m_unique;

    static std::mt19937 s_rng;
};

#endif // WEIGHTEDWORDPOOL_H
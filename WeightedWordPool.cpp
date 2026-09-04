#include "WeightedWordPool.h"

std::mt19937 WeightedWordPool::s_rng{std::random_device{}()};

WeightedWordPool::WeightedWordPool() = default;

void WeightedWordPool::reset(const QVector<QVariantMap> &weightedWords)
{
    m_pool = weightedWords;

    // ★ 空池不打乱，避免 begin()==end() 的无意义操作
    if (!m_pool.isEmpty()) {
        std::shuffle(m_pool.begin(), m_pool.end(), s_rng);
    }

    rebuildUnique();
}

bool WeightedWordPool::isEmpty() const
{
    // ★ 以“还有没有可抽的独立单词”为准，而不是 m_pool 是否为空
    return m_unique.isEmpty();
}

int WeightedWordPool::remaining() const
{
    return m_unique.size();   // 界面“剩余 N 个”用这个
}

int WeightedWordPool::rawPoolSize() const
{
    return m_pool.size();     // 仅供调试
}

QSet<QString> WeightedWordPool::uniqueWords() const
{
    return m_unique;
}

QVariantMap WeightedWordPool::draw()
{
    // ★ ① 池空直接返回空 map，绝不越界
    if (m_pool.isEmpty())
        return QVariantMap();

    // ★ ② 随机抽一个槽位
    int idx = std::uniform_int_distribution<int>(0, m_pool.size() - 1)(s_rng);

    // ★ ③ 先【值拷贝】拿走，再 erase —— 这就是修复悬空引用的关键
    QVariantMap picked = m_pool.at(idx);
    QString word = picked.value("word").toString();

    // ★ ④ 抽中后把该词所有重复项一次性移除
    removeWord(word);

    // ★ ⑤ 返回的是独立拷贝，m_pool 怎么变都不影响
    return picked;
}

void WeightedWordPool::removeWord(const QString &word)
{
    if (word.isEmpty())
        return;

    // ★ 从池里擦掉该词全部重复项
    auto it = std::remove_if(m_pool.begin(), m_pool.end(),
                             [&word](const QVariantMap &w) {
                                 return w.value("word").toString() == word;
                             });
    m_pool.erase(it, m_pool.end());

    // ★ 同步独立集合
    m_unique.remove(word);
}

void WeightedWordPool::rebuildUnique()
{
    m_unique.clear();
    for (const QVariantMap &w : m_pool)
        m_unique.insert(w.value("word").toString());
}
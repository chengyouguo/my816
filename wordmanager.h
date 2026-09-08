#ifndef WORDMANAGER_H
#define WORDMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QVariantMap>
#include <QSqlDatabase>

/*
 * WordManager —— 唯一总管
 *
 * 职责：
 *   1. 数据库初始化与表结构（words / grades / grade_word_map）
 *   2. 年级管理（增删改查、当前年级）
 *   3. 单词本管理（总表增删、年级映射增删）
 *   4. 听写 / 听看 取词（一条 SQL：ORDER BY 权重 DESC, RANDOM() LIMIT 10）
 *   5. 听写 / 听看 打分（答对降权、答错升权，封顶 5 下限 0）
 *
 * 设计要点：
 *   - 全程 QVariantMap 传递，不用 struct Word
 *   - 新词加入时 wrong_count = review_count = 5（双满权重，两端霸屏）
 *   - QML 就绪：单例 + Q_PROPERTY + signal + Q_INVOKABLE
 *
 * 表结构：
 *   words(word TEXT PRIMARY KEY, phonetic TEXT, definition TEXT, example TEXT)
 *   grades(name TEXT PRIMARY KEY)
 *   grade_word_map(grade TEXT, word TEXT, wrong_count INT DEFAULT 0,
 *                  review_count INT DEFAULT 0, last_wrong_time TEXT,
 *                  PRIMARY KEY(grade, word))
 */
class WordManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString  currentGrade READ currentGrade WRITE setCurrentGrade NOTIFY gradeChanged)
    Q_PROPERTY(QStringList grades       READ grades       NOTIFY gradesChanged)

public:
    // ---- 单例 ----
    static WordManager* instance();

    // 必须在主工程里调用一次：init(dbPath)
    bool init(const QString &dbPath);

    // ---- 年级管理 ----
    Q_INVOKABLE QStringList grades() const;
    Q_INVOKABLE QString     currentGrade() const;
    Q_INVOKABLE void        setCurrentGrade(const QString &grade);
    Q_INVOKABLE bool        addGrade(const QString &grade);
    Q_INVOKABLE bool        removeGrade(const QString &grade);

    // ---- 单词本管理 ----
    // 总表：添加/查询/删除一个单词
    Q_INVOKABLE bool   addWordToDict(const QString &word,
                                      const QString &phonetic,
                                      const QString &definition,
                                      const QString &example = QString());
    Q_INVOKABLE bool   removeWordFromDict(const QString &word);
    Q_INVOKABLE QVariantMap findWordInDict(const QString &key);  // 模糊查找
    Q_INVOKABLE QVariantList allWordsOfDict() const;             // 单词本全表

    // 年级映射：加入/移除（加入时 wrong_count = review_count = 5）
    Q_INVOKABLE bool   addWordToGrade(const QString &grade, const QString &word);
    Q_INVOKABLE bool   removeWordFromGrade(const QString &grade, const QString &word);
   Q_INVOKABLE QVector<QVariantMap> wordsOfGrade(const QString &grade) const;
    Q_INVOKABLE bool   wordInGrade(const QString &grade, const QString &word) const;

    // ---- 取词（一轮测试要考的，最多 10 个）----
    // 听写：按 wrong_count 排序
    Q_INVOKABLE QVector<QVariantMap> getSpellList(const QString &grade, int limit = 10);
    // 听看：按 review_count 排序
    Q_INVOKABLE QVector<QVariantMap> getReadList(const QString &grade, int limit = 10);

    // ---- 打分 ----
    // 听写：correct=true 答对错=true 拼错
    Q_INVOKABLE void onSpellResult(const QString &grade, const QString &word, bool correct);
    // 听看：correct=true 认识错=true 不认识
    Q_INVOKABLE void onReadResult(const QString &grade, const QString &word, bool correct);

    // ---- 数据库对象（供主工程在极少数情况下取用，如迁移脚本）----
    QSqlDatabase database() const { return m_db; }

signals:
    void gradeChanged(const QString &grade);
    void gradesChanged();

private:
    explicit WordManager(QObject *parent = nullptr);
    ~WordManager();

    // 通用取词：按指定权重列排序（wrong_count 或 review_count）
    QVector<QVariantMap> fetchList(const QString &grade,
                                   const QString &weightColumn,
                                   int limit) const;
    // 通用打分：correct 时 weight-1，否则 weight+1（封顶 5，下限 0）
    void updateWeight(const QString &grade, const QString &word,
                      const QString &weightColumn, bool correct);

    QSqlDatabase m_db;
    QString      m_currentGrade;
};

#endif // WORDMANAGER_H

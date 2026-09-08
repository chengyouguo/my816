#include "wordmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QApplication> // 添加这行以使用 qApp
// ======================================================================
// 单例
// ======================================================================
WordManager* WordManager::instance()
{
    static WordManager *s_inst = nullptr;
    if (!s_inst) {
        s_inst = new WordManager(qApp);  // 以 QApplication 为父对象，进程退出自动析构
    }
    return s_inst;
}

WordManager::WordManager(QObject *parent) : QObject(parent) {}

WordManager::~WordManager() {}

// ======================================================================
// 数据库初始化
// ======================================================================
bool WordManager::init(const QString &dbPath)
{//qDebug() << "[WordManager] init() called with dbPath:" << dbPath;
    if (m_db.isOpen()) {
        qDebug() << "[WordManager] 数据库已初始化，跳过重复调用";
        return true;
    }
    // 1. 获取或创建数据库连接
    if (QSqlDatabase::contains("wordmanager_conn")) {
        m_db = QSqlDatabase::database("wordmanager_conn");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "wordmanager_conn");
    }

    // 2. 设置数据库路径
    m_db.setDatabaseName(dbPath);

    // 3. 打开数据库
    if (!m_db.open()) {
        qDebug() << "[WordManager] 无法打开数据库:"
                 << dbPath << m_db.lastError().text();
        return false;
    }

    // 4. 建表
    QSqlQuery q(m_db);

    q.exec(R"(
        CREATE TABLE IF NOT EXISTS words (
            word       TEXT PRIMARY KEY,
            phonetic   TEXT,
            definition TEXT,
            example    TEXT
        )
    )");

    q.exec(R"(
        CREATE TABLE IF NOT EXISTS grades (
            name TEXT PRIMARY KEY
        )
    )");

    q.exec(R"(
        CREATE TABLE IF NOT EXISTS grade_word_map (
            grade           TEXT NOT NULL,
            word            TEXT NOT NULL,
            wrong_count     INTEGER DEFAULT 0,
            review_count    INTEGER DEFAULT 0,
            last_wrong_time TEXT,
            PRIMARY KEY (grade, word),
            FOREIGN KEY (grade) REFERENCES grades(name) ON DELETE CASCADE,
            FOREIGN KEY (word)  REFERENCES words(word)  ON DELETE CASCADE
        )
    )");

    // 5. 旧表兼容
    q.exec("SELECT review_count FROM grade_word_map LIMIT 1");
    if (q.lastError().isValid()) {
        qDebug() << "[WordManager] 检测到旧表，补充 review_count 列";
        q.exec("ALTER TABLE grade_word_map ADD COLUMN review_count INTEGER DEFAULT 0");
    }

    qDebug() << "[WordManager] 数据库初始化完成:" << dbPath;
    return true;
}






// ======================================================================
// 年级管理
// ======================================================================
QStringList WordManager::grades() const
{
    QStringList out;
    if (!m_db.isOpen()) return out;
    QSqlQuery q(m_db);
    q.exec("SELECT name FROM grades ORDER BY name");
    while (q.next()) out << q.value(0).toString();
    return out;
}

QString WordManager::currentGrade() const
{
    return m_currentGrade;
}

void WordManager::setCurrentGrade(const QString &grade)
{
    if (grade == m_currentGrade) return;
    m_currentGrade = grade;
    emit gradeChanged(grade);
}

bool WordManager::addGrade(const QString &grade)
{
    if (grade.isEmpty())
        return false;

    // 名字改为 g 或 gradeList，用 const 值接收（零拷贝，无悬垂风险）
    const auto gradeList = grades();
    if (gradeList.contains(grade))
        return false;

    QSqlQuery q(m_db);
    q.prepare("INSERT INTO grades (name) VALUES (?)");
    q.addBindValue(grade);

    if (!q.exec()) {
        qDebug() << "[WordManager] addGrade failed:" << q.lastError().text();
        return false;
    }

    emit gradesChanged();
    return true;
}

bool WordManager::removeGrade(const QString &grade)
{
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM grades WHERE name = ?");
    q.addBindValue(grade);
    if (!q.exec()) return false;
    if (m_currentGrade == grade) {
        m_currentGrade.clear();
        emit gradeChanged(QString());
    }
    emit gradesChanged();
    return true;
}

// ======================================================================
// 单词本管理（总表 words）
// ======================================================================
bool WordManager::addWordToDict(const QString &word,
                                 const QString &phonetic,
                                 const QString &definition,
                                 const QString &example)
{
    if (word.isEmpty()) return false;
    QSqlQuery q(m_db);
    q.prepare(R"(
        INSERT OR REPLACE INTO words (word, phonetic, definition, example)
        VALUES (?, ?, ?, ?)
    )");
    q.addBindValue(word);
    q.addBindValue(phonetic);
    q.addBindValue(definition);
    q.addBindValue(example);
    if (!q.exec()) {
        qDebug() << "[WordManager] addWordToDict failed:" << q.lastError().text();
        return false;
    }
    return true;
}

bool WordManager::removeWordFromDict(const QString &word)
{
    // 先解除所有年级映射（外键 ON DELETE CASCADE 也会处理，这里显式清更稳）
    {
        QSqlQuery q(m_db);
        q.prepare("DELETE FROM grade_word_map WHERE word = ?");
        q.addBindValue(word);
        q.exec();
    }
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM words WHERE word = ?");
    q.addBindValue(word);
    return q.exec();
}

QVariantMap WordManager::findWordInDict(const QString &key)
{
    QVariantMap out;
    if (key.isEmpty()) return out;
    QSqlQuery q(m_db);
    q.prepare("SELECT word, phonetic, definition, example FROM words "
              "WHERE word = ? OR word LIKE ? LIMIT 1");
    q.addBindValue(key);
    q.addBindValue(key + "%");
    if (q.exec() && q.next()) {
        out["word"]       = q.value(0).toString();
        out["phonetic"]   = q.value(1).toString();
        out["definition"] = q.value(2).toString();
        out["example"]    = q.value(3).toString();
    }
    return out;
}

QVariantList WordManager::allWordsOfDict() const
{
    QVariantList out;
    QSqlQuery q(m_db);
    q.exec("SELECT word, phonetic, definition, example FROM words ORDER BY word");
    while (q.next()) {
        QVariantMap w;
        w["word"]       = q.value(0).toString();
        w["phonetic"]   = q.value(1).toString();
        w["definition"] = q.value(2).toString();
        w["example"]    = q.value(3).toString();
        out.append(w);
    }
    return out;
}

// ======================================================================
// 年级-单词映射
// ======================================================================
bool WordManager::addWordToGrade(const QString &grade, const QString &word)
{
    if (grade.isEmpty() || word.isEmpty()) return false;

    // 总表必须存在该词
    if (findWordInDict(word).isEmpty()) {
        qDebug() << "[WordManager] addWordToGrade: 总表不存在" << word;
        return false;
    }

    // 已在映射中则跳过
    if (wordInGrade(grade, word)) return false;

    // 新词：wrong_count = review_count = 5（双满权重，听写听看两端霸屏）
    QSqlQuery q(m_db);
    q.prepare(R"(
        INSERT INTO grade_word_map (grade, word, wrong_count, review_count)
        VALUES (?, ?, 5, 5)
    )");
    q.addBindValue(grade);
    q.addBindValue(word);
    if (!q.exec()) {
        qDebug() << "[WordManager] addWordToGrade failed:" << q.lastError().text();
        return false;
    }
    return true;
}

bool WordManager::removeWordFromGrade(const QString &grade, const QString &word)
{
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM grade_word_map WHERE grade = ? AND word = ?");
    q.addBindValue(grade);
    q.addBindValue(word);
    return q.exec();
}

bool WordManager::wordInGrade(const QString &grade, const QString &word) const
{
    QSqlQuery q(m_db);
    q.prepare("SELECT 1 FROM grade_word_map WHERE grade = ? AND word = ?");
    q.addBindValue(grade);
    q.addBindValue(word);
    return q.exec() && q.next();
}

QVector<QVariantMap> WordManager::wordsOfGrade(const QString &grade) const
{
   QVector<QVariantMap>out;
    QSqlQuery q(m_db);
    q.prepare(R"(
        SELECT w.word, w.phonetic, w.definition, w.example,
               g.wrong_count, g.review_count
        FROM grade_word_map g JOIN words w ON w.word = g.word
        WHERE g.grade = ?
        ORDER BY g.wrong_count DESC
    )");
    q.addBindValue(grade);
    if (!q.exec()) return out;
    while (q.next()) {
        QVariantMap w;
        w["word"]        = q.value(0).toString();
        w["phonetic"]    = q.value(1).toString();
        w["definition"]  = q.value(2).toString();
        w["example"]     = q.value(3).toString();
        w["wrong_count"]  = q.value(4).toInt();
        w["review_count"] = q.value(5).toInt();
        out.append(w);
    }
    return out;
}

// ======================================================================
// 取词（核心：一条 SQL 搞定）
// ======================================================================
QVector<QVariantMap> WordManager::fetchList(const QString &grade,
                                             const QString &weightColumn,
                                             int limit) const
{
    QVector<QVariantMap> res;
    if (!m_db.isOpen() || grade.isEmpty() || limit <= 0) return res;

    // ORDER BY 权重 DESC  → 高权重优先
    //           , RANDOM() → 同权重每次随机
    // LIMIT ?            → 精确取 limit 个，不足不补齐
    const QString sql = QString(R"(
        SELECT w.word, w.phonetic, w.definition, w.example
        FROM grade_word_map g JOIN words w ON w.word = g.word
        WHERE g.grade = ?
        ORDER BY g.%1 DESC, RANDOM()
        LIMIT ?
    )").arg(weightColumn);

    QSqlQuery q(m_db);
    q.prepare(sql);
    q.addBindValue(grade);
    q.addBindValue(limit);
    if (!q.exec()) {
        qDebug() << "[WordManager] fetchList failed:" << q.lastError().text();
        return res;
    }

    while (q.next()) {
        res.append({
            {"word",       q.value(0).toString()},
            {"phonetic",   q.value(1).toString()},
            {"definition", q.value(2).toString()},
            {"example",    q.value(3).toString()}
        });
    }
    return res;
}

QVector<QVariantMap> WordManager::getSpellList(const QString &grade, int limit)
{
    return fetchList(grade, "wrong_count", limit);
}

QVector<QVariantMap> WordManager::getReadList(const QString &grade, int limit)
{
    return fetchList(grade, "review_count", limit);
}

// ======================================================================
// 打分（核心：答对降权、答错升权，封顶 5 下限 0）
// ======================================================================
void WordManager::updateWeight(const QString &grade, const QString &word,
                                const QString &weightColumn, bool correct)
{
    if (grade.isEmpty() || word.isEmpty()) return;

    // 先读当前值
    int cur = 0;
    {
        QSqlQuery q(m_db);
        q.prepare(QString("SELECT %1 FROM grade_word_map WHERE grade = ? AND word = ?")
                  .arg(weightColumn));
        q.addBindValue(grade);
        q.addBindValue(word);
        if (q.exec() && q.next()) cur = q.value(0).toInt();
    }

    int next = correct ? cur - 1 : cur + 1;
    if (next < 0)   next = 0;
    if (next > 5)   next = 5;

    QSqlQuery q(m_db);
    if (weightColumn == "wrong_count") {
        q.prepare("UPDATE grade_word_map SET wrong_count = ?, last_wrong_time = ? "
                  "WHERE grade = ? AND word = ?");
        q.addBindValue(next);
        q.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    } else {
        q.prepare(QString("UPDATE grade_word_map SET %1 = ? WHERE grade = ? AND word = ?")
                  .arg(weightColumn));
        q.addBindValue(next);
    }
    q.addBindValue(grade);
    q.addBindValue(word);
    q.exec();
}

void WordManager::onSpellResult(const QString &grade, const QString &word, bool correct)
{
    updateWeight(grade, word, "wrong_count", correct);
}

void WordManager::onReadResult(const QString &grade, const QString &word, bool correct)
{
    updateWeight(grade, word, "review_count", correct);
}

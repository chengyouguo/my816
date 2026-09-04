#include "wordmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <random>  // ✅ 必须加这一行！
#include <QSettings>
#include <QDir>
#include <QCoreApplication>
WordManager::WordManager(QObject *parent) : QObject(parent) {
    if (!init(QCoreApplication::applicationDirPath())) {
        qFatal("数据库打开失败，程序终止！请检查 mypro_data/words.db");
    }
    initCurrentGrade();
   // initCurrentGrade();
    qDebug() << "Using saved grade:" << m_currentGrade;
}

WordManager::~WordManager()
{
    if (m_db.isOpen())
        m_db.close();
}

bool WordManager::init(const QString &dbPath)
{
    QString dbDir = QDir::cleanPath(QCoreApplication::applicationDirPath()
  + "/mypro_data");

    QDir().mkpath(dbDir);  // 确保目录存在

    QString dbPath1 = dbDir + "/words.db";

    m_db = QSqlDatabase::addDatabase("QSQLITE", "word-db");
    m_db.setDatabaseName(dbPath1);

    if (!m_db.open()) {
        qDebug() << "DB open failed:" << m_db.lastError().text();
        return false;
    }

    QSqlQuery query(m_db);

    // ✅ 每张表的 SQL 都以分号结尾
    bool ok = true;

    ok &= query.exec(R"(
        CREATE TABLE IF NOT EXISTS words (
            word TEXT PRIMARY KEY,
            phonetic TEXT,
            definition TEXT,
            example TEXT,
            example_trans TEXT
        );
    )");
    if (!ok) qDebug() << "CREATE words failed:" << query.lastError().text();

   ok &=query.exec(R"(
    CREATE TABLE IF NOT EXISTS grade_word_map (
        id INTEGER PRIMARY KEY,
        grade TEXT,
        word TEXT,
        wrong_count INTEGER DEFAULT 0,
        last_wrong_time TEXT
    )
)");
     if (!ok) qDebug() << "CREATE words failed:" << query.lastError().text();
    // 兼容旧库
   query.exec("ALTER TABLE grade_word_map ADD COLUMN last_wrong_time TEXT");
    ok &= query.exec(R"(
        CREATE TABLE IF NOT EXISTS grades (
            grade TEXT PRIMARY KEY
        );
    )");
    if (!ok) qDebug() << "CREATE grades failed:" << query.lastError().text();

    return ok;
   // if (!m_currentGrade.isEmpty()) {
      //  emit gradeChanged(m_currentGrade);
  //  }
   // return true;

}
bool WordManager::gradeExists(const QString &grade) const
{
    QSqlQuery q(m_db);
    q.prepare("SELECT 1 FROM grades WHERE grade = ?");
    q.addBindValue(grade);
    return q.exec() && q.next();
}
QStringList WordManager::grades() const
{
    QStringList res;
    QSqlQuery q(m_db);
    q.exec("SELECT grade FROM grades ORDER BY grade");
    while (q.next())
        res.append(q.value(0).toString());
    return res;
}

/* ========== 模糊查找（只读总表） ========== */
QVariantMap WordManager::fuzzyFindWord(const QString &key)
{
    QSqlQuery q(m_db);
    q.prepare(R"(
        SELECT word, phonetic, definition, example, example_trans
        FROM words
         WHERE LOWER(word) LIKE LOWER(:key)
        LIMIT 1
    )");
//q.addBindValue("%" + key + "%");

//)");
    q.bindValue(":key", key.toLower() + "%");
    q.exec();

    if (q.next()) {
        return {
            {"word", q.value(0)},
            {"phonetic", q.value(1)},
            {"definition", q.value(2)},
            {"example", q.value(3)},
            {"example_trans", q.value(4)}
        };
    }
    return {};
}

/* ========== 年级词表（JOIN 查询） ========== */
QVector<QVariantMap> WordManager::wordsOfGrade(const QString &grade)
{
    QVector<QVariantMap> res;
    QSqlQuery q(m_db);
    q.prepare(R"(
        SELECT w.word, w.phonetic, w.definition
        FROM words w
        JOIN grade_word_map g ON w.word = g.word
        WHERE g.grade = ?
        ORDER BY w.word
    )");
    q.addBindValue(grade);
    q.exec();

    while (q.next()) {
        res.append({
            {"word", q.value(0)},
            {"phonetic", q.value(1)},
            {"definition", q.value(2)}
        });
    }
    return res;
}

/* ========== 添加到当前年级 ========== */
bool WordManager::addWordToGrade(const QString &grade, const QString &word)
{
    if (grade.isEmpty() || word.isEmpty())
        return false;

    // ── 1. 先查 words 总表有没有这个词 ──
    QSqlQuery q1(m_db);
    q1.prepare("SELECT 1 FROM words WHERE word = ?");
    q1.addBindValue(word);
    if (!q1.exec() || !q1.next()) {
        // 总表里没有这个词 → 不能加
        qDebug() << "words 总表中不存在:" << word;
        return false;
    }

    // ── 2. 再查年级表里有没有（你的 wordInGrade 已经有这功能） ──
    if (wordInGrade(grade, word)) {
        // 已经有了 → 不加
        qDebug() << grade << "里已经有" << word << "了，跳过";
        return false;
    }

    // ── 3. 都没有 → 插入映射 ──
    QSqlQuery q2(m_db);
    q2.prepare("INSERT INTO grade_word_map (grade, word) VALUES (?, ?)");
    q2.addBindValue(grade);
    q2.addBindValue(word);
    if (!q2.exec()) {
        qDebug() << "插入映射失败:" << q2.lastError().text();
        return false;
    }

    return true;
}

/* ========== 是否已存在 ========== */
bool WordManager::wordInGrade(const QString &grade, const QString &word)
{
    QSqlQuery q(m_db);
    q.prepare(R"(
        SELECT 1 FROM grade_word_map
        WHERE grade = ? AND word = ?
    )");
    q.addBindValue(grade);
    q.addBindValue(word);
    return q.exec() && q.next();
}

/* ========== 从年级移除 ========== */
bool WordManager::removeWordFromGrade(const QString &grade, const QString &word)
{
    QSqlQuery q(m_db);
    q.prepare(R"(
        DELETE FROM grade_word_map
        WHERE grade = ? AND word = ?
    )");
    q.addBindValue(grade);
    q.addBindValue(word);
    return q.exec();
}
bool WordManager::addGrade(const QString &grade)
{
    if (grade.trimmed().isEmpty())
        return false;

    QSqlQuery q(m_db);
    q.prepare("INSERT INTO grades (grade) VALUES (?)");
    q.addBindValue(grade.trimmed());

    bool ok = q.exec();
    if (ok)
        emit gradesChanged(); // ✅ 只发列表变化
    return ok;
}

bool WordManager::removeGrade(const QString &grade)
{
    if (grade.trimmed().isEmpty())
        return false;

    // 1. 删映射
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM grade_word_map WHERE grade = ?");
    q.addBindValue(grade);
    q.exec();

    // 2. 删年级
    q.prepare("DELETE FROM grades WHERE grade = ?");
    q.addBindValue(grade);
    bool ok = q.exec();

    if (!ok)
        return false;

    emit gradesChanged(); // ✅ 列表变了

    // 3. ⚠️ 如果删的是当前年级 → 触发切换
    if (m_currentGrade == grade) {
        QStringList gs = grades();
        if (!gs.isEmpty())
            setCurrentGrade(gs.first()); // ✅ 会发 gradeChanged
        else
            setCurrentGrade(QString());  // ✅ 会发 gradeChanged("")
    }

    return true;
}

void WordManager::setCurrentGrade(const QString &grade)
{
    if (grade.isEmpty() || !gradeExists(grade))
        return;
    if (m_currentGrade == grade)
        return;

    m_currentGrade = grade;
    emit gradeChanged(grade); // ✅ 唯一出口
}
void WordManager::increaseWrongCount(const QString &grade, const QString &word)
{
    QSqlQuery q(m_db);
    q.prepare(R"(
        UPDATE grade_word_map
        SET wrong_count = wrong_count + 1
        WHERE grade = ? AND word = ?
    )");
    q.addBindValue(grade);
    q.addBindValue(word);
    q.exec();
}
QVector<QVariantMap> WordManager::getWeightedWordsOfGrade(const QString &grade) const
{
    QVector<QVariantMap> result;

    QSqlQuery q(m_db);
    q.prepare(R"(
        SELECT w.word, w.phonetic, w.definition, g.wrong_count
        FROM words w
        JOIN grade_word_map g ON w.word = g.word
        WHERE g.grade = ?
    )");
    q.addBindValue(grade);
    q.exec();

    while (q.next()) {
        QVariantMap word;
        word["word"] = q.value(0);
        word["phonetic"] = q.value(1);
        word["definition"] = q.value(2);
        int wrong = q.value(3).toInt();

        int repeat = 1 + qMin(wrong, 5);
        for (int i = 0; i < repeat; ++i)
            result.append(word);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(result.begin(), result.end(), g);

    return result;
}
void WordManager::markWordUnknown(const QString &grade, const QString &word)
{
    qDebug() << "markWordUnknown called with grade:" << grade << "word:" << word;

    QSqlQuery q(m_db);
    q.prepare(R"(
        UPDATE grade_word_map
        SET wrong_count = wrong_count + 1,
            last_wrong_time = datetime('now')
        WHERE grade = ? AND word = ?
    )");
    q.addBindValue(grade);
    q.addBindValue(word);
    if (!q.exec()) {
        qDebug() << "标记错词失败:" << q.lastError().text();
    } else {
        qDebug() << "影响行数:" << q.numRowsAffected();
    }
}

void WordManager::markWordKnown(const QString &grade, const QString &word)
{
    QSqlQuery q(m_db);
    q.prepare(R"(
        UPDATE grade_word_map
        SET wrong_count = MAX(0, wrong_count - 1),
            review_count = review_count + 1
        WHERE grade = ? AND word = ?
    )");
    q.addBindValue(grade);
    q.addBindValue(word);
    q.exec();
}
void WordManager::initCurrentGrade()
{
    QSettings settings;
    QString last = settings.value("lastGrade").toString();

    if (!last.isEmpty() && gradeExists(last))
        setCurrentGrade(last);
    else if (!grades().isEmpty())
      setCurrentGrade(grades().constFirst());
}
QString WordManager::currentGrade() const {
    return m_currentGrade;
}
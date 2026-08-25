#pragma once
#include <QObject>
#include <QVector>
#include <QVariantMap>
#include <QSqlDatabase>

class WordManager : public QObject
{
    Q_OBJECT
public:
    explicit WordManager(QObject *parent = nullptr);
    ~WordManager();

    bool init(const QString &dbPath);

    // 年级列表
    QStringList grades() const;
    bool gradeExists(const QString &grade) const;


    // 只读总表：模糊查找
    QVariantMap fuzzyFindWord(const QString &key);

    // 年级映射表操作
    QVector<QVariantMap> wordsOfGrade(const QString &grade);
    bool addWordToGrade(const QString &grade, const QString &word);
    bool wordInGrade(const QString &grade, const QString &word);
    bool removeWordFromGrade(const QString &grade, const QString &word);
public:
    // ... 其他已有的代码（比如构造函数） ...
QVector<QVariantMap> getWeightedWordsOfGrade(const QString& grade) const;
    // ✅ 新增：声明添加年级的函数
    bool addGrade(const QString &grade);

    // ✅ 新增：声明删除年级的函数（因为后面可能用到）
    bool removeGrade(const QString &grade);

private:
    QSqlDatabase m_db;
signals:
    void gradesChanged();   // ✅ 关键信号
public:
    // ... 已有代码 ...

    // ✅ 1. 读当前年级
    QString currentGrade() const;

    // ✅ 2. 改当前年级（会发信号）
    void setCurrentGrade(const QString &grade);

signals:
    // ✅ 3. 当前年级变了（Tab 靠这个刷新）
    void gradeChanged(const QString &grade);

private:
    // ✅ 4. 内存状态（不存数据库）
    QString m_currentGrade;
    void increaseWrongCount(const QString &grade, const QString &word);
//QVector<QVariantMap> weightedWordsOfGrade(const QString &grade)const ;

public:
    // 懂了
    void markWordKnown(const QString &grade, const QString &word);
    // 不懂
    void markWordUnknown(const QString &grade, const QString &word);
  void initCurrentGrade() ;
};
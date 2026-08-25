#pragma once
#include <QWidget>
#include <QString>
#include "wordmanager.h"

class BaseTab : public QWidget
{
    Q_OBJECT
public:
    explicit BaseTab(WordManager *mgr, QWidget *parent = nullptr);
    ~BaseTab() override = default;

protected:
    // ✅ 子类必须实现：年级变化唯一入口
    virtual void onGradeChanged(const QString &grade) = 0;

    // ✅ 子类只读接口（防手贱）
    WordManager *manager() const { return m_mgr; }
    QString currentGrade() const { return m_currentGrade; }

private slots:
    // ✅ 内部中转：缓存 + 转发
    void handleGradeChanged(const QString &grade);

private:
    WordManager *m_mgr = nullptr;
    QString m_currentGrade;
};
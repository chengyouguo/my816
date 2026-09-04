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

    // ✅ 外部主动刷新
    void refresh();

    // ✅ 子类调用：首次设置年级
    void setGrade(const QString &grade);

protected:
    // ✅ 子类必须实现：年级变化唯一入口
    virtual void onGradeChanged(const QString &grade) = 0;

    // ✅ 子类可选覆写：刷新逻辑（默认 = 再调一次 onGradeChanged）
    virtual void onRefresh();

    // ✅ 只读接口
    WordManager *manager() const { return m_mgr; }
    QString currentGrade() const { return m_currentGrade; }

    // ✅ 子类 showEvent 直接调这个，不用自己写判断
    void ensureGradeAndShow();

private slots:
    void handleGradeChanged(const QString &grade);

private:
    WordManager *m_mgr = nullptr;
    QString m_currentGrade;
};